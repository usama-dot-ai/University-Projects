package com.networkthreatanalyzer.parser;

import com.networkthreatanalyzer.capture.CapturedPacket;
import com.networkthreatanalyzer.model.NetworkProtocol;
import com.networkthreatanalyzer.model.PacketRecord;
import com.networkthreatanalyzer.model.ParseStatus;
import com.networkthreatanalyzer.model.TransportProtocol;
import org.pcap4j.packet.ArpPacket;
import org.pcap4j.packet.BsdLoopbackPacket;
import org.pcap4j.packet.Dot1qVlanTagPacket;
import org.pcap4j.packet.EthernetPacket;
import org.pcap4j.packet.FragmentedPacket;
import org.pcap4j.packet.IcmpV4CommonPacket;
import org.pcap4j.packet.IcmpV6CommonPacket;
import org.pcap4j.packet.IllegalPacket;
import org.pcap4j.packet.IpV4Packet;
import org.pcap4j.packet.IpV6ExtFragmentPacket;
import org.pcap4j.packet.IpV6ExtOptionsPacket;
import org.pcap4j.packet.IpV6ExtRoutingPacket;
import org.pcap4j.packet.IpV6ExtUnknownPacket;
import org.pcap4j.packet.IpV6Packet;
import org.pcap4j.packet.LinuxSllPacket;
import org.pcap4j.packet.Packet;
import org.pcap4j.packet.TcpPacket;
import org.pcap4j.packet.UdpPacket;
import org.pcap4j.packet.UnknownPacket;
import org.pcap4j.packet.factory.PacketFactories;
import org.pcap4j.packet.namednumber.DataLinkType;
import org.pcap4j.packet.namednumber.EtherType;
import org.pcap4j.packet.namednumber.IpNumber;

import java.net.InetAddress;
import java.util.Objects;

/**
 * Stateless, thread-safe parser that transforms a {@link CapturedPacket} into a
 * standardized immutable {@link PacketRecord}.
 *
 * <p>
 * Uses explicit {@link DataLinkType} dispatch and layer-by-layer packet
 * traversal
 * rather than recursive {@link Packet#get(Class)} lookups so embedded packets
 * (such as
 * those inside ICMP error payloads) never overwrite outer packet metadata.
 */
public final class PacketParser {

    private static final int MAX_VLAN_DEPTH = 8;
    private static final int MAX_IPV6_EXT_DEPTH = 16;

    private static final int MIN_ETHERNET_HEADER_BYTES = 14;
    private static final int MIN_BSD_LOOPBACK_HEADER_BYTES = 4;
    private static final int MIN_LINUX_SLL_HEADER_BYTES = 16;
    private static final int MIN_IPV4_HEADER_BYTES = 20;
    private static final int MIN_IPV6_HEADER_BYTES = 40;
    private static final int MIN_ARP_HEADER_BYTES = 28;
    private static final int MIN_TCP_HEADER_BYTES = 20;
    private static final int MIN_TCP_DATA_OFFSET_WORDS = 5;
    private static final int MIN_UDP_HEADER_BYTES = 8;
    private static final int MIN_ICMP_HEADER_BYTES = 4;
    private static final int MIN_IPV6_EXT_HEADER_BYTES = 8;

    private static final int TCP_RESERVED_ECE_MASK = 0x01;
    private static final int TCP_RESERVED_CWR_MASK = 0x02;

