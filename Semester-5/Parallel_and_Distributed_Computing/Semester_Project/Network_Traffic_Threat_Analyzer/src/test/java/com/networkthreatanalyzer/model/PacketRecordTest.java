package com.networkthreatanalyzer.model;

import org.junit.jupiter.api.Test;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.time.Instant;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PacketRecordTest {

    private static final Instant SAMPLE_TIME = Instant.parse("2026-01-15T10:30:00Z");

    private static PacketRecord.Builder validTcpBuilder() throws UnknownHostException {
        return PacketRecord.builder()
                .sequenceNumber(1L)
                .timestamp(SAMPLE_TIME)
                .capturedLength(74)
                .originalLength(74)
                .sourceMac("aa:bb:cc:dd:ee:01")
                .destinationMac("aa:bb:cc:dd:ee:02")
                .networkProtocol(NetworkProtocol.IPV4)
                .sourceAddress(InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 10 }))
                .destinationAddress(InetAddress.getByAddress(new byte[] { 10, 0, 0, 5 }))
                .hopLimit(64)
                .fragmented(false)
                .transportProtocol(TransportProtocol.TCP)
                .sourcePort(54321)
                .destinationPort(443)
                .tcpFlags(PacketRecord.TCP_SYN | PacketRecord.TCP_ACK)
                .icmpType(PacketRecord.UNAVAILABLE)
                .icmpCode(PacketRecord.UNAVAILABLE)
                .payloadLength(0)
                .parseStatus(ParseStatus.SUCCESS);
    }

    @Test
    void constructsValidIpv4TcpRecord() throws UnknownHostException {
        InetAddress src = InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 10 });
        InetAddress dst = InetAddress.getByAddress(new byte[] { 10, 0, 0, 5 });

        PacketRecord record = new PacketRecord(
                1L,
                SAMPLE_TIME,
                74,
                74,
                "aa:bb:cc:dd:ee:01",
                "aa:bb:cc:dd:ee:02",
                NetworkProtocol.IPV4,
                src,
                dst,
                64,
                false,
                TransportProtocol.TCP,
                54321,
                443,
                PacketRecord.TCP_SYN | PacketRecord.TCP_ACK,
                PacketRecord.UNAVAILABLE,
                PacketRecord.UNAVAILABLE,
                0,
                ParseStatus.SUCCESS);

        assertEquals(1L, record.sequenceNumber());
        assertEquals(SAMPLE_TIME, record.timestamp());
        assertEquals(74, record.capturedLength());
        assertEquals(74, record.originalLength());
        assertEquals("aa:bb:cc:dd:ee:01", record.sourceMac());
        assertEquals("aa:bb:cc:dd:ee:02", record.destinationMac());
        assertEquals(NetworkProtocol.IPV4, record.networkProtocol());
        assertEquals(src, record.sourceAddress());
        assertEquals(dst, record.destinationAddress());
        assertEquals(64, record.hopLimit());
        assertFalse(record.fragmented());
        assertEquals(TransportProtocol.TCP, record.transportProtocol());
        assertEquals(54321, record.sourcePort());
        assertEquals(443, record.destinationPort());
        assertEquals(PacketRecord.TCP_SYN | PacketRecord.TCP_ACK, record.tcpFlags());
        assertEquals(PacketRecord.UNAVAILABLE, record.icmpType());
        assertEquals(PacketRecord.UNAVAILABLE, record.icmpCode());
        assertEquals(0, record.payloadLength());
        assertEquals(ParseStatus.SUCCESS, record.parseStatus());
        assertFalse(record.isTruncated());
        assertTrue(record.hasTcpFlag(PacketRecord.TCP_SYN));
        assertTrue(record.hasTcpFlag(PacketRecord.TCP_SYN | PacketRecord.TCP_ACK));
        assertFalse(record.hasTcpFlag(PacketRecord.TCP_RST));
    }

    @Test
    void constructsValidIpv6IcmpRecordWithUnavailablePortsAndFlags() throws UnknownHostException {
        InetAddress srcV6 = InetAddress.getByName("2001:db8::1");
        InetAddress dstV6 = InetAddress.getByName("2001:db8::2");

        PacketRecord record = PacketRecord.builder()
                .sequenceNumber(2L)
                .timestamp(SAMPLE_TIME)
                .capturedLength(118)
                .originalLength(118)
                .networkProtocol(NetworkProtocol.IPV6)
                .sourceAddress(srcV6)
                .destinationAddress(dstV6)
                .hopLimit(255)
                .transportProtocol(TransportProtocol.ICMPV6)
                .icmpType(128)
                .icmpCode(0)
                .payloadLength(56)
                .parseStatus(ParseStatus.SUCCESS)
                .build();

        assertNull(record.sourceMac());
        assertNull(record.destinationMac());
        assertEquals(PacketRecord.UNAVAILABLE, record.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, record.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, record.tcpFlags());
        assertEquals(128, record.icmpType());
        assertEquals(0, record.icmpCode());
        assertEquals(56, record.payloadLength());
        assertFalse(record.hasTcpFlag(PacketRecord.TCP_SYN));
    }

    @Test
    void constructsValidArpRecordWithoutTransportLayer() throws UnknownHostException {
        InetAddress senderIp = InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 1 });
        InetAddress targetIp = InetAddress.getByAddress(new byte[] { (byte) 192, (byte) 168, 1, 20 });

        PacketRecord arpRecord = PacketRecord.builder()
                .sequenceNumber(3L)
                .timestamp(SAMPLE_TIME)
                .capturedLength(42)
                .originalLength(42)
                .sourceMac("aa:bb:cc:dd:ee:01")
                .destinationMac("ff:ff:ff:ff:ff:ff")
                .networkProtocol(NetworkProtocol.ARP)
                .sourceAddress(senderIp)
                .destinationAddress(targetIp)
                .transportProtocol(TransportProtocol.NONE)
                .parseStatus(ParseStatus.SUCCESS)
                .build();

        assertEquals(NetworkProtocol.ARP, arpRecord.networkProtocol());
        assertEquals(TransportProtocol.NONE, arpRecord.transportProtocol());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.hopLimit());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.tcpFlags());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.icmpType());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.icmpCode());
        assertEquals(PacketRecord.UNAVAILABLE, arpRecord.payloadLength());
    }

    @Test
    void constructsValidPartialAndMalformedRecordsWithNullAddressesAndAllUnavailableNumbers() {
        PacketRecord partialFragment = PacketRecord.builder()
                .sequenceNumber(4L)
                .timestamp(SAMPLE_TIME)
                .capturedLength(64)
                .originalLength(1500)
                .networkProtocol(NetworkProtocol.IPV4)
                .hopLimit(0)
                .fragmented(true)
                .transportProtocol(TransportProtocol.UDP)
                .parseStatus(ParseStatus.PARTIAL)
                .build();

        assertTrue(partialFragment.isTruncated());
        assertNull(partialFragment.sourceAddress());
        assertNull(partialFragment.destinationAddress());
        assertEquals(PacketRecord.UNAVAILABLE, partialFragment.sourcePort());
        assertEquals(PacketRecord.UNAVAILABLE, partialFragment.destinationPort());
        assertEquals(PacketRecord.UNAVAILABLE, partialFragment.payloadLength());

        PacketRecord malformed = PacketRecord.builder()
                .sequenceNumber(5L)
                .timestamp(SAMPLE_TIME)
                .capturedLength(0)
                .originalLength(0)
                .networkProtocol(NetworkProtocol.UNKNOWN)
                .transportProtocol(TransportProtocol.NONE)
                .parseStatus(ParseStatus.MALFORMED)
                .build();

        assertEquals(ParseStatus.MALFORMED, malformed.parseStatus());
        assertEquals(PacketRecord.UNAVAILABLE, malformed.hopLimit());
        assertEquals(PacketRecord.UNAVAILABLE, malformed.payloadLength());
    }

    @Test
    void allowsBoundaryNumericValues() throws UnknownHostException {
        PacketRecord record = validTcpBuilder()
                .hopLimit(0)
                .sourcePort(0)
                .destinationPort(65_535)
                .tcpFlags(0)
                .icmpType(255)
                .icmpCode(255)
                .payloadLength(0)
                .build();

        assertEquals(0, record.hopLimit());
        assertEquals(0, record.sourcePort());
        assertEquals(65_535, record.destinationPort());
        assertEquals(0, record.tcpFlags());
        assertEquals(255, record.icmpType());
        assertEquals(255, record.icmpCode());
        assertFalse(record.hasTcpFlag(PacketRecord.TCP_FIN));

        PacketRecord maxTcpFlags = validTcpBuilder()
                .tcpFlags(255)
                .build();
        assertTrue(maxTcpFlags.hasTcpFlag(PacketRecord.TCP_CWR));
    }

    @Test
    void rejectsNullRequiredFields() throws UnknownHostException {
        assertThrows(NullPointerException.class, () -> validTcpBuilder().timestamp(null).build());
        assertThrows(NullPointerException.class, () -> validTcpBuilder().networkProtocol(null).build());
        assertThrows(NullPointerException.class, () -> validTcpBuilder().transportProtocol(null).build());
        assertThrows(NullPointerException.class, () -> validTcpBuilder().parseStatus(null).build());
    }

    @Test
    void rejectsInvalidSequenceAndLengthValues() throws UnknownHostException {
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().sequenceNumber(0).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().sequenceNumber(-1).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().capturedLength(-1).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().originalLength(-1).build());
        assertThrows(IllegalArgumentException.class,
                () -> validTcpBuilder().capturedLength(100).originalLength(64).build());
    }

    @Test
    void rejectsBlankMacAddresses() throws UnknownHostException {
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().sourceMac("").build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().destinationMac("   ").build());
    }

    @Test
    void rejectsOutOfRangeProtocolNumericValues() throws UnknownHostException {
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().hopLimit(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().hopLimit(256).build());

        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().sourcePort(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().sourcePort(65_536).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().destinationPort(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().destinationPort(65_536).build());

        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().tcpFlags(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().tcpFlags(256).build());

        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().icmpType(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().icmpType(256).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().icmpCode(-2).build());
        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().icmpCode(256).build());

        assertThrows(IllegalArgumentException.class, () -> validTcpBuilder().payloadLength(-2).build());
    }

    @Test
    void rejectsInvalidFlagMaskInHasTcpFlag() throws UnknownHostException {
        PacketRecord record = validTcpBuilder().build();
        assertThrows(IllegalArgumentException.class, () -> record.hasTcpFlag(0));
        assertThrows(IllegalArgumentException.class, () -> record.hasTcpFlag(-1));
        assertThrows(IllegalArgumentException.class, () -> record.hasTcpFlag(256));
    }
}