
package com.networkthreatanalyzer.capture;

import com.networkthreatanalyzer.model.PacketRecord;
import com.networkthreatanalyzer.pipeline.PcapPacketSource;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Instant;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the sequential PCAP-to-PacketRecord adapter.
 *
 * <p>
 * Integration tests that open valid PCAP files require native Npcap
 * (WinPcap-compatible mode on Windows) or libpcap.
 */
class PcapPacketSourceTest {

    @TempDir
    Path tempDir;

    // ---------------------------------------------------------------------
    // Construction and lifecycle tests
    // ---------------------------------------------------------------------

    @Test
    void rejectsNullPathOnConstruction() {
        assertThrows(
                NullPointerException.class,
                () -> new PcapPacketSource(null));
    }

    @Test
    void enforcesLifecycleBeforeOpenAndAfterClose() {
        Path pcap = tempDir.resolve("lifecycle.pcap");

        PcapPacketSource source = new PcapPacketSource(pcap);

        // Reading before opening must fail.
        assertThrows(IllegalStateException.class, source::readNext);
        assertFalse(source.isOpen());
        assertFalse(source.isEof());

        // Closing before opening is safe.
        source.close();

        assertFalse(source.isOpen());
        assertThrows(IllegalStateException.class, source::readNext);

        // Repeated close is safe.
        source.close();

        // Reopening after close is not allowed.
        assertThrows(IllegalStateException.class, source::open);
    }

    @Test
    void rejectsOpeningAnInvalidCaptureFile() throws IOException {
        Path missingFile = tempDir.resolve("missing.pcap");

        PcapPacketSource source = new PcapPacketSource(missingFile);

        try (source) {
            assertThrows(PcapReadException.class, source::open);

            // A failed open leaves the source closed.
            assertFalse(source.isOpen());

            assertThrows(IllegalStateException.class, source::readNext);
        }
    }

    // ---------------------------------------------------------------------
    // Native-dependent integration tests
    // ---------------------------------------------------------------------

    @Test
    void readsAndParsesPacketsSequentially() throws IOException {
        Path pcap = tempDir.resolve("sequential-source.pcap");

        byte[] tcpPacket = PcapTestFixtures.buildEthernetIpv4TcpSynPacket();

        byte[] udpPacket = PcapTestFixtures.buildEthernetIpv4UdpPacket(
                new byte[] { 10, 20, 30, 40 });

        Instant firstTimestamp = Instant.parse("2026-03-10T10:15:30.123456Z");

        Instant secondTimestamp = Instant.parse("2026-03-10T10:15:31.654321Z");

        PcapTestFixtures.writeClassicPcap(
                pcap,
                ByteOrder.LITTLE_ENDIAN,
                false,
                List.of(
                        new PcapTestFixtures.RawPcapRecord(
                                firstTimestamp,
                                tcpPacket,
                                tcpPacket.length),
                        new PcapTestFixtures.RawPcapRecord(
                                secondTimestamp,
                                udpPacket,
                                udpPacket.length)));

        try (PcapPacketSource source = new PcapPacketSource(pcap)) {

            source.open();

            assertTrue(source.isOpen());
            assertFalse(source.isEof());

            // First packet
            Optional<PacketRecord> firstResult = source.readNext();

            assertTrue(firstResult.isPresent());

            PacketRecord first = firstResult.get();

            assertEquals(1L, first.sequenceNumber());
            assertEquals(firstTimestamp, first.timestamp());
            assertEquals(tcpPacket.length, first.capturedLength());
            assertEquals(tcpPacket.length, first.originalLength());
            assertNotNull(first.parseStatus());

            // Second packet
            Optional<PacketRecord> secondResult = source.readNext();

            assertTrue(secondResult.isPresent());

            PacketRecord second = secondResult.get();

            assertEquals(2L, second.sequenceNumber());
            assertEquals(secondTimestamp, second.timestamp());
            assertEquals(udpPacket.length, second.capturedLength());
            assertEquals(udpPacket.length, second.originalLength());
            assertNotNull(second.parseStatus());

            // EOF
            assertTrue(source.readNext().isEmpty());
            assertTrue(source.isEof());

            // Repeated reads after EOF remain empty.
            assertTrue(source.readNext().isEmpty());

            // Reader remains open until explicitly closed.
            assertTrue(source.isOpen());
        }
    }

    @Test
    void handlesEmptyCaptureAndRepeatedEofReads() throws IOException {
        Path emptyPcap = tempDir.resolve("empty-source.pcap");

        PcapTestFixtures.writeEmptyClassicPcap(emptyPcap);

        try (PcapPacketSource source = new PcapPacketSource(emptyPcap)) {

            source.open();

            assertTrue(source.isOpen());

            assertTrue(source.readNext().isEmpty());
            assertTrue(source.isEof());

            assertTrue(source.readNext().isEmpty());
            assertTrue(source.readNext().isEmpty());

            assertTrue(source.isOpen());
        }
    }

    @Test
    void propagatesCaptureReadErrors() throws IOException {
        Path corruptPcap = tempDir.resolve("corrupt-source.pcap");

        byte[] validPacket = PcapTestFixtures.buildEthernetIpv4TcpSynPacket();

        PcapTestFixtures.writeClassicPcap(
                corruptPcap,
                ByteOrder.LITTLE_ENDIAN,
                false,
                List.of(
                        new PcapTestFixtures.RawPcapRecord(
                                Instant.parse("2026-03-10T10:00:00Z"),
                                validPacket,
                                validPacket.length)));

        // Append a corrupt record header claiming 500 captured bytes,
        // followed by only 10 bytes of packet data.
        ByteBuffer corruptRecord = ByteBuffer.allocate(16 + 10).order(ByteOrder.LITTLE_ENDIAN);

        corruptRecord.putInt(1_773_000_000);
        corruptRecord.putInt(0);
        corruptRecord.putInt(500);
        corruptRecord.putInt(500);
        corruptRecord.put(new byte[10]);

        byte[] existing = Files.readAllBytes(corruptPcap);

        byte[] combined = new byte[existing.length + corruptRecord.capacity()];

        System.arraycopy(existing, 0, combined, 0, existing.length);

        System.arraycopy(
                corruptRecord.array(),
                0,
                combined,
                existing.length,
                corruptRecord.capacity());

        Files.write(corruptPcap, combined);

        try (PcapPacketSource source = new PcapPacketSource(corruptPcap)) {

            source.open();

            // The first valid packet should be parsed successfully.
            Optional<PacketRecord> first = source.readNext();

            assertTrue(first.isPresent());
            assertEquals(1L, first.get().sequenceNumber());

            // The malformed PCAP record must produce a read exception,
            // not be mistaken for EOF.
            assertThrows(PcapReadException.class, source::readNext);

            // The source must become closed after an unrecoverable read error.
            assertFalse(source.isOpen());

            assertThrows(IllegalStateException.class, source::readNext);
        }
    }
}