    /**
     * Parses a single {@link CapturedPacket} into a {@link PacketRecord}.
     *
     * <p>
     * Never throws on malformed, truncated, fragmented, or unsupported packet data;
     * decode completeness is reflected in {@link PacketRecord#parseStatus()}.
     *
     * @param capturedPacket captured raw packet and metadata (non-null)
     * @return standardized immutable {@link PacketRecord}
     */
    public PacketRecord parse(CapturedPacket capturedPacket) {
        Objects.requireNonNull(capturedPacket, "capturedPacket must not be null");

        MutableParseState state = new MutableParseState(capturedPacket.isTruncated());
        Packet root = capturedPacket.packet();
        DataLinkType dlt = capturedPacket.dataLinkType();

        if (capturedPacket.capturedLength() == 0 || root instanceof IllegalPacket) {
            state.malformed = true;
            return buildRecord(capturedPacket, state);
        }

        Packet l3Packet = null;
        EtherType l3EtherType = null;
        int capturedL3Length = -1;
        boolean unsupportedDlt = false;
        if (DataLinkType.EN10MB.equals(dlt)) {
            if (root instanceof EthernetPacket eth) {
                EthernetPacket.EthernetHeader ethHeader = eth.getHeader();
                if (ethHeader == null) {
                    state.malformed = true;
                } else {
                    int l2HeaderLength = ethHeader.length();
                    if (ethHeader.getSrcAddr() != null) {
                        state.sourceMac = ethHeader.getSrcAddr().toString();
                    }
                    if (ethHeader.getDstAddr() != null) {
                        state.destinationMac = ethHeader.getDstAddr().toString();
                    }
                    l3EtherType = ethHeader.getType();
                    l3Packet = eth.getPayload();

                    int vlanDepth = 0;
                    while (l3Packet instanceof Dot1qVlanTagPacket vlan && vlanDepth < MAX_VLAN_DEPTH) {
                        Dot1qVlanTagPacket.Dot1qVlanTagHeader vlanHeader = vlan.getHeader();
                        if (vlanHeader == null) {
                            state.malformed = true;
                            l3Packet = null;
                            break;
                        }
                        l2HeaderLength += vlanHeader.length();
                        l3EtherType = vlanHeader.getType();
                        l3Packet = vlan.getPayload();
                        vlanDepth++;
                    }
                    if (vlanDepth == MAX_VLAN_DEPTH && l3Packet instanceof Dot1qVlanTagPacket) {
                        state.malformed = true;
                        l3Packet = null;
                    }
                    if (!state.malformed) {
                        capturedL3Length = Math.max(
                                0,
                                capturedPacket.capturedLength() - l2HeaderLength);
                    }
                }
            } else {
                handleUnexpectedL2Packet(root, MIN_ETHERNET_HEADER_BYTES, state);
            }
        } else if (DataLinkType.RAW.equals(dlt)) {
            l3Packet = root;
            capturedL3Length = capturedPacket.capturedLength();

            if (root instanceof IpV4Packet) {
                l3EtherType = EtherType.IPV4;
            } else if (root instanceof IpV6Packet) {
                l3EtherType = EtherType.IPV6;
            }
        } else if (DataLinkType.NULL.equals(dlt)) {

            if (root instanceof BsdLoopbackPacket loopback) {
                capturedL3Length = Math.max(
                        0,
                        capturedPacket.capturedLength() - MIN_BSD_LOOPBACK_HEADER_BYTES);

                l3Packet = loopback.getPayload();
            } else {
                handleUnexpectedL2Packet(root, MIN_BSD_LOOPBACK_HEADER_BYTES, state);
            }
        } else if (DataLinkType.LINUX_SLL.equals(dlt)) {
            if (root instanceof LinuxSllPacket sll) {
                capturedL3Length = Math.max(
                        0,
                        capturedPacket.capturedLength() - MIN_LINUX_SLL_HEADER_BYTES);
                LinuxSllPacket.LinuxSllHeader sllHeader = sll.getHeader();
                l3EtherType = sllHeader != null ? sllHeader.getProtocol() : null;
                l3Packet = sll.getPayload();
            } else {
                handleUnexpectedL2Packet(root, MIN_LINUX_SLL_HEADER_BYTES, state);
            }
        } else {
            unsupportedDlt = true;
            state.networkProtocol = NetworkProtocol.OTHER;
            state.transportProtocol = TransportProtocol.NONE;
            state.partial = true;
        }

        if (!state.malformed && !unsupportedDlt) {
            parseNetworkLayer(l3Packet, l3EtherType, capturedL3Length, state);
        }

        return buildRecord(capturedPacket, state);
    }

    private static void handleUnexpectedL2Packet(
            Packet root,
            int minL2HeaderBytes,
            MutableParseState state) {
        if (root instanceof UnknownPacket) {
            if (state.truncatedCapture && root.length() < minL2HeaderBytes) {
                state.partial = true;
            } else {
                state.networkProtocol = NetworkProtocol.OTHER;
                state.transportProtocol = TransportProtocol.NONE;
                state.partial = true;
            }
        } else {
            state.malformed = true;
        }
    }

