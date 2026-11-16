package com.networkthreatanalyzer.parser;

import java.nio.ByteOrder;
import com.networkthreatanalyzer.capture.CapturedPacket;
import com.networkthreatanalyzer.model.NetworkProtocol;
import com.networkthreatanalyzer.model.PacketRecord;
import com.networkthreatanalyzer.model.ParseStatus;
import com.networkthreatanalyzer.model.TransportProtocol;
import org.junit.jupiter.api.Test;
import org.pcap4j.packet.ArpPacket;
import org.pcap4j.packet.Dot1qVlanTagPacket;
import org.pcap4j.packet.EthernetPacket;
import org.pcap4j.packet.IcmpV4CommonPacket;
import org.pcap4j.packet.IcmpV4DestinationUnreachablePacket;
import org.pcap4j.packet.IcmpV4EchoPacket;
import org.pcap4j.packet.IcmpV6CommonPacket;
import org.pcap4j.packet.IcmpV6EchoRequestPacket;
import org.pcap4j.packet.IllegalPacket;
import org.pcap4j.packet.IllegalRawDataException;
import org.pcap4j.packet.IpV4Packet;
import org.pcap4j.packet.IpV4Rfc791Tos;
import org.pcap4j.packet.IpV6ExtUnknownPacket;
import org.pcap4j.packet.IpV6Packet;
import org.pcap4j.packet.IpV6SimpleFlowLabel;
import org.pcap4j.packet.IpV6SimpleTrafficClass;
import org.pcap4j.packet.Packet;
import org.pcap4j.packet.SimpleBuilder;
import org.pcap4j.packet.TcpPacket;
import org.pcap4j.packet.UdpPacket;
import org.pcap4j.packet.UnknownPacket;
import org.pcap4j.packet.factory.PacketFactories;
import org.pcap4j.packet.namednumber.ArpHardwareType;
import org.pcap4j.packet.namednumber.ArpOperation;
import org.pcap4j.packet.namednumber.DataLinkType;
import org.pcap4j.packet.namednumber.EtherType;
import org.pcap4j.packet.namednumber.IcmpV4Code;
import org.pcap4j.packet.namednumber.IcmpV4Type;
import org.pcap4j.packet.namednumber.IcmpV6Code;
import org.pcap4j.packet.namednumber.IcmpV6Type;
import org.pcap4j.packet.namednumber.IpNumber;
import org.pcap4j.packet.namednumber.IpVersion;
import org.pcap4j.packet.namednumber.TcpPort;
import org.pcap4j.packet.namednumber.UdpPort;
import org.pcap4j.util.ByteArrays;
import org.pcap4j.util.MacAddress;

import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.time.Instant;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Pure unit tests for {@link PacketParser}.
 *
 * <p>
 * Uses Pcap4J builders and {@link PacketFactories} without invoking
 * {@code Pcaps}
 * or requiring native Npcap/libpcap access.
 */
class PacketParserTest {

    private static final Instant SAMPLE_TIME = Instant.parse("2026-02-20T14:00:00.123456Z");
    private static final MacAddress SRC_MAC = MacAddress.getByName("aa:bb:cc:dd:ee:01");
    private static final MacAddress DST_MAC = MacAddress.getByName("aa:bb:cc:dd:ee:02");

    private final PacketParser parser = new PacketParser();

    private static Packet decodeFromRaw(byte[] raw, DataLinkType dlt) {
        return PacketFactories.getFactory(Packet.class, DataLinkType.class)
                .newInstance(raw, 0, raw.length, dlt);
    }

    private static CapturedPacket captured(long seq, Packet packet, int originalLength, DataLinkType dlt) {
        return new CapturedPacket(seq, packet, SAMPLE_TIME, packet.length(), originalLength, dlt);
    }

    private static Inet4Address ipv4(int a, int b, int c, int d) throws UnknownHostException {
        return (Inet4Address) InetAddress.getByAddress(new byte[] { (byte) a, (byte) b, (byte) c, (byte) d });
    }

    private static Inet6Address ipv6(String literal) throws UnknownHostException {
        return (Inet6Address) InetAddress.getByName(literal);
    }

    @Test
    void rejectsNullCapturedPacket() {
        assertThrows(NullPointerException.class, () -> parser.parse(null));
    }

