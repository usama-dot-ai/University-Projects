package com.networkthreatanalyzer.capture;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.pcap4j.packet.namednumber.DataLinkType;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Instant;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for {@link PcapFileReader}.
 *
 * <p>
 * Validation and pre-open lifecycle tests run without opening native handles.
 * Reader integration tests that open valid classic PCAP fixtures via
 * {@code Pcaps.openOffline} require native Npcap (WinPcap API-compatible mode
 * on Windows)
 * or libpcap.
 */
class PcapFileReaderTest {

    @TempDir
    Path tempDir;

    // -------------------------------------------------------------------------
    // Pure validation and pre-open lifecycle tests (no native handle opened)
    // -------------------------------------------------------------------------

    @Test
    void rejectsNullPathOnConstruction() {
        assertThrows(NullPointerException.class, () -> new PcapFileReader(null));
    }

    @Test
    void enforcesLifecycleBeforeOpenAndOnClose() {
        Path path = tempDir.resolve("not-opened.pcap");
        PcapFileReader reader = new PcapFileReader(path);

        assertEquals(PcapFileReader.State.NEW, reader.getState());
        assertFalse(reader.isOpen());
        assertThrows(IllegalStateException.class, reader::readNext);
        assertThrows(IllegalStateException.class, reader::getDataLinkType);

        reader.close();
        assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        assertFalse(reader.isOpen());

        // Idempotent close
        reader.close();
        assertEquals(PcapFileReader.State.CLOSED, reader.getState());

        // Reject reopening or reading after close
        assertThrows(IllegalStateException.class, reader::open);
        assertThrows(IllegalStateException.class, reader::readNext);
    }

    @Test
    void rejectsMissingDirectoryAndInvalidExtensionFiles() throws IOException {
        Path missing = tempDir.resolve("missing.pcap");
        try (PcapFileReader reader = new PcapFileReader(missing)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }

        Path directory = Files.createDirectory(tempDir.resolve("folder.pcap"));
        try (PcapFileReader reader = new PcapFileReader(directory)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }

        Path wrongExt = tempDir.resolve("capture.txt");
        Files.write(wrongExt, new byte[24]);
        try (PcapFileReader reader = new PcapFileReader(wrongExt)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }

        Path pcapngExt = tempDir.resolve("capture.pcapng");
        PcapTestFixtures.writePcapngHeader(pcapngExt);
        try (PcapFileReader reader = new PcapFileReader(pcapngExt)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }
    }

    @Test
    void rejectsPcapngDisguisedWithPcapExtensionAndInvalidMagicBytes() throws IOException {
        Path disguisedPcapng = tempDir.resolve("disguised.pcap");
        PcapTestFixtures.writePcapngHeader(disguisedPcapng);

        try (PcapFileReader reader = new PcapFileReader(disguisedPcapng)) {
            PcapReadException ex = assertThrows(PcapReadException.class, reader::open);
            assertTrue(ex.getMessage().contains("PCAPNG"));
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }

        Path invalidMagic = tempDir.resolve("invalid-magic.pcap");
        Files.write(invalidMagic, new byte[32]);
        try (PcapFileReader reader = new PcapFileReader(invalidMagic)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }

        Path tooSmallHeader = tempDir.resolve("short-header.pcap");
        ByteBuffer partialHeader = ByteBuffer.allocate(12).order(ByteOrder.LITTLE_ENDIAN);
        partialHeader.putInt(0xA1B2C3D4);
        Files.write(tooSmallHeader, partialHeader.array());
        try (PcapFileReader reader = new PcapFileReader(tooSmallHeader)) {
            assertThrows(PcapReadException.class, reader::open);
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
        }
    }

    // -------------------------------------------------------------------------
    // Native-dependent integration tests (require Npcap / libpcap)
    // -------------------------------------------------------------------------