    private static void parseNetworkLayer(
            Packet l3Packet,
            EtherType l3EtherType,
            int capturedL3Length,
            MutableParseState state) {
        if (l3Packet == null) {
            NetworkProtocol expected = mapEtherType(l3EtherType);
            state.networkProtocol = expected;
            if (expected == NetworkProtocol.OTHER) {
                state.transportProtocol = TransportProtocol.NONE;
            } else if (expected != NetworkProtocol.UNKNOWN) {
                if (state.truncatedCapture) {
                    state.partial = true;
                } else {
                    state.malformed = true;
                }
            } else {
                state.partial = true;
            }
            return;
        }

        if (l3Packet instanceof IpV4Packet ip4) {
            parseIpv4(ip4, capturedL3Length, state);
        } else if (l3Packet instanceof IpV6Packet ip6) {
            parseIpv6(ip6, capturedL3Length, state);
        } else if (l3Packet instanceof ArpPacket arp) {
            parseArp(arp, state);
        } else if (l3Packet instanceof IllegalPacket ill) {
            state.networkProtocol = mapEtherType(l3EtherType);
            int minBytes = minL3Bytes(l3EtherType);
            if (state.truncatedCapture && minBytes > 0 && ill.length() < minBytes) {
                state.partial = true;
            } else {
                state.malformed = true;
            }
        } else if (l3Packet instanceof UnknownPacket) {
            NetworkProtocol mapped = mapEtherType(l3EtherType);
            if (mapped == NetworkProtocol.IPV4 || mapped == NetworkProtocol.IPV6 || mapped == NetworkProtocol.ARP) {
                state.networkProtocol = mapped;
                state.partial = true;
            } else {
                state.networkProtocol = NetworkProtocol.OTHER;
                state.transportProtocol = TransportProtocol.NONE;
            }
        } else {
            state.networkProtocol = NetworkProtocol.OTHER;
            state.transportProtocol = TransportProtocol.NONE;
        }
    }

    private static void parseIpv4(
            IpV4Packet ip4,
            int capturedL3Length,
            MutableParseState state) {
        state.networkProtocol = NetworkProtocol.IPV4;

        IpV4Packet.IpV4Header header = ip4.getHeader();

        if (header == null) {
            state.malformed = true;
            return;
        }

        state.sourceAddress = header.getSrcAddr();
        state.destinationAddress = header.getDstAddr();
        state.hopLimit = header.getTtlAsInt();

        boolean moreFragments = header.getMoreFragmentFlag();
        short fragmentOffset = header.getFragmentOffset();

        state.fragmented = moreFragments || fragmentOffset > 0;

        if (state.fragmented) {
            state.partial = true;
        }

        IpNumber protocol = header.getProtocol();
        state.transportProtocol = mapIpNumber(protocol);

        int declaredHeaderLength = header.length();
        int declaredTotalLength = header.getTotalLengthAsInt();

        if (declaredHeaderLength < MIN_IPV4_HEADER_BYTES
                || declaredTotalLength < declaredHeaderLength) {
            state.malformed = true;
            return;
        }

        // Calculate the captured IPv4 payload length using the original
        // IPv4 packet bytes, excluding any Ethernet padding.
        int availableIpv4Length = capturedL3Length >= 0
                ? capturedL3Length
                : ip4.getRawData().length;

        int capturedIpv4Length = Math.min(
                availableIpv4Length,
                declaredTotalLength);

        int capturedIpv4PayloadLength = Math.max(
                0,
                capturedIpv4Length - declaredHeaderLength);

        if (fragmentOffset > 0) {
            // Non-initial fragment: its payload does not begin with
            // a transport-layer header.
            return;
        }

        Packet payload = ip4.getPayload();

        if (state.fragmented && payload instanceof FragmentedPacket frag) {
            // Re-decode the initial fragment's raw transport bytes because
            // Pcap4J may represent fragmented transport data as FragmentedPacket.
            byte[] rawPayload = frag.getRawData();

            if (rawPayload.length == 0) {
                state.partial = true;
                return;
            }

            int capturedTransportLength = Math.min(
                    rawPayload.length,
                    capturedIpv4PayloadLength);

            Packet redecoded = decodeByIpNumber(rawPayload, protocol);

            parseTransportLayer(
                    redecoded,
                    protocol,
                    true,
                    capturedTransportLength,
                    state);

        } else {
            // Unfragmented IPv4 packet, or a fragmented packet whose payload
            // was already decoded by Pcap4J.
            parseTransportLayer(
                    payload,
                    protocol,
                    state.fragmented,
                    capturedIpv4PayloadLength,
                    state);
        }
    }

