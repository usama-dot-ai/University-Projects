package com.networkthreatanalyzer.model;

import java.net.InetAddress;
import java.time.Instant;
import java.util.Objects;

/**
 * Immutable, standardized representation of a single captured and parsed
 * network packet.
 *
 * <p>
 * Unavailable address fields ({@code sourceMac}, {@code destinationMac},
 * {@code sourceAddress}, {@code destinationAddress}) are represented as
 * {@code null}.
 * Unavailable numeric protocol fields ({@code hopLimit}, {@code sourcePort},
 * {@code destinationPort}, {@code tcpFlags}, {@code icmpType},
 * {@code icmpCode},
 * {@code payloadLength}) are represented using {@link #UNAVAILABLE}
 * ({@code -1}).
 *
 * <p>
 * <strong>Payload length contract:</strong> {@code payloadLength} represents
 * the number
 * of bytes immediately following the decoded transport header when
 * determinable:
 * <ul>
 * <li>TCP: bytes following the TCP header.</li>
 * <li>UDP: bytes following the UDP header.</li>
 * <li>ICMPv4 / ICMPv6: bytes following the ICMP header.</li>
 * <li>ARP or packets without a transport header: {@code -1}.</li>
 * <li>Truncated or undecodable transport headers: {@code -1} when the payload
 * length
 * cannot be reliably determined.</li>
 * </ul>
 *
 * @param sequenceNumber     1-based monotonic packet index within the capture
 *                           source (&gt;= 1)
 * @param timestamp          capture timestamp from the packet header (non-null)
 * @param capturedLength     number of bytes captured in the trace (&gt;= 0,
 *                           &lt;= {@code originalLength})
 * @param originalLength     original packet length on the wire (&gt;= 0)
 * @param sourceMac          source MAC address if available, or {@code null}
 * @param destinationMac     destination MAC address if available, or
 *                           {@code null}
 * @param networkProtocol    identified network-layer protocol (non-null)
 * @param sourceAddress      source IP address if available, or {@code null}
 * @param destinationAddress destination IP address if available, or
 *                           {@code null}
 * @param hopLimit           IPv4 TTL or IPv6 Hop Limit (0–255), or {@code -1}
 *                           if unavailable
 * @param fragmented         {@code true} if the IP packet is fragmented
 * @param transportProtocol  identified transport-layer protocol (non-null)
 * @param sourcePort         transport source port (0–65535), or {@code -1} if
 *                           unavailable
 * @param destinationPort    transport destination port (0–65535), or {@code -1}
 *                           if unavailable
 * @param tcpFlags           8-bit TCP flags bitmask (0–255), or {@code -1} if
 *                           unavailable
 * @param icmpType           ICMPv4/ICMPv6 type field (0–255), or {@code -1} if
 *                           unavailable
 * @param icmpCode           ICMPv4/ICMPv6 code field (0–255), or {@code -1} if
 *                           unavailable
 * @param payloadLength      bytes following the decoded transport header (&gt;=
 *                           0), or {@code -1}
 *                           if no transport header is present or length cannot
 *                           be reliably determined
 * @param parseStatus        parsing completeness/validity status (non-null)
 */