    @Test
    void readsClassicPcapSequentiallyExtractsMetadataAndHandlesEof() throws IOException {
        Path pcap = tempDir.resolve("sequential.pcap");
        byte[] pkt1 = PcapTestFixtures.buildEthernetIpv4TcpSynPacket();
        byte[] pkt2 = PcapTestFixtures.buildEthernetIpv4UdpPacket(new byte[] { 10, 20, 30, 40, 50 });
        // Build a 74-byte Ethernet+IPv4+TCP packet with 20-byte payload, then truncate
        // to 58 bytes
        // (>= 54 bytes for full L2/L3/L4 headers) while recording original wire length
        // of 1500.
        byte[] fullTcpPayloadPacket = PcapTestFixtures.buildEthernetIpv4TcpPayloadPacket(new byte[20]);
        byte[] truncatedBytes = Arrays.copyOf(fullTcpPayloadPacket, 58);

        Instant t1 = Instant.parse("2026-02-10T08:00:01.123456Z");
        Instant t2 = Instant.parse("2026-02-10T08:00:02.654321Z");
        Instant t3 = Instant.parse("2026-02-10T08:00:03.000001Z");

        PcapTestFixtures.writeClassicPcap(
                pcap,
                ByteOrder.LITTLE_ENDIAN,
                false,
                List.of(
                        new PcapTestFixtures.RawPcapRecord(t1, pkt1, pkt1.length),
                        new PcapTestFixtures.RawPcapRecord(t2, pkt2, pkt2.length),
                        new PcapTestFixtures.RawPcapRecord(t3, truncatedBytes, 1500)));

        try (PcapFileReader reader = new PcapFileReader(pcap)) {
            reader.open();
            assertEquals(PcapFileReader.State.OPEN, reader.getState());
            assertTrue(reader.isOpen());
            assertEquals(DataLinkType.EN10MB, reader.getDataLinkType());

            // Reject double open
            assertThrows(IllegalStateException.class, reader::open);

            Optional<CapturedPacket> first = reader.readNext();
            assertTrue(first.isPresent());
            assertEquals(1L, first.get().sequenceNumber());
            assertEquals(t1, first.get().timestamp());
            assertEquals(pkt1.length, first.get().capturedLength());
            assertEquals(pkt1.length, first.get().originalLength());
            assertEquals(DataLinkType.EN10MB, first.get().dataLinkType());
            assertFalse(first.get().isTruncated());

            Optional<CapturedPacket> second = reader.readNext();
            assertTrue(second.isPresent());
            assertEquals(2L, second.get().sequenceNumber());
            assertEquals(t2, second.get().timestamp());
            assertEquals(pkt2.length, second.get().capturedLength());
            assertEquals(pkt2.length, second.get().originalLength());
            assertFalse(second.get().isTruncated());

            // Verify truncated packet length metadata without assuming complete protocol
            // decoding
            Optional<CapturedPacket> third = reader.readNext();
            assertTrue(third.isPresent());
            assertEquals(3L, third.get().sequenceNumber());
            assertEquals(t3, third.get().timestamp());
            assertNotNull(third.get().packet());
            assertEquals(58, third.get().capturedLength());
            assertEquals(58, third.get().packet().length());
            assertEquals(1500, third.get().originalLength());
            assertTrue(third.get().isTruncated());

            assertEquals(3L, reader.getPacketsRead());

            // EOF behavior: empty Optional and idempotent subsequent reads
            assertTrue(reader.readNext().isEmpty());
            assertEquals(PcapFileReader.State.EOF, reader.getState());
            assertTrue(reader.readNext().isEmpty());
            assertEquals(3L, reader.getPacketsRead());

            // Reject reopening in EOF state
            assertThrows(IllegalStateException.class, reader::open);

            reader.close();
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
            assertFalse(reader.isOpen());
            assertThrows(IllegalStateException.class, reader::readNext);
        }
    }

    @Test
    void readsEmptyClassicPcapAndReturnsEofImmediately() throws IOException {
        Path emptyCapture = tempDir.resolve("empty.pcap");
        PcapTestFixtures.writeEmptyClassicPcap(emptyCapture);

        try (PcapFileReader reader = new PcapFileReader(emptyCapture)) {
            reader.open();
            assertEquals(DataLinkType.EN10MB, reader.getDataLinkType());
            assertTrue(reader.readNext().isEmpty());
            assertEquals(PcapFileReader.State.EOF, reader.getState());
            assertEquals(0L, reader.getPacketsRead());
        }
    }

