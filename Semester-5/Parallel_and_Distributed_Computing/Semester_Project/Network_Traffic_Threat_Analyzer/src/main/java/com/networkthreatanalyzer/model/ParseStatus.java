package com.networkthreatanalyzer.model;

/**
 * Indicates how completely a captured packet was decoded into a
 * {@link PacketRecord}.
 */
public enum ParseStatus {
    /** All expected headers present in the packet were parsed without error. */
    SUCCESS,
    /**
     * Outer layers were parsed, but inner layers were unavailable (e.g., snaplen
     * truncation or non-initial fragment).
     */
    PARTIAL,
    /** Packet headers were corrupt, inconsistent, or failed decoding. */
    MALFORMED
}