    private static void parseIpv6(IpV6Packet ip6,
            int capturedL3Length,
            MutableParseState state) {
        state.networkProtocol = NetworkProtocol.IPV6;

        IpV6Packet.IpV6Header header = ip6.getHeader();

        if (header == null) {
            state.malformed = true;
            return;
        }

        state.sourceAddress = header.getSrcAddr();
        state.destinationAddress = header.getDstAddr();
        state.hopLimit = header.getHopLimitAsInt();

        int ipv6HeaderLength = header.length();

        int availableIpv6Length = capturedL3Length >= 0
                ? capturedL3Length
                : ip6.getRawData().length;

        if (ipv6HeaderLength < MIN_IPV6_HEADER_BYTES
                || availableIpv6Length < ipv6HeaderLength) {
            state.malformed = true;
            return;
        }

        // Calculate the captured IPv6 payload length.
        // IPv6 payload length excludes the fixed IPv6 header.
        int availableIpv6PayloadLength = availableIpv6Length - ipv6HeaderLength;

        int declaredIpv6PayloadLength = header.getPayloadLengthAsInt();

        // Preserve the existing handling of a zero payload-length field.
        int remainingCapturedPayload = declaredIpv6PayloadLength == 0
                ? availableIpv6PayloadLength
                : Math.min(
                        availableIpv6PayloadLength,
                        declaredIpv6PayloadLength);

        IpNumber nextHeader = header.getNextHeader();
        Packet current = ip6.getPayload();

        int depth = 0;
        boolean unresolvedExtension = false;
        boolean stoppedAtNonInitialFragment = false;

        while (current != null && depth < MAX_IPV6_EXT_DEPTH) {

            if (current instanceof IpV6ExtOptionsPacket optionsPacket) {
                IpV6ExtOptionsPacket.IpV6ExtOptionsHeader optHeader = optionsPacket.getHeader();

                if (optHeader == null) {
                    state.malformed = true;
                    return;
                }

                remainingCapturedPayload = Math.max(
                        0,
                        remainingCapturedPayload - optHeader.length());

                nextHeader = optHeader.getNextHeader();
                current = optionsPacket.getPayload();
                depth++;

            } else if (current instanceof IpV6ExtRoutingPacket routingPacket) {
                IpV6ExtRoutingPacket.IpV6ExtRoutingHeader routingHeader = routingPacket.getHeader();

                if (routingHeader == null) {
                    state.malformed = true;
                    return;
                }

                remainingCapturedPayload = Math.max(
                        0,
                        remainingCapturedPayload - routingHeader.length());

                nextHeader = routingHeader.getNextHeader();
                current = routingPacket.getPayload();
                depth++;

            } else if (current instanceof IpV6ExtFragmentPacket fragExt) {
                state.fragmented = true;
                state.partial = true;

                IpV6ExtFragmentPacket.IpV6ExtFragmentHeader fragHeader = fragExt.getHeader();

                if (fragHeader == null) {
                    state.malformed = true;
                    return;
                }

                remainingCapturedPayload = Math.max(
                        0,
                        remainingCapturedPayload - fragHeader.length());

                nextHeader = fragHeader.getNextHeader();

                if (fragHeader.getFragmentOffset() > 0) {
                    // Non-initial fragment: its payload does not begin
                    // with a transport-layer header.
                    stoppedAtNonInitialFragment = true;
                    current = null;
                    break;
                }

                Packet fragPayload = fragExt.getPayload();

                if (fragPayload instanceof FragmentedPacket frag) {
                    byte[] rawPayload = frag.getRawData();

                    if (rawPayload.length == 0) {
                        current = null;
                        break;
                    }

                    int capturedFragmentPayloadLength = Math.min(
                            rawPayload.length,
                            remainingCapturedPayload);

                    current = decodeByIpNumber(rawPayload, nextHeader);

                    remainingCapturedPayload = capturedFragmentPayloadLength;

                } else {
                    current = fragPayload;
                }

                depth++;

            } else if (current instanceof IpV6ExtUnknownPacket) {
                // Unsupported IPv6 extension header: stop traversal safely
                // without misclassifying it as a transport-layer protocol.
                state.transportProtocol = TransportProtocol.OTHER;
                state.partial = true;
                unresolvedExtension = true;
                current = null;
                break;

            } else {
                // Reached the transport layer or an unsupported non-extension packet.
                break;
            }
        }

        if (depth >= MAX_IPV6_EXT_DEPTH && isKnownIpv6Extension(current)) {
            state.transportProtocol = TransportProtocol.OTHER;
            state.malformed = true;
            return;
        }

        if (unresolvedExtension) {
            return;
        }

        state.transportProtocol = mapIpNumber(nextHeader);

        if (stoppedAtNonInitialFragment) {
            return;
        }

        if (IpNumber.IPV6_NONXT.equals(nextHeader)) {
            state.transportProtocol = TransportProtocol.NONE;
            return;
        }

        // Pass the remaining captured bytes after all IPv6 extension headers
        // have been accounted for.
        parseTransportLayer(
                current,
                nextHeader,
                state.fragmented,
                remainingCapturedPayload,
                state);
    }

