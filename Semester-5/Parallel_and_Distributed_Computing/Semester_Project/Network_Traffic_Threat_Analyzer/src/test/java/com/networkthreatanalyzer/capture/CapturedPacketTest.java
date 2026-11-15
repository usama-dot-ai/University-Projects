package com.networkthreatanalyzer.capture;

import org.junit.jupiter.api.Test;
import org.pcap4j.packet.Packet;
import org.pcap4j.packet.UnknownPacket;
import org.pcap4j.packet.namednumber.DataLinkType;

import java.time.Instant;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Pure unit tests for {@link CapturedPacket}.
 *
 * <p>
 * Does not invoke {@code Pcaps} or require native Npcap/libpcap access.
 */
class CapturedPacketTest {

    private static final Instant SAMPLE_TIMESTAMP = Instant.parse("2026-02-01T12:00:00.123456Z");

    private static Packet dummyPacket(int length) {
        return new UnknownPacket.Builder()
                .rawData(new byte[length])
                .build();
    }

    @Test
    void constructsValidUntruncatedCapturedPacket() {
        Packet packet = dummyPacket(60);

        CapturedPacket captured = new CapturedPacket(
                1L,
                packet,
                SAMPLE_TIMESTAMP,
                60,
                60,
                DataLinkType.EN10MB);

        assertEquals(1L, captured.sequenceNumber());
        assertSame(packet, captured.packet());
        assertEquals(SAMPLE_TIMESTAMP, captured.timestamp());
        assertEquals(60, captured.capturedLength());
        assertEquals(60, captured.originalLength());
        assertEquals(DataLinkType.EN10MB, captured.dataLinkType());
        assertFalse(captured.isTruncated());
    }

    @Test
    void constructsValidTruncatedCapturedPacket() {
        Packet packet = dummyPacket(58);

        CapturedPacket captured = new CapturedPacket(
                2L,
                packet,
                SAMPLE_TIMESTAMP,
                58,
                1500,
                DataLinkType.EN10MB);

        assertEquals(58, captured.capturedLength());
        assertEquals(1500, captured.originalLength());
        assertTrue(captured.isTruncated());
    }

    @Test
    void rejectsNullRequiredArguments() {
        Packet packet = dummyPacket(20);

        assertThrows(NullPointerException.class,
                () -> new CapturedPacket(1L, null, SAMPLE_TIMESTAMP, 20, 20, DataLinkType.EN10MB));
        assertThrows(NullPointerException.class,
                () -> new CapturedPacket(1L, packet, null, 20, 20, DataLinkType.EN10MB));
        assertThrows(NullPointerException.class, () -> new CapturedPacket(1L, packet, SAMPLE_TIMESTAMP, 20, 20, null));
    }

    @Test
    void rejectsInvalidSequenceAndLengths() {
        Packet packet = dummyPacket(64);

        assertThrows(IllegalArgumentException.class,
                () -> new CapturedPacket(0L, packet, SAMPLE_TIMESTAMP, 64, 64, DataLinkType.EN10MB));
        assertThrows(IllegalArgumentException.class,
                () -> new CapturedPacket(-1L, packet, SAMPLE_TIMESTAMP, 64, 64, DataLinkType.EN10MB));
        assertThrows(IllegalArgumentException.class,
                () -> new CapturedPacket(1L, packet, SAMPLE_TIMESTAMP, -1, 64, DataLinkType.EN10MB));
        assertThrows(IllegalArgumentException.class,
                () -> new CapturedPacket(1L, packet, SAMPLE_TIMESTAMP, 64, -1, DataLinkType.EN10MB));
        assertThrows(IllegalArgumentException.class,
                () -> new CapturedPacket(1L, packet, SAMPLE_TIMESTAMP, 100, 64, DataLinkType.EN10MB));
    }
}