    @Test
    void readsBigEndianMicrosecondAndBothEndianNanosecondClassicPcapsWithFullTimestamps() throws IOException {
        byte[] pkt = PcapTestFixtures.buildEthernetIpv4TcpSynPacket();
        Instant microTs = Instant.parse("2026-02-11T09:15:30.123456Z");
        Instant nanoTs = Instant.parse("2026-02-11T09:15:30.123456789Z");

        Path bigEndianMicro = tempDir.resolve("big-endian-micro.pcap");
        PcapTestFixtures.writeClassicPcap(
                bigEndianMicro,
                ByteOrder.BIG_ENDIAN,
                false,
                List.of(new PcapTestFixtures.RawPcapRecord(microTs, pkt, pkt.length)));
        try (PcapFileReader reader = new PcapFileReader(bigEndianMicro)) {
            reader.open();
            Optional<CapturedPacket> read = reader.readNext();
            assertTrue(read.isPresent());
            assertEquals(microTs, read.get().timestamp());
            assertEquals(pkt.length, read.get().capturedLength());
        }

        Path littleEndianNano = tempDir.resolve("little-endian-nano.pcap");
        PcapTestFixtures.writeClassicPcap(
                littleEndianNano,
                ByteOrder.LITTLE_ENDIAN,
                true,
                List.of(new PcapTestFixtures.RawPcapRecord(nanoTs, pkt, pkt.length)));
        try (PcapFileReader reader = new PcapFileReader(littleEndianNano)) {
            reader.open();
            Optional<CapturedPacket> read = reader.readNext();
            assertTrue(read.isPresent());
            assertEquals(nanoTs, read.get().timestamp());
            assertEquals(pkt.length, read.get().capturedLength());
        }

        Path bigEndianNano = tempDir.resolve("big-endian-nano.pcap");
        PcapTestFixtures.writeClassicPcap(
                bigEndianNano,
                ByteOrder.BIG_ENDIAN,
                true,
                List.of(new PcapTestFixtures.RawPcapRecord(nanoTs, pkt, pkt.length)));
        try (PcapFileReader reader = new PcapFileReader(bigEndianNano)) {
            reader.open();
            Optional<CapturedPacket> read = reader.readNext();
            assertTrue(read.isPresent());
            assertEquals(nanoTs, read.get().timestamp());
            assertEquals(pkt.length, read.get().capturedLength());
        }
    }

    @Test
    void transitionsToClosedOnCorruptPacketRecordAndRemainsSafeToClose() throws IOException {
        Path corruptPcap = tempDir.resolve("corrupt-record.pcap");
        byte[] validPkt = PcapTestFixtures.buildEthernetIpv4TcpSynPacket();

        PcapTestFixtures.writeClassicPcap(
                corruptPcap,
                ByteOrder.LITTLE_ENDIAN,
                false,
                List.of(new PcapTestFixtures.RawPcapRecord(
                        Instant.parse("2026-02-12T00:00:00Z"), validPkt, validPkt.length)));

        // Append a 16-byte packet header claiming 500 captured bytes when only 10 bytes
        // follow.
        ByteBuffer trailingCorruptRecord = ByteBuffer.allocate(16 + 10).order(ByteOrder.LITTLE_ENDIAN);
        trailingCorruptRecord.putInt(1_770_000_000);
        trailingCorruptRecord.putInt(0);
        trailingCorruptRecord.putInt(500);
        trailingCorruptRecord.putInt(500);
        trailingCorruptRecord.put(new byte[10]);

        byte[] existing = Files.readAllBytes(corruptPcap);
        byte[] combined = new byte[existing.length + trailingCorruptRecord.capacity()];
        System.arraycopy(existing, 0, combined, 0, existing.length);
        System.arraycopy(trailingCorruptRecord.array(), 0, combined, existing.length, trailingCorruptRecord.capacity());
        Files.write(corruptPcap, combined);

        PcapFileReader reader = new PcapFileReader(corruptPcap);
        try (reader) {
            reader.open();
            assertTrue(reader.readNext().isPresent());
            assertThrows(PcapReadException.class, reader::readNext);

            // Unrecoverable read failure must immediately close handle and transition to
            // CLOSED
            assertEquals(PcapFileReader.State.CLOSED, reader.getState());
            assertFalse(reader.isOpen());
            assertThrows(IllegalStateException.class, reader::readNext);
        }
        // Repeated close via try-with-resources and explicit call remains idempotent
        reader.close();
        assertEquals(PcapFileReader.State.CLOSED, reader.getState());
    }
}