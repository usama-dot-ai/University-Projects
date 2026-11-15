package com.networkthreatanalyzer.model;

/**
 * Transport-layer (or encapsulated Layer 4) protocol identified for a captured
 * packet.
 */
public enum TransportProtocol {
    /** Transmission Control Protocol (TCP). */
    TCP,
    /** User Datagram Protocol (UDP). */
    UDP,
    /** Internet Control Message Protocol for IPv4 (ICMPv4). */
    ICMPV4,
    /** Internet Control Message Protocol for IPv6 (ICMPv6). */
    ICMPV6,
    /**
     * Recognized IP protocol other than TCP, UDP, or ICMP (e.g., GRE, ESP, AH,
     * SCTP).
     */
    OTHER,
    /**
     * No transport layer present or transport layer could not be decoded (e.g.,
     * ARP, non-IP, or truncated L3).
     */
    NONE
}