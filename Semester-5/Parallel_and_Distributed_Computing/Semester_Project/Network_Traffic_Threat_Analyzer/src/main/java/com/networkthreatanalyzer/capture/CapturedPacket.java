package com.networkthreatanalyzer.capture;

import org.pcap4j.packet.Packet;
import org.pcap4j.packet.namednumber.DataLinkType;

import java.time.Instant;
import java.util.Objects;

/**
 * Capture-layer container holding a single raw Pcap4J {@link Packet} and the
 * capture metadata obtained immediately from {@code PcapHandle} on the reader
 * thread.
 *
 * @param sequenceNumber 1-based sequential index within the capture file (&gt;=
 *                       1)
 * @param packet         raw Pcap4J packet object (non-null)
 * @param timestamp      capture timestamp converted to {@link Instant}
 *                       (non-null)
 * @param capturedLength number of bytes stored in the capture file (&gt;= 0,
 *                       &lt;= {@code originalLength})
 * @param originalLength original packet length on the wire (&gt;= 0)
 * @param dataLinkType   capture file data-link type (non-null)
 */
public record CapturedPacket(
        long sequenceNumber,
        Packet packet,
        Instant timestamp,
        int capturedLength,
        int originalLength,
        DataLinkType dataLinkType) {

    /**
     * Validates capture-layer invariants on construction.
     */
    public CapturedPacket {
        if (sequenceNumber < 1) {
            throw new IllegalArgumentException("sequenceNumber must be >= 1, got: " + sequenceNumber);
        }
        Objects.requireNonNull(packet, "packet must not be null");
        Objects.requireNonNull(timestamp, "timestamp must not be null");
        if (capturedLength < 0) {
            throw new IllegalArgumentException("capturedLength must be >= 0, got: " + capturedLength);
        }
        if (originalLength < 0) {
            throw new IllegalArgumentException("originalLength must be >= 0, got: " + originalLength);
        }
        if (capturedLength > originalLength) {
            throw new IllegalArgumentException(
                    "capturedLength (" + capturedLength + ") must not exceed originalLength (" + originalLength + ")");
        }
        Objects.requireNonNull(dataLinkType, "dataLinkType must not be null");
    }

    /**
     * Returns {@code true} when the captured packet was truncated relative to its
     * wire length.
     *
     * @return {@code true} if {@code capturedLength < originalLength}
     */
    public boolean isTruncated() {
        return capturedLength < originalLength;
    }
}