    private static void parseArp(ArpPacket arp, MutableParseState state) {
        state.networkProtocol = NetworkProtocol.ARP;
        state.transportProtocol = TransportProtocol.NONE;

        ArpPacket.ArpHeader header = arp.getHeader();
        if (header == null) {
            state.malformed = true;
            return;
        }

        if (state.sourceMac == null && header.getSrcHardwareAddr() != null) {
            state.sourceMac = header.getSrcHardwareAddr().toString();
        }
        if (state.destinationMac == null && header.getDstHardwareAddr() != null) {
            state.destinationMac = header.getDstHardwareAddr().toString();
        }
        if (header.getSrcProtocolAddr() != null) {
            state.sourceAddress = header.getSrcProtocolAddr();
        }
        if (header.getDstProtocolAddr() != null) {
            state.destinationAddress = header.getDstProtocolAddr();
        }
    }

    private static void parseTransportLayer(
            Packet transportPacket,
            IpNumber expectedIpNumber,
            boolean inFragment,
            MutableParseState state) {

        int capturedTransportLength = transportPacket == null
                ? 0
                : transportPacket.getRawData().length;

        parseTransportLayer(
                transportPacket,
                expectedIpNumber,
                inFragment,
                capturedTransportLength,
                state);
    }

    private static void parseTransportLayer(
            Packet transportPacket,
            IpNumber expectedIpNumber,
            boolean inFragment,
            int capturedTransportLength,
            MutableParseState state) {

        if (transportPacket == null) {
            if (state.transportProtocol == TransportProtocol.OTHER
                    || state.transportProtocol == TransportProtocol.NONE) {
                return;
            }

            if (state.truncatedCapture || inFragment) {
                state.partial = true;
            } else {
                state.malformed = true;
            }
            return;
        }

        if (transportPacket instanceof TcpPacket tcp) {
            state.transportProtocol = TransportProtocol.TCP;

            TcpPacket.TcpHeader header = tcp.getHeader();

            if (header == null
                    || header.getDataOffsetAsInt() < MIN_TCP_DATA_OFFSET_WORDS) {
                state.malformed = true;
                return;
            }

            state.sourcePort = header.getSrcPort().valueAsInt();
            state.destinationPort = header.getDstPort().valueAsInt();
            state.tcpFlags = extractTcpFlags(header);

            int tcpHeaderLength = header.getDataOffsetAsInt() * 4;

            if (capturedTransportLength < tcpHeaderLength) {
                if (state.truncatedCapture || inFragment) {
                    state.partial = true;
                } else {
                    state.malformed = true;
                }
                return;
            }

            state.payloadLength = capturedTransportLength - tcpHeaderLength;

        } else if (transportPacket instanceof UdpPacket udp) {
            state.transportProtocol = TransportProtocol.UDP;

            UdpPacket.UdpHeader header = udp.getHeader();

            if (header == null
                    || header.getLengthAsInt() < MIN_UDP_HEADER_BYTES) {
                state.malformed = true;
                return;
            }

            state.sourcePort = header.getSrcPort().valueAsInt();
            state.destinationPort = header.getDstPort().valueAsInt();

            int declaredUdpLength = header.getLengthAsInt();
            int declaredPayloadBytes = declaredUdpLength - MIN_UDP_HEADER_BYTES;

            int capturedPayloadBytes = Math.max(
                    0,
                    capturedTransportLength - MIN_UDP_HEADER_BYTES);

            state.payloadLength = Math.min(
                    capturedPayloadBytes,
                    declaredPayloadBytes);

            if (capturedTransportLength < declaredUdpLength) {
                if (state.truncatedCapture || inFragment) {
                    state.partial = true;
                } else {
                    state.malformed = true;
                }
            }

        } else if (transportPacket instanceof IcmpV4CommonPacket icmp4) {
            state.transportProtocol = TransportProtocol.ICMPV4;

            IcmpV4CommonPacket.IcmpV4CommonHeader header = icmp4.getHeader();

            if (header == null
                    || header.getType() == null
                    || header.getCode() == null) {
                state.malformed = true;
                return;
            }

            state.icmpType = Byte.toUnsignedInt(header.getType().value());
            state.icmpCode = Byte.toUnsignedInt(header.getCode().value());

            state.payloadLength = Math.max(
                    0,
                    capturedTransportLength - header.length());

        } else if (transportPacket instanceof IcmpV6CommonPacket icmp6) {
            state.transportProtocol = TransportProtocol.ICMPV6;

            IcmpV6CommonPacket.IcmpV6CommonHeader header = icmp6.getHeader();

            if (header == null
                    || header.getType() == null
                    || header.getCode() == null) {
                state.malformed = true;
                return;
            }

            state.icmpType = Byte.toUnsignedInt(header.getType().value());
            state.icmpCode = Byte.toUnsignedInt(header.getCode().value());

            state.payloadLength = Math.max(
                    0,
                    capturedTransportLength - header.length());

        } else if (transportPacket instanceof IllegalPacket ill) {
            handleIllegalTransportPacket(
                    ill,
                    expectedIpNumber,
                    inFragment,
                    state);

        } else if (transportPacket instanceof UnknownPacket) {
            if (isRecognizedTransportIpNumber(expectedIpNumber)
                    || isIpv6ExtensionIpNumber(expectedIpNumber)) {
                state.partial = true;
            }
            // Otherwise, a structurally valid unsupported IP protocol
            // keeps payloadLength = -1.

        } else {
            // Other decoded L4 protocols not modeled individually map to OTHER.
            state.transportProtocol = TransportProtocol.OTHER;
        }
    }