public record PacketRecord(
        long sequenceNumber,
        Instant timestamp,
        int capturedLength,
        int originalLength,
        String sourceMac,
        String destinationMac,
        NetworkProtocol networkProtocol,
        InetAddress sourceAddress,
        InetAddress destinationAddress,
        int hopLimit,
        boolean fragmented,
        TransportProtocol transportProtocol,
        int sourcePort,
        int destinationPort,
        int tcpFlags,
        int icmpType,
        int icmpCode,
        int payloadLength,
        ParseStatus parseStatus) {

    /** Sentinel value indicating an unavailable numeric field. */
    public static final int UNAVAILABLE = -1;

    /** Minimum valid TCP/UDP port number. */
    public static final int MIN_PORT = 0;

    /** Maximum valid TCP/UDP port number. */
    public static final int MAX_PORT = 65_535;

    /**
     * Maximum valid 8-bit unsigned value (hop limit, ICMP type, ICMP code, TCP
     * flags octet).
     */
    public static final int MAX_UINT8 = 255;

    /** TCP FIN flag bit (bit 0). */
    public static final int TCP_FIN = 0x01;

    /** TCP SYN flag bit (bit 1). */
    public static final int TCP_SYN = 0x02;

    /** TCP RST flag bit (bit 2). */
    public static final int TCP_RST = 0x04;

    /** TCP PSH flag bit (bit 3). */
    public static final int TCP_PSH = 0x08;

    /** TCP ACK flag bit (bit 4). */
    public static final int TCP_ACK = 0x10;

    /** TCP URG flag bit (bit 5). */
    public static final int TCP_URG = 0x20;

    /** TCP ECE (ECN-Echo) flag bit (bit 6). */
    public static final int TCP_ECE = 0x40;

    /** TCP CWR (Congestion Window Reduced) flag bit (bit 7). */
    public static final int TCP_CWR = 0x80;

    /**
     * Compact constructor validating required references and per-field numeric
     * invariants.
     */
    public PacketRecord {
        if (sequenceNumber < 1) {
            throw new IllegalArgumentException("sequenceNumber must be >= 1, got: " + sequenceNumber);
        }
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

        validateOptionalMac(sourceMac, "sourceMac");
        validateOptionalMac(destinationMac, "destinationMac");

        Objects.requireNonNull(networkProtocol, "networkProtocol must not be null");
        validateUint8OrUnavailable(hopLimit, "hopLimit");
        Objects.requireNonNull(transportProtocol, "transportProtocol must not be null");
        validatePortOrUnavailable(sourcePort, "sourcePort");
        validatePortOrUnavailable(destinationPort, "destinationPort");
        validateUint8OrUnavailable(tcpFlags, "tcpFlags");
        validateUint8OrUnavailable(icmpType, "icmpType");
        validateUint8OrUnavailable(icmpCode, "icmpCode");

        // Payload length is >= 0 when determinable after a decoded transport header, or
        // -1 (UNAVAILABLE)
        // when no transport header exists or truncation/malformation prevents reliable
        // length calculation.
        if (payloadLength < UNAVAILABLE) {
            throw new IllegalArgumentException(
                    "payloadLength must be >= 0 or -1 (unavailable), got: " + payloadLength);
        }

        Objects.requireNonNull(parseStatus, "parseStatus must not be null");
    }

    /**
     * Checks safely whether all bits in {@code flagMask} are set in
     * {@link #tcpFlags()}.
     *
     * <p>
     * Use this method instead of raw bitwise masking on {@link #tcpFlags()} so that
     * {@link #UNAVAILABLE} ({@code -1}) never evaluates as matching TCP flags.
     *
     * @param flagMask one or more combined {@code TCP_*} bit constants (1–255)
     * @return {@code true} if {@code tcpFlags} is available and all bits in
     *         {@code flagMask} are set
     */
    public boolean hasTcpFlag(int flagMask) {
        if (flagMask <= 0 || flagMask > MAX_UINT8) {
            throw new IllegalArgumentException("flagMask must be between 1 and 255, got: " + flagMask);
        }
        return tcpFlags != UNAVAILABLE && (tcpFlags & flagMask) == flagMask;
    }

    /**
     * Returns {@code true} if the packet was truncated during capture
     * ({@code capturedLength < originalLength}).
     *
     * @return {@code true} if capture length is smaller than wire length
     */
    public boolean isTruncated() {
        return capturedLength < originalLength;
    }

    /**
     * Creates a new {@link Builder} initialized with {@link #UNAVAILABLE}
     * ({@code -1})
     * for optional numeric fields and {@code false} for {@code fragmented}.
     *
     * @return a new builder instance
     */
    public static Builder builder() {
        return new Builder();
    }

    private static void validateOptionalMac(String mac, String fieldName) {
        if (mac != null && mac.isBlank()) {
            throw new IllegalArgumentException(fieldName + " must not be blank; use null when unavailable");
        }
    }

    private static void validatePortOrUnavailable(int value, String fieldName) {
        if (value != UNAVAILABLE && (value < MIN_PORT || value > MAX_PORT)) {
            throw new IllegalArgumentException(
                    fieldName + " must be in range 0..65535 or -1 (unavailable), got: " + value);
        }
    }

    private static void validateUint8OrUnavailable(int value, String fieldName) {
        if (value != UNAVAILABLE && (value < 0 || value > MAX_UINT8)) {
            throw new IllegalArgumentException(
                    fieldName + " must be in range 0..255 or -1 (unavailable), got: " + value);
        }
    }

    /**
     * Fluent builder for constructing {@link PacketRecord} instances safely without
     * positional parameter mistakes.
     */
    public static final class Builder {
        private long sequenceNumber;
        private Instant timestamp;
        private int capturedLength;
        private int originalLength;
        private String sourceMac;
        private String destinationMac;
        private NetworkProtocol networkProtocol;
        private InetAddress sourceAddress;
        private InetAddress destinationAddress;
        private int hopLimit = UNAVAILABLE;
        private boolean fragmented;
        private TransportProtocol transportProtocol;
        private int sourcePort = UNAVAILABLE;
        private int destinationPort = UNAVAILABLE;
        private int tcpFlags = UNAVAILABLE;
        private int icmpType = UNAVAILABLE;
        private int icmpCode = UNAVAILABLE;
        private int payloadLength = UNAVAILABLE;
        private ParseStatus parseStatus;

        private Builder() {
        }

        public Builder sequenceNumber(long sequenceNumber) {
            this.sequenceNumber = sequenceNumber;
            return this;
        }

        public Builder timestamp(Instant timestamp) {
            this.timestamp = timestamp;
            return this;
        }

        public Builder capturedLength(int capturedLength) {
            this.capturedLength = capturedLength;
            return this;
        }

        public Builder originalLength(int originalLength) {
            this.originalLength = originalLength;
            return this;
        }

        public Builder sourceMac(String sourceMac) {
            this.sourceMac = sourceMac;
            return this;
        }

        public Builder destinationMac(String destinationMac) {
            this.destinationMac = destinationMac;
            return this;
        }

        public Builder networkProtocol(NetworkProtocol networkProtocol) {
            this.networkProtocol = networkProtocol;
            return this;
        }

        public Builder sourceAddress(InetAddress sourceAddress) {
            this.sourceAddress = sourceAddress;
            return this;
        }

        public Builder destinationAddress(InetAddress destinationAddress) {
            this.destinationAddress = destinationAddress;
            return this;
        }

        public Builder hopLimit(int hopLimit) {
            this.hopLimit = hopLimit;
            return this;
        }

        public Builder fragmented(boolean fragmented) {
            this.fragmented = fragmented;
            return this;
        }

        public Builder transportProtocol(TransportProtocol transportProtocol) {
            this.transportProtocol = transportProtocol;
            return this;
        }

        public Builder sourcePort(int sourcePort) {
            this.sourcePort = sourcePort;
            return this;
        }

        public Builder destinationPort(int destinationPort) {
            this.destinationPort = destinationPort;
            return this;
        }

        public Builder tcpFlags(int tcpFlags) {
            this.tcpFlags = tcpFlags;
            return this;
        }

        public Builder icmpType(int icmpType) {
            this.icmpType = icmpType;
            return this;
        }

        public Builder icmpCode(int icmpCode) {
            this.icmpCode = icmpCode;
            return this;
        }

        /**
         * Sets the number of bytes following the decoded transport header, or
         * {@link PacketRecord#UNAVAILABLE} ({@code -1}) when unavailable or
         * undeterminable.
         *
         * @param payloadLength transport payload length in bytes, or {@code -1}
         * @return this builder
         */
        public Builder payloadLength(int payloadLength) {
            this.payloadLength = payloadLength;
            return this;
        }

        public Builder parseStatus(ParseStatus parseStatus) {
            this.parseStatus = parseStatus;
            return this;
        }

        public PacketRecord build() {
            return new PacketRecord(
                    sequenceNumber,
                    timestamp,
                    capturedLength,
                    originalLength,
                    sourceMac,
                    destinationMac,
                    networkProtocol,
                    sourceAddress,
                    destinationAddress,
                    hopLimit,
                    fragmented,
                    transportProtocol,
                    sourcePort,
                    destinationPort,
                    tcpFlags,
                    icmpType,
                    icmpCode,
                    payloadLength,
                    parseStatus);
        }
    }
}