    @Test
    void parsesEthernetIpv4TcpWithAllEightFlagsIncludingEceAndCwr() throws UnknownHostException {
        Inet4Address srcIp = ipv4(192, 168, 1, 10);
        Inet4Address dstIp = ipv4(10, 0, 0, 20);

        byte[] tcpBytes = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 54321))
                .dstPort(TcpPort.HTTPS)
                .sequenceNumber(100)
                .acknowledgmentNumber(200)
                .dataOffset((byte) 5)
                .fin(true)
                .syn(true)
                .rst(true)
                .psh(true)
                .ack(true)
                .urg(true)
                .reserved((byte) 0x03) // ECE (0x01) + CWR (0x02)
                .window((short) 65_535)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[12]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        // Also confirm reserved NS bit (0x04) is NOT copied into PacketRecord tcpFlags.
        tcpBytes[12] = (byte) (tcpBytes[12] | 0x01); // NS bit

        byte[] ipBytes = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 200) // >127 verifies unsigned TTL handling
                .protocol(IpNumber.TCP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(tcpBytes))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        byte[] ethBytes = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new UnknownPacket.Builder().rawData(ipBytes))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        Packet root = decodeFromRaw(ethBytes, DataLinkType.EN10MB);
        PacketRecord record = parser.parse(captured(1L, root, ethBytes.length, DataLinkType.EN10MB));

        assertEquals(1L, record.sequenceNumber());
        assertEquals(SAMPLE_TIME, record.timestamp());
        assertEquals(ethBytes.length, record.capturedLength());
        assertEquals(ethBytes.length, record.originalLength());
        assertEquals("aa:bb:cc:dd:ee:01", record.sourceMac());
        assertEquals("aa:bb:cc:dd:ee:02", record.destinationMac());
        assertEquals(NetworkProtocol.IPV4, record.networkProtocol());
        assertEquals(srcIp, record.sourceAddress());
        assertEquals(dstIp, record.destinationAddress());
        assertEquals(200, record.hopLimit());
        assertFalse(record.fragmented());
        assertEquals(TransportProtocol.TCP, record.transportProtocol());
        assertEquals(54321, record.sourcePort());
        assertEquals(443, record.destinationPort());
        assertEquals(0xFF, record.tcpFlags());
        assertTrue(record.hasTcpFlag(PacketRecord.TCP_ECE));
        assertTrue(record.hasTcpFlag(PacketRecord.TCP_CWR));
        assertEquals(12, record.payloadLength());
        assertEquals(ParseStatus.SUCCESS, record.parseStatus());
    }

    @Test
    void parsesVlanTaggedUdpAndExcludesEthernetPaddingFromPayloadLength() throws UnknownHostException {
        Inet4Address srcIp = ipv4(10, 1, 1, 2);
        Inet4Address dstIp = ipv4(8, 8, 8, 8);

        UdpPacket.Builder udpBuilder = new UdpPacket.Builder()
                .srcPort(UdpPort.getInstance((short) 50000))
                .dstPort(UdpPort.DOMAIN)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[] { 1, 2, 3, 4 }))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        IpV4Packet.Builder ipBuilder = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 64)
                .protocol(IpNumber.UDP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(udpBuilder)
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        Dot1qVlanTagPacket.Builder vlanBuilder = new Dot1qVlanTagPacket.Builder()
                .priority((byte) 0)
                .cfi(false)
                .vid((short) 100)
                .type(EtherType.IPV4)
                .payloadBuilder(ipBuilder);

        byte[] frame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.DOT1Q_VLAN_TAGGED_FRAMES)
                .payloadBuilder(vlanBuilder)
                .paddingAtBuild(true) // pads frame to 60 bytes
                .build()
                .getRawData();

        Packet root = decodeFromRaw(frame, DataLinkType.EN10MB);
        PacketRecord record = parser.parse(captured(2L, root, frame.length, DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV4, record.networkProtocol());
        assertEquals(TransportProtocol.UDP, record.transportProtocol());
        assertEquals(50000, record.sourcePort());
        assertEquals(53, record.destinationPort());
        // Must report 4 UDP payload bytes, excluding Ethernet padding.
        assertEquals(4, record.payloadLength());
        assertEquals(ParseStatus.SUCCESS, record.parseStatus());
    }

    @Test
    void doesNotLeakEmbeddedTcpHeaderInsideIcmpErrorPayload() throws UnknownHostException {
        Inet4Address attacker = ipv4(192, 168, 1, 50);
        Inet4Address router = ipv4(10, 0, 0, 1);

        byte[] innerIpTcp = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 64)
                .protocol(IpNumber.TCP)
                .srcAddr(attacker)
                .dstAddr(ipv4(172, 16, 0, 5))
                .payloadBuilder(new TcpPacket.Builder()
                        .srcPort(TcpPort.getInstance((short) 12345))
                        .dstPort(TcpPort.HTTP)
                        .syn(true)
                        .dataOffset((byte) 5)
                        .srcAddr(attacker)
                        .dstAddr(ipv4(172, 16, 0, 5))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        byte[] icmpErrorBody = ByteBuffer.allocate(4 + innerIpTcp.length)
                .putInt(0)
                .put(innerIpTcp)
                .array();

        IcmpV4CommonPacket.Builder icmpBuilder = new IcmpV4CommonPacket.Builder()
                .type(IcmpV4Type.DESTINATION_UNREACHABLE)
                .code(IcmpV4Code.HOST_UNREACHABLE)
                .payloadBuilder(new UnknownPacket.Builder().rawData(icmpErrorBody))
                .correctChecksumAtBuild(true);

        byte[] frame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .ttl((byte) 64)
                        .protocol(IpNumber.ICMPV4)
                        .srcAddr(router)
                        .dstAddr(attacker)
                        .payloadBuilder(icmpBuilder)
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        Packet root = decodeFromRaw(frame, DataLinkType.EN10MB);
        PacketRecord record = parser.parse(captured(3L, root, frame.length, DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV4, record.networkProtocol());
        assertEquals(router, record.sourceAddress());
        assertEquals(attacker, record.destinationAddress());
        assertEquals(TransportProtocol.ICMPV4, record.transportProtocol());
        assertEquals(3, record.icmpType());
        assertEquals(1, record.icmpCode());
        assertEquals(PacketRecord.UNAVAILABLE, record.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, record.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, record.tcpFlags());
        assertTrue(record.payloadLength() > 0);
        assertEquals(ParseStatus.SUCCESS, record.parseStatus());
    }

    @Test
    void parsesArpWithHardwareAndProtocolAddresses() throws UnknownHostException {
        Inet4Address senderIp = ipv4(192, 168, 1, 1);
        Inet4Address targetIp = ipv4(192, 168, 1, 25);

        byte[] arpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(MacAddress.ETHER_BROADCAST_ADDRESS)
                .type(EtherType.ARP)
                .payloadBuilder(new ArpPacket.Builder()
                        .hardwareType(ArpHardwareType.ETHERNET)
                        .protocolType(EtherType.IPV4)
                        .hardwareAddrLength((byte) MacAddress.SIZE_IN_BYTES)
                        .protocolAddrLength((byte) ByteArrays.INET4_ADDRESS_SIZE_IN_BYTES)
                        .operation(ArpOperation.REQUEST)
                        .srcHardwareAddr(SRC_MAC)
                        .srcProtocolAddr(senderIp)
                        .dstHardwareAddr(MacAddress.getByName("00:00:00:00:00:00"))
                        .dstProtocolAddr(targetIp))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        Packet root = decodeFromRaw(arpFrame, DataLinkType.EN10MB);
        PacketRecord record = parser.parse(captured(4L, root, arpFrame.length, DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.ARP, record.networkProtocol());
        assertEquals(TransportProtocol.NONE, record.transportProtocol());
        assertEquals("aa:bb:cc:dd:ee:01", record.sourceMac());
        assertEquals("ff:ff:ff:ff:ff:ff", record.destinationMac());
        assertEquals(senderIp, record.sourceAddress());
        assertEquals(targetIp, record.destinationAddress());
        assertEquals(PacketRecord.UNAVAILABLE, record.hopLimit());
        assertEquals(PacketRecord.UNAVAILABLE, record.payloadLength());
        assertEquals(ParseStatus.SUCCESS, record.parseStatus());
    }

    @Test
    void parsesIpv6IcmpV6AndIpv6NoNextHeaderAndIpv6UnknownExtension()
            throws UnknownHostException, IllegalRawDataException {
        Inet6Address srcV6 = ipv6("2001:db8::1");
        Inet6Address dstV6 = ipv6("2001:db8::2");

        // 1. ICMPv6 Echo Request (type 128 tests unsigned conversion)
        byte[] icmpV6Frame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.ICMPV6)
                        .hopLimit((byte) 255)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new IcmpV6CommonPacket.Builder()
                                .type(IcmpV6Type.ECHO_REQUEST)
                                .code(IcmpV6Code.NO_CODE)
                                .srcAddr(srcV6)
                                .dstAddr(dstV6)
                                .payloadBuilder(new IcmpV6EchoRequestPacket.Builder()
                                        .identifier((short) 1)
                                        .sequenceNumber((short) 1)
                                        .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[10])))
                                .correctChecksumAtBuild(true))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord icmp6Record = parser.parse(
                captured(5L, decodeFromRaw(icmpV6Frame, DataLinkType.EN10MB), icmpV6Frame.length, DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.IPV6, icmp6Record.networkProtocol());
        assertEquals(TransportProtocol.ICMPV6, icmp6Record.transportProtocol());
        assertEquals(255, icmp6Record.hopLimit());
        assertEquals(128, icmp6Record.icmpType());
        assertEquals(0, icmp6Record.icmpCode());
        assertEquals(14, icmp6Record.payloadLength()); // 4-byte echo id/seq + 10 bytes data
        assertEquals(ParseStatus.SUCCESS, icmp6Record.parseStatus());

        // 2. IPv6 No Next Header (59)
        byte[] noNextFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.IPV6_NONXT)
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord noNextRecord = parser.parse(
                captured(6L, decodeFromRaw(noNextFrame, DataLinkType.EN10MB), noNextFrame.length, DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.IPV6, noNextRecord.networkProtocol());
        assertEquals(TransportProtocol.NONE, noNextRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, noNextRecord.payloadLength());
        assertEquals(ParseStatus.SUCCESS, noNextRecord.parseStatus());

        // 3. Explicit IpV6ExtUnknownPacket stops extension traversal safely -> OTHER +
        // PARTIAL
        byte[] extUnknownBytes = new byte[] {
                (byte) 6, 0, 0, 0, 0, 0, 0, 0 // NextHeader = TCP (6), HdrExtLen = 0 (8 bytes)
        };
        IpV6ExtUnknownPacket extUnknown = IpV6ExtUnknownPacket.newPacket(extUnknownBytes, 0, extUnknownBytes.length);
        Packet ipv6WithUnknownExt = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.getInstance((byte) 135))
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new SimpleBuilder(extUnknown))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build();

        PacketRecord unknownExtRecord = parser.parse(
                captured(7L, ipv6WithUnknownExt, ipv6WithUnknownExt.length(), DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.IPV6, unknownExtRecord.networkProtocol());
        assertEquals(srcV6, unknownExtRecord.sourceAddress());
        assertEquals(dstV6, unknownExtRecord.destinationAddress());
        assertEquals(TransportProtocol.OTHER, unknownExtRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, unknownExtRecord.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, unknownExtRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, unknownExtRecord.parseStatus());
    }

    @Test
    void handlesNonEthernetDataLinkTypesAndUnsupportedProtocols() throws UnknownHostException {
        Inet4Address srcIp = ipv4(127, 0, 0, 1);
        Inet4Address dstIp = ipv4(127, 0, 0, 1);

        byte[] rawIpTcp = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 64)
                .protocol(IpNumber.TCP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new TcpPacket.Builder()
                        .srcPort(TcpPort.getInstance((short) 4000))
                        .dstPort(TcpPort.getInstance((short) 8080))
                        .syn(true)
                        .dataOffset((byte) 5)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        // DataLinkType.RAW
        Packet rawRoot = decodeFromRaw(rawIpTcp, DataLinkType.RAW);
        PacketRecord rawRecord = parser.parse(captured(8L, rawRoot, rawIpTcp.length, DataLinkType.RAW));
        assertNull(rawRecord.sourceMac());
        assertNull(rawRecord.destinationMac());
        assertEquals(NetworkProtocol.IPV4, rawRecord.networkProtocol());
        assertEquals(TransportProtocol.TCP, rawRecord.transportProtocol());
        assertEquals(4000, rawRecord.sourcePort());
        assertEquals(8080, rawRecord.destinationPort());
        assertEquals(ParseStatus.SUCCESS, rawRecord.parseStatus());

        // DataLinkType.NULL (BSD loopback: 4-byte AF_INET=2 header + IPv4)
        byte[] loopbackBytes = ByteBuffer.allocate(4 + rawIpTcp.length)
                .order(ByteOrder.nativeOrder())
                .putInt(2)
                .put(rawIpTcp)
                .array();
        Packet loopbackRoot = decodeFromRaw(loopbackBytes, DataLinkType.NULL);
        PacketRecord loopbackRecord = parser.parse(
                captured(9L, loopbackRoot, loopbackBytes.length, DataLinkType.NULL));
        assertNull(loopbackRecord.sourceMac());
        assertEquals(NetworkProtocol.IPV4, loopbackRecord.networkProtocol());
        assertEquals(TransportProtocol.TCP, loopbackRecord.transportProtocol());
        assertEquals(ParseStatus.SUCCESS, loopbackRecord.parseStatus());

        // DataLinkType.LINUX_SLL (16-byte header with EtherType 0x0800 at bytes 14..15)
        byte[] sllBytes = new byte[16 + rawIpTcp.length];
        sllBytes[14] = 0x08;
        sllBytes[15] = 0x00;
        System.arraycopy(rawIpTcp, 0, sllBytes, 16, rawIpTcp.length);
        Packet sllRoot = decodeFromRaw(sllBytes, DataLinkType.LINUX_SLL);
        PacketRecord sllRecord = parser.parse(captured(10L, sllRoot, sllBytes.length, DataLinkType.LINUX_SLL));
        assertNull(sllRecord.sourceMac());
        assertEquals(NetworkProtocol.IPV4, sllRecord.networkProtocol());
        assertEquals(TransportProtocol.TCP, sllRecord.transportProtocol());
        assertEquals(ParseStatus.SUCCESS, sllRecord.parseStatus());

        // Unsupported valid Ethernet protocol (LLDP 0x88CC) -> OTHER + NONE + SUCCESS
        byte[] lldpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.getInstance((short) 0x88CC))
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[46]))
                .paddingAtBuild(true)
                .build()
                .getRawData();
        PacketRecord lldpRecord = parser.parse(
                captured(11L, decodeFromRaw(lldpFrame, DataLinkType.EN10MB), lldpFrame.length, DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.OTHER, lldpRecord.networkProtocol());
        assertEquals(TransportProtocol.NONE, lldpRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, lldpRecord.payloadLength());
        assertEquals(ParseStatus.SUCCESS, lldpRecord.parseStatus());

        // Unsupported DataLinkType (IEEE802_11) -> safe PARTIAL
        Packet unsupportedDltPacket = new UnknownPacket.Builder().rawData(new byte[32]).build();
        PacketRecord unsupportedDltRecord = parser.parse(
                captured(12L, unsupportedDltPacket, 32, DataLinkType.IEEE802_11));
        assertEquals(NetworkProtocol.OTHER, unsupportedDltRecord.networkProtocol());
        assertEquals(TransportProtocol.NONE, unsupportedDltRecord.transportProtocol());
        assertEquals(ParseStatus.PARTIAL, unsupportedDltRecord.parseStatus());
    }

    @Test
    void handlesIpv4InitialAndNonInitialFragments() throws UnknownHostException {
        Inet4Address srcIp = ipv4(192, 168, 10, 1);
        Inet4Address dstIp = ipv4(192, 168, 10, 2);

        byte[] tcpBytes = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 44444))
                .dstPort(TcpPort.HTTPS)
                .syn(true)
                .dataOffset((byte) 5)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[8]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        // Initial IPv4 fragment (offset=0, MF=true): transport header must still be
        // decoded.
        byte[] initialFragEth = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .moreFragmentFlag(true)
                        .fragmentOffset((short) 0)
                        .ttl((byte) 64)
                        .protocol(IpNumber.TCP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(tcpBytes))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord initFragRecord = parser.parse(captured(
                13L, decodeFromRaw(initialFragEth, DataLinkType.EN10MB), initialFragEth.length, DataLinkType.EN10MB));
        assertTrue(initFragRecord.fragmented());
        assertEquals(TransportProtocol.TCP, initFragRecord.transportProtocol());
        assertEquals(44444, initFragRecord.sourcePort());
        assertEquals(443, initFragRecord.destinationPort());
        assertEquals(PacketRecord.TCP_SYN, initFragRecord.tcpFlags());
        assertEquals(8, initFragRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, initFragRecord.parseStatus());

        // Non-initial IPv4 fragment (offset > 0): never interpret payload as ports.
        byte[] nonInitialFragEth = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .moreFragmentFlag(false)
                        .fragmentOffset((short) 4)
                        .ttl((byte) 64)
                        .protocol(IpNumber.TCP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[24]))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord followFragRecord = parser.parse(captured(
                14L, decodeFromRaw(nonInitialFragEth, DataLinkType.EN10MB),
                nonInitialFragEth.length, DataLinkType.EN10MB));
        assertTrue(followFragRecord.fragmented());
        assertEquals(TransportProtocol.TCP, followFragRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, followFragRecord.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, followFragRecord.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, followFragRecord.tcpFlags());
        assertEquals(PacketRecord.UNAVAILABLE, followFragRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, followFragRecord.parseStatus());
    }

    @Test
    void handlesIpv6FragmentHeaderFollowedByDestinationOptionsAndNonInitialFragment() throws UnknownHostException {
        Inet6Address srcV6 = ipv6("2001:db8::10");
        Inet6Address dstV6 = ipv6("2001:db8::20");

        byte[] tcpBytes = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 33333))
                .dstPort(TcpPort.HTTP)
                .ack(true)
                .dataOffset((byte) 5)
                .srcAddr(srcV6)
                .dstAddr(dstV6)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[6]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        // Build: IPv6 -> Fragment Header (offset=0, M=1, next=60 DST_OPTS) -> DST_OPTS
        // (next=6 TCP) -> TCP
        ByteBuffer fragPlusDstOptsPlusTcp = ByteBuffer.allocate(8 + 8 + tcpBytes.length);
        // Fragment header (8 bytes): NextHeader=60 (DST_OPTS), Res=0, Offset/M=0x0001
        // (M=1), ID=1
        fragPlusDstOptsPlusTcp.put((byte) 60);
        fragPlusDstOptsPlusTcp.put((byte) 0);
        fragPlusDstOptsPlusTcp.putShort((short) 0x0001);
        fragPlusDstOptsPlusTcp.putInt(1);
        // Destination options header (8 bytes): NextHeader=6 (TCP), HdrExtLen=0, PadN
        // (type=1, len=4, 4 zeros)
        fragPlusDstOptsPlusTcp.put((byte) 6);
        fragPlusDstOptsPlusTcp.put((byte) 0);
        fragPlusDstOptsPlusTcp.put((byte) 1);
        fragPlusDstOptsPlusTcp.put((byte) 4);
        fragPlusDstOptsPlusTcp.putInt(0);
        fragPlusDstOptsPlusTcp.put(tcpBytes);

        byte[] ipv6InitialFragFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.IPV6_FRAG)
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(fragPlusDstOptsPlusTcp.array()))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord initV6Frag = parser.parse(captured(
                15L, decodeFromRaw(ipv6InitialFragFrame, DataLinkType.EN10MB),
                ipv6InitialFragFrame.length, DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.IPV6, initV6Frag.networkProtocol());
        assertTrue(initV6Frag.fragmented());
        assertEquals(TransportProtocol.TCP, initV6Frag.transportProtocol());
        assertEquals(33333, initV6Frag.sourcePort());
        assertEquals(80, initV6Frag.destinationPort());
        assertEquals(PacketRecord.TCP_ACK, initV6Frag.tcpFlags());
        assertEquals(6, initV6Frag.payloadLength());
        assertEquals(ParseStatus.PARTIAL, initV6Frag.parseStatus());

        // Non-initial IPv6 fragment (offset > 0, M=0, next=17 UDP)
        ByteBuffer nonInitFragPayload = ByteBuffer.allocate(8 + 16);
        nonInitFragPayload.put((byte) 17);
        nonInitFragPayload.put((byte) 0);
        nonInitFragPayload.putShort((short) 0x0010); // offset = 2, M = 0
        nonInitFragPayload.putInt(1);
        nonInitFragPayload.put(new byte[16]);

        byte[] ipv6FollowFragFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.IPV6_FRAG)
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(nonInitFragPayload.array()))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord followV6Frag = parser.parse(captured(
                16L, decodeFromRaw(ipv6FollowFragFrame, DataLinkType.EN10MB),
                ipv6FollowFragFrame.length, DataLinkType.EN10MB));
        assertTrue(followV6Frag.fragmented());
        assertEquals(TransportProtocol.UDP, followV6Frag.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, followV6Frag.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, followV6Frag.payloadLength());
        assertEquals(ParseStatus.PARTIAL, followV6Frag.parseStatus());
    }

    @Test
    void classifiesTcpOptionsTruncatedBeforeDeclaredHeaderLengthAsPartialAndInvalidOffsetAsMalformed()
            throws UnknownHostException {
        Inet4Address srcIp = ipv4(192, 168, 1, 10);
        Inet4Address dstIp = ipv4(10, 0, 0, 20);

        // Construct TCP header with dataOffset = 8 (32 bytes = 20 fixed + 12 options)
        byte[] rawTcpWithOptions = new byte[32];
        ByteBuffer buf = ByteBuffer.wrap(rawTcpWithOptions);
        buf.putShort((short) 12345);
        buf.putShort((short) 443);
        buf.putInt(1);
        buf.putInt(0);
        buf.put((byte) (8 << 4)); // dataOffset = 8 -> 32 bytes
        buf.put((byte) 0x02); // SYN
        buf.putShort((short) 65_535);

        byte[] fullFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .ttl((byte) 64)
                        .protocol(IpNumber.TCP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(rawTcpWithOptions))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        // Snaplen-truncate after 24 TCP bytes (14 Eth + 20 IPv4 + 24 TCP = 58 bytes <
        // 66 full).
        // >= 20 fixed TCP bytes are present, dataOffset=8 >= 5, but 24 < 32 declared
        // header bytes.
        byte[] truncatedAt58 = Arrays.copyOf(fullFrame, 58);
        Packet truncatedRoot = decodeFromRaw(truncatedAt58, DataLinkType.EN10MB);
        PacketRecord partialTcpRecord = parser.parse(
                captured(17L, truncatedRoot, fullFrame.length, DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV4, partialTcpRecord.networkProtocol());
        assertEquals(TransportProtocol.TCP, partialTcpRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, partialTcpRecord.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, partialTcpRecord.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, partialTcpRecord.tcpFlags());
        assertEquals(PacketRecord.UNAVAILABLE, partialTcpRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, partialTcpRecord.parseStatus());

        // Now test invalid dataOffset = 4 (< 5) with 24 bytes available -> MALFORMED
        byte[] invalidOffsetTcp = Arrays.copyOf(rawTcpWithOptions, 24);
        invalidOffsetTcp[12] = (byte) (4 << 4);
        byte[] malformedFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .ttl((byte) 64)
                        .protocol(IpNumber.TCP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(invalidOffsetTcp))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord malformedTcpRecord = parser.parse(captured(
                18L, decodeFromRaw(malformedFrame, DataLinkType.EN10MB),
                malformedFrame.length, DataLinkType.EN10MB));
        assertEquals(TransportProtocol.TCP, malformedTcpRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, malformedTcpRecord.payloadLength());
        assertEquals(ParseStatus.MALFORMED, malformedTcpRecord.parseStatus());
    }

    @Test
    void handlesUdpDeclaredLengthExceedingAvailableBytesInInitialFragmentAndSnaplenTruncation()
            throws UnknownHostException {
        Inet4Address srcIp = ipv4(10, 0, 0, 1);
        Inet4Address dstIp = ipv4(10, 0, 0, 2);

        // UDP header declaring 108 total UDP bytes (8 header + 100 payload)
        byte[] fullUdpBytes = new UdpPacket.Builder()
                .srcPort(UdpPort.getInstance((short) 51000))
                .dstPort(UdpPort.DOMAIN)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[100]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        // 1. Initial IPv4 fragment (offset=0, MF=true) carrying 8 UDP header + 16
        // payload bytes = 24 bytes
        byte[] first24UdpBytes = Arrays.copyOf(fullUdpBytes, 24);
        byte[] initialFragUdpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .moreFragmentFlag(true)
                        .fragmentOffset((short) 0)
                        .ttl((byte) 64)
                        .protocol(IpNumber.UDP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(first24UdpBytes))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord initFragUdpRecord = parser.parse(captured(
                19L, decodeFromRaw(initialFragUdpFrame, DataLinkType.EN10MB),
                initialFragUdpFrame.length, DataLinkType.EN10MB));
        assertTrue(initFragUdpRecord.fragmented());
        assertEquals(TransportProtocol.UDP, initFragUdpRecord.transportProtocol());
        assertEquals(51000, initFragUdpRecord.sourcePort());
        assertEquals(53, initFragUdpRecord.destinationPort());
        assertEquals(16, initFragUdpRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, initFragUdpRecord.parseStatus());

        // 2. Snaplen truncation: full 142-byte Ethernet+IPv4+UDP packet truncated at 58
        // bytes (16 UDP payload bytes)
        byte[] fullUdpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .ttl((byte) 64)
                        .protocol(IpNumber.UDP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(fullUdpBytes))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        byte[] snaplenTruncatedUdp = Arrays.copyOf(fullUdpFrame, 58); // 14 Eth + 20 IP + 8 UDP + 16 payload
        PacketRecord snaplenUdpRecord = parser.parse(captured(
                20L, decodeFromRaw(snaplenTruncatedUdp, DataLinkType.EN10MB),
                fullUdpFrame.length, DataLinkType.EN10MB));
        assertFalse(snaplenUdpRecord.fragmented());
        assertTrue(snaplenUdpRecord.isTruncated());
        assertEquals(TransportProtocol.UDP, snaplenUdpRecord.transportProtocol());
        assertEquals(51000, snaplenUdpRecord.sourcePort());
        assertEquals(53, snaplenUdpRecord.destinationPort());
        assertEquals(16, snaplenUdpRecord.payloadLength());
        assertEquals(ParseStatus.PARTIAL, snaplenUdpRecord.parseStatus());

        // 3. Invalid UDP declared length (< 8) -> MALFORMED
        byte[] invalidUdpLen = Arrays.copyOf(fullUdpBytes, 16);
        invalidUdpLen[4] = 0;
        invalidUdpLen[5] = 4; // UDP length = 4 < 8
        byte[] badUdpLenFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV4)
                .payloadBuilder(new IpV4Packet.Builder()
                        .version(IpVersion.IPV4)
                        .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                        .ihl((byte) 5)
                        .ttl((byte) 64)
                        .protocol(IpNumber.UDP)
                        .srcAddr(srcIp)
                        .dstAddr(dstIp)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(invalidUdpLen))
                        .correctChecksumAtBuild(true)
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord malformedUdpRecord = parser.parse(captured(
                21L, decodeFromRaw(badUdpLenFrame, DataLinkType.EN10MB),
                badUdpLenFrame.length, DataLinkType.EN10MB));
        assertEquals(TransportProtocol.UDP, malformedUdpRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, malformedUdpRecord.payloadLength());
        assertEquals(ParseStatus.MALFORMED, malformedUdpRecord.parseStatus());
    }

    @Test
    void calculatesIpv6TcpAndUdpPayloadLengthsUsingCapturedFrameLength()
            throws UnknownHostException {
        Inet6Address srcV6 = ipv6("2001:db8::100");
        Inet6Address dstV6 = ipv6("2001:db8::200");

        // 1. Full IPv6 UDP packet: 8-byte UDP header + 100-byte payload.
        // Destination port 53 exercises Pcap4J's DNS payload decoding.
        byte[] fullUdpBytes = new UdpPacket.Builder()
                .srcPort(UdpPort.getInstance((short) 51000))
                .dstPort(UdpPort.DOMAIN)
                .srcAddr(srcV6)
                .dstAddr(dstV6)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[100]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        byte[] fullUdpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.UDP)
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(fullUdpBytes))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord fullUdpRecord = parser.parse(captured(
                24L,
                decodeFromRaw(fullUdpFrame, DataLinkType.EN10MB),
                fullUdpFrame.length,
                DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV6, fullUdpRecord.networkProtocol());
        assertEquals(TransportProtocol.UDP, fullUdpRecord.transportProtocol());
        assertEquals(51000, fullUdpRecord.sourcePort());
        assertEquals(53, fullUdpRecord.destinationPort());
        assertEquals(100, fullUdpRecord.payloadLength());
        assertEquals(ParseStatus.SUCCESS, fullUdpRecord.parseStatus());

        // 2. Snaplen truncation: retain Ethernet + IPv6 + UDP headers
        // and only 16 bytes of the original 100-byte UDP payload.
        int truncatedFrameLength = 14 + 40 + 8 + 16;
        byte[] truncatedUdpFrame = Arrays.copyOf(fullUdpFrame, truncatedFrameLength);

        PacketRecord truncatedUdpRecord = parser.parse(captured(
                25L,
                decodeFromRaw(truncatedUdpFrame, DataLinkType.EN10MB),
                fullUdpFrame.length,
                DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV6, truncatedUdpRecord.networkProtocol());
        assertEquals(TransportProtocol.UDP, truncatedUdpRecord.transportProtocol());
        assertEquals(51000, truncatedUdpRecord.sourcePort());
        assertEquals(53, truncatedUdpRecord.destinationPort());
        assertEquals(16, truncatedUdpRecord.payloadLength());
        assertTrue(truncatedUdpRecord.isTruncated());
        assertEquals(ParseStatus.PARTIAL, truncatedUdpRecord.parseStatus());

        // 3. Full IPv6 TCP packet with a 20-byte TCP header
        // and 13 bytes of application payload.
        byte[] tcpBytes = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 33333))
                .dstPort(TcpPort.HTTP)
                .ack(true)
                .dataOffset((byte) 5)
                .srcAddr(srcV6)
                .dstAddr(dstV6)
                .payloadBuilder(new UnknownPacket.Builder().rawData(new byte[13]))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true)
                .build()
                .getRawData();

        byte[] tcpFrame = new EthernetPacket.Builder()
                .srcAddr(SRC_MAC)
                .dstAddr(DST_MAC)
                .type(EtherType.IPV6)
                .payloadBuilder(new IpV6Packet.Builder()
                        .version(IpVersion.IPV6)
                        .trafficClass(IpV6SimpleTrafficClass.newInstance((byte) 0))
                        .flowLabel(IpV6SimpleFlowLabel.newInstance(0))
                        .nextHeader(IpNumber.TCP)
                        .hopLimit((byte) 64)
                        .srcAddr(srcV6)
                        .dstAddr(dstV6)
                        .payloadBuilder(new UnknownPacket.Builder().rawData(tcpBytes))
                        .correctLengthAtBuild(true))
                .paddingAtBuild(true)
                .build()
                .getRawData();

        PacketRecord tcpRecord = parser.parse(captured(
                26L,
                decodeFromRaw(tcpFrame, DataLinkType.EN10MB),
                tcpFrame.length,
                DataLinkType.EN10MB));

        assertEquals(NetworkProtocol.IPV6, tcpRecord.networkProtocol());
        assertEquals(TransportProtocol.TCP, tcpRecord.transportProtocol());
        assertEquals(33333, tcpRecord.sourcePort());
        assertEquals(80, tcpRecord.destinationPort());
        assertEquals(13, tcpRecord.payloadLength());
        assertEquals(ParseStatus.SUCCESS, tcpRecord.parseStatus());
    }

    @Test
    void handlesMalformedEmptyAndIllegalRootPackets() {
        Packet empty = new UnknownPacket.Builder().rawData(new byte[0]).build();
        PacketRecord emptyRecord = parser.parse(captured(22L, empty, 0, DataLinkType.EN10MB));
        assertEquals(NetworkProtocol.UNKNOWN, emptyRecord.networkProtocol());
        assertEquals(TransportProtocol.NONE, emptyRecord.transportProtocol());
        assertEquals(ParseStatus.MALFORMED, emptyRecord.parseStatus());

        // Truncated 6-byte Ethernet frame decodes via factory to IllegalPacket
        Packet illegalRoot = decodeFromRaw(new byte[6], DataLinkType.EN10MB);
        assertTrue(illegalRoot instanceof IllegalPacket);
        PacketRecord illegalRecord = parser.parse(captured(23L, illegalRoot, 6, DataLinkType.EN10MB));
        assertEquals(ParseStatus.MALFORMED, illegalRecord.parseStatus());
    }
}