    private static void handleIllegalTransportPacket(
            IllegalPacket ill,
            IpNumber expectedIpNumber,
            boolean inFragment,
            MutableParseState state) {
        if (IpNumber.TCP.equals(expectedIpNumber)) {
            byte[] raw = ill.getRawData();
            if (raw.length < MIN_TCP_HEADER_BYTES) {
                if (state.truncatedCapture || inFragment) {
                    state.partial = true;
                } else {
                    state.malformed = true;
                }
                return;
            }
            int dataOffsetWords = (raw[12] >>> 4) & 0x0F;
            if (dataOffsetWords < MIN_TCP_DATA_OFFSET_WORDS) {
                state.malformed = true;
                return;
            }
            int declaredTcpHeaderBytes = dataOffsetWords * 4;
            if (raw.length < declaredTcpHeaderBytes) {
                state.partial = true;
                return;
            }
            state.malformed = true;
            return;
        }

        if (IpNumber.UDP.equals(expectedIpNumber)) {
            byte[] raw = ill.getRawData();
            if (raw.length < MIN_UDP_HEADER_BYTES) {
                if (state.truncatedCapture || inFragment) {
                    state.partial = true;
                } else {
                    state.malformed = true;
                }
                return;
            }
            int udpLength = ((raw[4] & 0xFF) << 8) | (raw[5] & 0xFF);
            if (udpLength < MIN_UDP_HEADER_BYTES) {
                state.malformed = true;
                return;
            }
            if (udpLength > raw.length && (state.truncatedCapture || inFragment)) {
                state.sourcePort = ((raw[0] & 0xFF) << 8) | (raw[1] & 0xFF);
                state.destinationPort = ((raw[2] & 0xFF) << 8) | (raw[3] & 0xFF);
                state.payloadLength = raw.length - MIN_UDP_HEADER_BYTES;
                state.partial = true;
                return;
            }
            state.malformed = true;
            return;
        }

        int minBytes = minHeaderBytesForIpNumber(expectedIpNumber);
        if ((state.truncatedCapture || inFragment) && minBytes > 0 && ill.length() < minBytes) {
            state.partial = true;
        } else {
            state.malformed = true;
        }
    }

