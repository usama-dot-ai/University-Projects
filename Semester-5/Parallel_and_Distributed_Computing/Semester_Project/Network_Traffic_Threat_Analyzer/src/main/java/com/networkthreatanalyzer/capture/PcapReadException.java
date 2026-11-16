package com.networkthreatanalyzer.capture;

import java.io.IOException;

/**
 * Signals an error validating, opening, or reading a classic PCAP capture file.
 */
public class PcapReadException extends IOException {

    /**
     * Constructs a {@code PcapReadException} with the specified detail message.
     *
     * @param message descriptive error message
     */
    public PcapReadException(String message) {
        super(message);
    }

    /**
     * Constructs a {@code PcapReadException} with the specified detail message and
     * cause.
     *
     * @param message descriptive error message
     * @param cause   underlying exception
     */
    public PcapReadException(String message, Throwable cause) {
        super(message, cause);
    }
}