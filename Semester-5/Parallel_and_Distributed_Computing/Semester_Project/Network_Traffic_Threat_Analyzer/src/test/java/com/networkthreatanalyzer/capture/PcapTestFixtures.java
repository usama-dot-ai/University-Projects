package com.networkthreatanalyzer.capture;

import org.pcap4j.packet.EthernetPacket;
import org.pcap4j.packet.IpV4Packet;
import org.pcap4j.packet.IpV4Rfc791Tos;
import org.pcap4j.packet.TcpPacket;
import org.pcap4j.packet.UdpPacket;
import org.pcap4j.packet.UnknownPacket;
import org.pcap4j.packet.namednumber.EtherType;
import org.pcap4j.packet.namednumber.IpNumber;
import org.pcap4j.packet.namednumber.IpVersion;
import org.pcap4j.packet.namednumber.TcpPort;
import org.pcap4j.packet.namednumber.UdpPort;
import org.pcap4j.util.MacAddress;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Instant;
import java.util.List;

/**
 * Programmatic fixture factory for generating classic PCAP and negative test
 * files in {@code @TempDir}.
 */
final class PcapTestFixtures {

    private static final int MAGIC_MICRO = 0xA1B2C3D4;
    private static final int MAGIC_NANO = 0xA1B23C4D;
    private static final int LINKTYPE_ETHERNET = 1;
    private static final int DEFAULT_SNAPLEN = 65_535;

    private PcapTestFixtures() {
    }

    record RawPcapRecord(Instant timestamp, byte[] capturedData, int originalLength) {
    }

    static byte[] buildEthernetIpv4TcpSynPacket() throws UnknownHostException {
        Inet4Address srcIp = (Inet4Address) InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 10 });
        Inet4Address dstIp = (Inet4Address) InetAddress.getByAddress(new byte[] { 10, 0, 0, 20 });

        TcpPacket.Builder tcpBuilder = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 54321))
                .dstPort(TcpPort.HTTPS)
                .sequenceNumber(1000)
                .acknowledgmentNumber(0)
                .dataOffset((byte) 5)
                .syn(true)
                .window((short) 65_535)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        IpV4Packet.Builder ipv4Builder = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 64)
                .protocol(IpNumber.TCP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(tcpBuilder)
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        EthernetPacket eth = new EthernetPacket.Builder()
                .srcAddr(MacAddress.getByName("aa:bb:cc:dd:ee:01"))
                .dstAddr(MacAddress.getByName("aa:bb:cc:dd:ee:02"))
                .type(EtherType.IPV4)
                .payloadBuilder(ipv4Builder)
                .paddingAtBuild(true)
                .build();

        return eth.getRawData();
    }

    static byte[] buildEthernetIpv4TcpPayloadPacket(byte[] tcpPayload) throws UnknownHostException {
        Inet4Address srcIp = (Inet4Address) InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 10 });
        Inet4Address dstIp = (Inet4Address) InetAddress.getByAddress(new byte[] { 10, 0, 0, 20 });

        TcpPacket.Builder tcpBuilder = new TcpPacket.Builder()
                .srcPort(TcpPort.getInstance((short) 54321))
                .dstPort(TcpPort.HTTPS)
                .sequenceNumber(1001)
                .acknowledgmentNumber(500)
                .dataOffset((byte) 5)
                .ack(true)
                .psh(true)
                .window((short) 65_535)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(tcpPayload))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        IpV4Packet.Builder ipv4Builder = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 64)
                .protocol(IpNumber.TCP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(tcpBuilder)
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        EthernetPacket eth = new EthernetPacket.Builder()
                .srcAddr(MacAddress.getByName("aa:bb:cc:dd:ee:01"))
                .dstAddr(MacAddress.getByName("aa:bb:cc:dd:ee:02"))
                .type(EtherType.IPV4)
                .payloadBuilder(ipv4Builder)
                .paddingAtBuild(true)
                .build();

        return eth.getRawData();
    }

    static byte[] buildEthernetIpv4UdpPacket(byte[] udpPayload) throws UnknownHostException {
        Inet4Address srcIp = (Inet4Address) InetAddress.getByAddress(new byte[] { 10, 0, 0, 20 });
        Inet4Address dstIp = (Inet4Address) InetAddress.getByAddress(new byte[] { 8, 8, 8, 8 });

        UdpPacket.Builder udpBuilder = new UdpPacket.Builder()
                .srcPort(UdpPort.getInstance((short) 55555))
                .dstPort(UdpPort.DOMAIN)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(new UnknownPacket.Builder().rawData(udpPayload))
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        IpV4Packet.Builder ipv4Builder = new IpV4Packet.Builder()
                .version(IpVersion.IPV4)
                .tos(IpV4Rfc791Tos.newInstance((byte) 0))
                .ihl((byte) 5)
                .ttl((byte) 128)
                .protocol(IpNumber.UDP)
                .srcAddr(srcIp)
                .dstAddr(dstIp)
                .payloadBuilder(udpBuilder)
                .correctChecksumAtBuild(true)
                .correctLengthAtBuild(true);

        EthernetPacket eth = new EthernetPacket.Builder()
                .srcAddr(MacAddress.getByName("aa:bb:cc:dd:ee:02"))
                .dstAddr(MacAddress.getByName("aa:bb:cc:dd:ee:03"))
                .type(EtherType.IPV4)
                .payloadBuilder(ipv4Builder)
                .paddingAtBuild(true)
                .build();

        return eth.getRawData();
    }

    static void writeClassicPcap(
            Path target,
            ByteOrder byteOrder,
            boolean nanosecondMagic,
            List<RawPcapRecord> records) throws IOException {
        int magic = nanosecondMagic ? MAGIC_NANO : MAGIC_MICRO;
        ByteBuffer globalHeader = ByteBuffer.allocate(24).order(byteOrder);
        globalHeader.putInt(magic);
        globalHeader.putShort((short) 2);
        globalHeader.putShort((short) 4);
        globalHeader.putInt(0);
        globalHeader.putInt(0);
        globalHeader.putInt(DEFAULT_SNAPLEN);
        globalHeader.putInt(LINKTYPE_ETHERNET);

        try (OutputStream out = Files.newOutputStream(target)) {
            out.write(globalHeader.array());
            ByteBuffer packetHeader = ByteBuffer.allocate(16).order(byteOrder);
            for (RawPcapRecord record : records) {
                packetHeader.clear();
                int tsSec = Math.toIntExact(record.timestamp().getEpochSecond());
                int tsSubSec = nanosecondMagic
                        ? record.timestamp().getNano()
                        : record.timestamp().getNano() / 1_000;
                packetHeader.putInt(tsSec);
                packetHeader.putInt(tsSubSec);
                packetHeader.putInt(record.capturedData().length);
                packetHeader.putInt(record.originalLength());
                out.write(packetHeader.array());
                out.write(record.capturedData());
            }
        }
    }

    static void writeEmptyClassicPcap(Path target) throws IOException {
        writeClassicPcap(target, ByteOrder.LITTLE_ENDIAN, false, List.of());
    }

    static void writePcapngHeader(Path target) throws IOException {
        ByteBuffer shb = ByteBuffer.allocate(28).order(ByteOrder.LITTLE_ENDIAN);
        shb.putInt(0x0A0D0D0A); // Section Header Block Type
        shb.putInt(28); // Block Total Length
        shb.putInt(0x1A2B3C4D); // Byte-Order Magic
        shb.putShort((short) 1);
        shb.putShort((short) 0);
        shb.putLong(-1L);
        shb.putInt(28);
        Files.write(target, shb.array());
    }
}