    private static int extractTcpFlags(TcpPacket.TcpHeader header) {
        int flags = 0;
        if (header.getFin()) {
            flags |= PacketRecord.TCP_FIN;
        }
        if (header.getSyn()) {
            flags |= PacketRecord.TCP_SYN;
        }
        if (header.getRst()) {
            flags |= PacketRecord.TCP_RST;
        }
        if (header.getPsh()) {
            flags |= PacketRecord.TCP_PSH;
        }
        if (header.getAck()) {
            flags |= PacketRecord.TCP_ACK;
        }
        if (header.getUrg()) {
            flags |= PacketRecord.TCP_URG;
        }
        // Pcap4J 1.8.2 stores ECE in bit 0 (0x01) and CWR in bit 1 (0x02) of its 6-bit
        // reserved field.
        byte reserved = header.getReserved();
        if ((reserved & TCP_RESERVED_ECE_MASK) != 0) {
            flags |= PacketRecord.TCP_ECE;
        }
        if ((reserved & TCP_RESERVED_CWR_MASK) != 0) {
            flags |= PacketRecord.TCP_CWR;
        }
        return flags;
    }

    private static Packet decodeByIpNumber(byte[] rawPayload, IpNumber ipNumber) {
        if (ipNumber == null) {
            return UnknownPacket.newPacket(rawPayload, 0, rawPayload.length);
        }
        return PacketFactories.getFactory(Packet.class, IpNumber.class)
                .newInstance(rawPayload, 0, rawPayload.length, ipNumber);
    }

    private static NetworkProtocol mapEtherType(EtherType etherType) {
        if (etherType == null) {
            return NetworkProtocol.UNKNOWN;
        }
        if (EtherType.IPV4.equals(etherType)) {
            return NetworkProtocol.IPV4;
        }
        if (EtherType.IPV6.equals(etherType)) {
            return NetworkProtocol.IPV6;
        }
        if (EtherType.ARP.equals(etherType)) {
            return NetworkProtocol.ARP;
        }
        return NetworkProtocol.OTHER;
    }

    private static TransportProtocol mapIpNumber(IpNumber ipNumber) {
        if (ipNumber == null) {
            return TransportProtocol.OTHER;
        }
        if (IpNumber.TCP.equals(ipNumber)) {
            return TransportProtocol.TCP;
        }
        if (IpNumber.UDP.equals(ipNumber)) {
            return TransportProtocol.UDP;
        }
        if (IpNumber.ICMPV4.equals(ipNumber)) {
            return TransportProtocol.ICMPV4;
        }
        if (IpNumber.ICMPV6.equals(ipNumber)) {
            return TransportProtocol.ICMPV6;
        }
        if (IpNumber.IPV6_NONXT.equals(ipNumber)) {
            return TransportProtocol.NONE;
        }
        return TransportProtocol.OTHER;
    }

    private static boolean isRecognizedTransportIpNumber(IpNumber ipNumber) {
        return IpNumber.TCP.equals(ipNumber)
                || IpNumber.UDP.equals(ipNumber)
                || IpNumber.ICMPV4.equals(ipNumber)
                || IpNumber.ICMPV6.equals(ipNumber);
    }

