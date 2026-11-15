package com.networkthreatanalyzer.model;

/**
 * Network-layer (or equivalent Layer 2.5/3) protocol identified for a captured
 * packet.
 */
public enum NetworkProtocol {
    /** Internet Protocol version 4 (IPv4). */
    IPV4,
    /** Internet Protocol version 6 (IPv6). */
    IPV6,
    /** Address Resolution Protocol (ARP). */
    ARP,
    /** Recognized non-IP/non-ARP protocol (e.g., LLDP, STP, IPX). */
    OTHER,
    /**
     * Network protocol could not be determined due to truncation, corruption, or
     * unknown encapsulation.
     */
    UNKNOWN
}