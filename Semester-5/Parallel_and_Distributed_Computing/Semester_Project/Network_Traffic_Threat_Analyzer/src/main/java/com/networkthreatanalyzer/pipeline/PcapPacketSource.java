
package com.networkthreatanalyzer.pipeline;

import com.networkthreatanalyzer.capture.CapturedPacket;
import com.networkthreatanalyzer.capture.PcapFileReader;
import com.networkthreatanalyzer.capture.PcapReadException;
import com.networkthreatanalyzer.model.PacketRecord;
import com.networkthreatanalyzer.parser.PacketParser;

import java.nio.file.Path;
import java.util.Objects;
import java.util.Optional;

/**
 * Sequential adapter that reads captured packets from a PCAP file
 * and converts them into standardized PacketRecord objects.
 *
 * <p>
 * This class performs no buffering, multithreading, or parallel
 * processing. It is intended to provide a sequential packet source
 * for a future producer-consumer pipeline.
 */
public final class PcapPacketSource implements AutoCloseable {

    private final PcapFileReader reader;
    private final PacketParser parser;

    private State state = State.NEW;

    private enum State {
        NEW,
        OPEN,
        EOF,
        CLOSED
    }

    /**
     * Creates a packet source for the specified PCAP file.
     *
     * @param filePath path to the PCAP file
     */
    public PcapPacketSource(Path filePath) {
        Objects.requireNonNull(filePath, "filePath must not be null");

        this.reader = new PcapFileReader(filePath);
        this.parser = new PacketParser();
    }

    /**
     * Opens the underlying PCAP reader.
     *
     * @throws PcapReadException     if the capture file cannot be opened
     * @throws IllegalStateException if this source is not in NEW state
     */
    public void open() throws PcapReadException {
        if (state != State.NEW) {
            throw new IllegalStateException(
                    "Cannot open PcapPacketSource in state: " + state);
        }

        try {
            reader.open();
            state = State.OPEN;
        } catch (PcapReadException | RuntimeException exception) {
            state = State.CLOSED;
            throw exception;
        }
    }

    /**
     * Reads and parses the next packet sequentially.
     *
     * @return the next parsed PacketRecord, or Optional.empty() at EOF
     * @throws PcapReadException     if reading the capture fails
     * @throws IllegalStateException if called before open or after close
     */
    public Optional<PacketRecord> readNext() throws PcapReadException {
        if (state == State.NEW || state == State.CLOSED) {
            throw new IllegalStateException(
                    "Cannot read from PcapPacketSource in state: " + state);
        }

        if (state == State.EOF) {
            return Optional.empty();
        }

        Optional<CapturedPacket> capturedPacket;

        try {
            capturedPacket = reader.readNext();
        } catch (PcapReadException exception) {
            state = State.CLOSED;
            throw exception;
        }

        if (capturedPacket.isEmpty()) {
            state = State.EOF;
            return Optional.empty();
        }

        try {
            PacketRecord record = parser.parse(capturedPacket.get());
            return Optional.of(record);
        } catch (RuntimeException exception) {
            close();
            throw exception;
        }
    }

    /**
     * Returns whether the source has reached EOF.
     *
     * @return true if EOF has been reached
     */
    public boolean isEof() {
        return state == State.EOF;
    }

    /**
     * Returns whether the source is currently open for reading.
     *
     * <p>
     * Returns true in OPEN and EOF states because the underlying
     * reader remains open until close() is called.
     *
     * @return true if the underlying reader has been opened and not closed
     */
    public boolean isOpen() {
        return state == State.OPEN || state == State.EOF;
    }

    /**
     * Closes the underlying PCAP reader.
     *
     * <p>
     * This operation is idempotent.
     */
    @Override
    public void close() {
        if (state == State.CLOSED) {
            return;
        }

        try {
            reader.close();
        } finally {
            state = State.CLOSED;
        }
    }
}