    private static boolean isIpv6ExtensionIpNumber(IpNumber ipNumber) {
        return IpNumber.IPV6_HOPOPT.equals(ipNumber)
                || IpNumber.IPV6_ROUTE.equals(ipNumber)
                || IpNumber.IPV6_FRAG.equals(ipNumber)
                || IpNumber.IPV6_DST_OPTS.equals(ipNumber)
                || IpNumber.AH.equals(ipNumber)
                || IpNumber.ESP.equals(ipNumber);
    }

    private static boolean isKnownIpv6Extension(Packet packet) {
        return packet instanceof IpV6ExtOptionsPacket
                || packet instanceof IpV6ExtRoutingPacket
                || packet instanceof IpV6ExtFragmentPacket
                || packet instanceof IpV6ExtUnknownPacket;
    }

    private static int minL3Bytes(EtherType etherType) {
        if (EtherType.IPV4.equals(etherType)) {
            return MIN_IPV4_HEADER_BYTES;
        }
        if (EtherType.IPV6.equals(etherType)) {
            return MIN_IPV6_HEADER_BYTES;
        }
        if (EtherType.ARP.equals(etherType)) {
            return MIN_ARP_HEADER_BYTES;
        }
        return 0;
    }

    private static int minHeaderBytesForIpNumber(IpNumber ipNumber) {
        if (IpNumber.TCP.equals(ipNumber)) {
            return MIN_TCP_HEADER_BYTES;
        }
        if (IpNumber.UDP.equals(ipNumber)) {
            return MIN_UDP_HEADER_BYTES;
        }
        if (IpNumber.ICMPV4.equals(ipNumber) || IpNumber.ICMPV6.equals(ipNumber)) {
            return MIN_ICMP_HEADER_BYTES;
        }
        if (isIpv6ExtensionIpNumber(ipNumber)) {
            return MIN_IPV6_EXT_HEADER_BYTES;
        }
        return 0;
    }

    private static PacketRecord buildRecord(CapturedPacket capturedPacket, MutableParseState state) {
        ParseStatus status;
        if (state.malformed) {
            status = ParseStatus.MALFORMED;
        } else if (state.partial || state.truncatedCapture || state.fragmented) {
            status = ParseStatus.PARTIAL;
        } else {
            status = ParseStatus.SUCCESS;
        }

        return PacketRecord.builder()
                .sequenceNumber(capturedPacket.sequenceNumber())
                .timestamp(capturedPacket.timestamp())
                .capturedLength(capturedPacket.capturedLength())
                .originalLength(capturedPacket.originalLength())
                .sourceMac(state.sourceMac)
                .destinationMac(state.destinationMac)
                .networkProtocol(state.networkProtocol)
                .sourceAddress(state.sourceAddress)
                .destinationAddress(state.destinationAddress)
                .hopLimit(state.hopLimit)
                .fragmented(state.fragmented)
                .transportProtocol(state.transportProtocol)
                .sourcePort(state.sourcePort)
                .destinationPort(state.destinationPort)
                .tcpFlags(state.tcpFlags)
                .icmpType(state.icmpType)
                .icmpCode(state.icmpCode)
                .payloadLength(state.payloadLength)
                .parseStatus(status)
                .build();
    }

    private static final class MutableParseState {
        private final boolean truncatedCapture;
        private String sourceMac;
        private String destinationMac;
        private NetworkProtocol networkProtocol = NetworkProtocol.UNKNOWN;
        private InetAddress sourceAddress;
        private InetAddress destinationAddress;
        private int hopLimit = PacketRecord.UNAVAILABLE;
        private boolean fragmented;
        private TransportProtocol transportProtocol = TransportProtocol.NONE;
        private int sourcePort = PacketRecord.UNAVAILABLE;
        private int destinationPort = PacketRecord.UNAVAILABLE;
        private int tcpFlags = PacketRecord.UNAVAILABLE;
        private int icmpType = PacketRecord.UNAVAILABLE;
        private int icmpCode = PacketRecord.UNAVAILABLE;
        private int payloadLength = PacketRecord.UNAVAILABLE;
        private boolean partial;
        private boolean malformed;

        private MutableParseState(boolean truncatedCapture) {
            this.truncatedCapture = truncatedCapture;
        }
    }
}