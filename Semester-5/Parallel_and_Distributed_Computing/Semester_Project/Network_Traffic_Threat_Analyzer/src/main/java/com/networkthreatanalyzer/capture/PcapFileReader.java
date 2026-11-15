package com.networkthreatanalyzer.capture;

import org.pcap4j.core.NotOpenException;
import org.pcap4j.core.PcapHandle;
import org.pcap4j.core.PcapHandle.TimestampPrecision;
import org.pcap4j.core.PcapNativeException;
import org.pcap4j.core.Pcaps;
import org.pcap4j.packet.Packet;
import org.pcap4j.packet.namednumber.DataLinkType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.sql.Timestamp;
import java.time.Instant;
import java.util.Locale;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.TimeoutException;

/**
 * Sequential offline reader for classic {@code .pcap} files backed by Pcap4J
 * 1.8.2.
 *
 * <p>
 * This class is not thread-safe and is intended to be owned by a single reader
 * thread,
 * matching Pcap4J's {@link PcapHandle} threading model where
 * {@link PcapHandle#getTimestamp()}
 * and {@link PcapHandle#getOriginalLength()} reflect the most recently read
 * packet.
 */
public final class PcapFileReader implements AutoCloseable {

    private static final Logger LOG = LoggerFactory.getLogger(PcapFileReader.class);

    private static final int CLASSIC_GLOBAL_HEADER_BYTES = 24;
    private static final int MAGIC_LENGTH_BYTES = 4;

    // Classic PCAP 4-byte magic sequences read in big-endian byte order:
    private static final int MAGIC_MICRO_BE = 0xA1B2C3D4;
    private static final int MAGIC_MICRO_LE = 0xD4C3B2A1;
    private static final int MAGIC_NANO_BE = 0xA1B23C4D;
    private static final int MAGIC_NANO_LE = 0x4D3CB2A1;

    // PCAPNG Section Header Block type (identical in both byte orders):
    private static final int MAGIC_PCAPNG = 0x0A0D0D0A;

    /**
     * Explicit reader lifecycle states.
     */
    public enum State {
        /** Reader constructed, native handle not yet opened. */
        NEW,
        /** Native handle opened and actively reading packets. */
        OPEN,
        /** End of capture file reached; handle remains open until {@link #close()}. */
        EOF,
        /**
         * Reader closed (or failed during initialization/read); cannot be reopened or
         * read.
         */
        CLOSED
    }

    private final Path filePath;

    private State state = State.NEW;
    private PcapHandle handle;
    private DataLinkType dataLinkType;
    private long packetsRead;

    /**
     * Creates a sequential reader for the given classic {@code .pcap} file path.
     *
     * @param filePath path to the {@code .pcap} file (non-null)
     */
    public PcapFileReader(Path filePath) {
        this.filePath = Objects.requireNonNull(filePath, "filePath must not be null");
    }

    /**
     * Validates the input path and classic PCAP magic header, detects timestamp
     * precision,
     * and opens the offline {@link PcapHandle}.
     *
     * <p>
     * May only be called once while in {@link State#NEW}. Any failure transitions
     * the
     * reader to {@link State#CLOSED} and closes any partially opened handle.
     *
     * @throws PcapReadException     if the file is missing, unreadable,
     *                               unsupported,
     *                               corrupt, or cannot be opened by Pcap4J
     * @throws IllegalStateException if the reader is not in {@link State#NEW}
     */
    public void open() throws PcapReadException {
        if (state != State.NEW) {
            throw new IllegalStateException("Cannot open PcapFileReader in state: " + state);
        }

        PcapHandle openedHandle = null;
        try {
            TimestampPrecision precision = validateAndDetectPrecision(filePath);
            openedHandle = Pcaps.openOffline(filePath.toString(), precision);

            DataLinkType dlt = openedHandle.getDlt();
            if (dlt == null) {
                throw new PcapReadException("PCAP file returned a null data-link type: " + filePath);
            }

            this.handle = openedHandle;
            this.dataLinkType = dlt;
            this.state = State.OPEN;
            LOG.info("Opened classic PCAP file '{}' (dlt={}, precision={})", filePath, dlt, precision);
        } catch (PcapNativeException e) {
            closeHandleQuietly(openedHandle);
            this.handle = null;
            this.state = State.CLOSED;
            throw new PcapReadException("Failed to open PCAP file with Pcap4J: " + filePath, e);
        } catch (PcapReadException | RuntimeException | Error e) {
            closeHandleQuietly(openedHandle);
            this.handle = null;
            this.state = State.CLOSED;
            throw e;
        }
    }

    /**
     * Reads the next packet sequentially from the capture file.
     *
     * <p>
     * On normal end-of-file, transitions to {@link State#EOF} and returns
     * {@link Optional#empty()}. On any unrecoverable read, metadata, or
     * packet-processing
     * failure, closes the native handle, transitions to {@link State#CLOSED}, and
     * throws
     * {@link PcapReadException}.
     *
     * @return {@link Optional} containing the next {@link CapturedPacket}, or
     *         {@link Optional#empty()} when end-of-file is reached
     * @throws PcapReadException     if reading fails or packet metadata is invalid
     * @throws IllegalStateException if called before {@link #open()} or after
     *                               {@link #close()}
     */
    public Optional<CapturedPacket> readNext() throws PcapReadException {
        if (state == State.NEW || state == State.CLOSED) {
            throw new IllegalStateException("Cannot read from PcapFileReader in state: " + state);
        }
        if (state == State.EOF) {
            return Optional.empty();
        }

        try {
            Packet packet = handle.getNextPacketEx();
            if (packet == null) {
                throw new PcapReadException(
                        "PcapHandle returned null packet before EOF at packet #" + (packetsRead + 1));
            }

            // Capture timestamp and original wire length immediately on the handle-owning
            // thread.
            Timestamp rawTimestamp = handle.getTimestamp();
            if (rawTimestamp == null) {
                throw new PcapReadException(
                        "PcapHandle returned null timestamp for packet #" + (packetsRead + 1));
            }
            Instant timestamp = rawTimestamp.toInstant();

            Integer originalLengthObj = handle.getOriginalLength();
            if (originalLengthObj == null) {
                throw new PcapReadException(
                        "PcapHandle returned null original packet length for packet #" + (packetsRead + 1));
            }
            int originalLength = originalLengthObj;
            int capturedLength = packet.length();

            if (capturedLength < 0 || originalLength < 0 || capturedLength > originalLength) {
                throw new PcapReadException(
                        "Invalid packet lengths at packet #" + (packetsRead + 1)
                                + ": capturedLength=" + capturedLength + ", originalLength=" + originalLength);
            }

            long sequenceNumber = packetsRead + 1;

            CapturedPacket capturedPacket = new CapturedPacket(
                    sequenceNumber,
                    packet,
                    timestamp,
                    capturedLength,
                    originalLength,
                    dataLinkType);

            packetsRead = sequenceNumber;
            return Optional.of(capturedPacket);
        } catch (EOFException e) {
            state = State.EOF;
            LOG.info("Reached EOF for PCAP file '{}' after reading {} packet(s)", filePath, packetsRead);
            return Optional.empty();
        } catch (PcapReadException e) {
            closeOnFailure();
            throw e;
        } catch (PcapNativeException | TimeoutException | NotOpenException | RuntimeException e) {
            closeOnFailure();
            throw new PcapReadException(
                    "Failed to read packet #" + (packetsRead + 1) + " from PCAP file: " + filePath, e);
        }
    }

    /**
     * Closes the underlying {@link PcapHandle} idempotently and transitions to
     * {@link State#CLOSED}.
     */
    @Override
    public void close() {
        if (state == State.CLOSED) {
            return;
        }
        try {
            closeHandleQuietly(handle);
        } finally {
            handle = null;
            state = State.CLOSED;
        }
    }

    /**
     * Returns the current lifecycle state of this reader.
     *
     * @return current {@link State}
     */
    public State getState() {
        return state;
    }

    /**
     * Returns {@code true} when the reader has an open handle ({@link State#OPEN}
     * or {@link State#EOF}).
     *
     * @return {@code true} if currently open
     */
    public boolean isOpen() {
        return state == State.OPEN || state == State.EOF;
    }

    /**
     * Returns the capture file's data-link type once opened.
     *
     * @return the {@link DataLinkType}
     * @throws IllegalStateException if the reader has not been opened
     */
    public DataLinkType getDataLinkType() {
        if (dataLinkType == null) {
            throw new IllegalStateException("DataLinkType is unavailable before open(); current state: " + state);
        }
        return dataLinkType;
    }

    /**
     * Returns the number of packets successfully read so far.
     *
     * @return non-negative packet count
     */
    public long getPacketsRead() {
        return packetsRead;
    }

    /**
     * Returns the path configured for this reader.
     *
     * @return capture file path
     */
    public Path getFilePath() {
        return filePath;
    }

    private void closeOnFailure() {
        try {
            closeHandleQuietly(handle);
        } finally {
            handle = null;
            state = State.CLOSED;
        }
    }

    private static TimestampPrecision validateAndDetectPrecision(Path path) throws PcapReadException {
        if (!Files.exists(path)) {
            throw new PcapReadException("PCAP file does not exist: " + path);
        }
        if (!Files.isRegularFile(path)) {
            throw new PcapReadException("PCAP path is not a regular file: " + path);
        }
        if (!Files.isReadable(path)) {
            throw new PcapReadException("PCAP file is not readable: " + path);
        }

        Path fileNamePath = path.getFileName();
        String fileName = fileNamePath != null ? fileNamePath.toString().toLowerCase(Locale.ROOT) : "";
        if (fileName.endsWith(".pcapng")) {
            throw new PcapReadException("PCAPNG files are not supported; expected classic .pcap file: " + path);
        }
        if (!fileName.endsWith(".pcap")) {
            throw new PcapReadException("Unsupported file extension (expected .pcap): " + path);
        }

        byte[] magicBytes = new byte[MAGIC_LENGTH_BYTES];
        try (InputStream in = Files.newInputStream(path)) {
            int totalRead = 0;
            while (totalRead < MAGIC_LENGTH_BYTES) {
                int read = in.read(magicBytes, totalRead, MAGIC_LENGTH_BYTES - totalRead);
                if (read == -1) {
                    throw new PcapReadException(
                            "File is too small to contain a valid 4-byte PCAP magic header: " + path);
                }
                totalRead += read;
            }
        } catch (IOException e) {
            if (e instanceof PcapReadException pre) {
                throw pre;
            }
            throw new PcapReadException("Failed to read PCAP header from file: " + path, e);
        }

        int magic = ByteBuffer.wrap(magicBytes).order(ByteOrder.BIG_ENDIAN).getInt();
        if (magic == MAGIC_PCAPNG) {
            throw new PcapReadException(
                    "PCAPNG format detected via magic bytes (0x0A0D0D0A); only classic .pcap is supported: " + path);
        }

        TimestampPrecision precision = resolvePrecisionFromMagic(magic);
        if (precision == null) {
            throw new PcapReadException(
                    String.format(Locale.ROOT,
                            "Invalid or unsupported PCAP magic header (0x%08X) in file: %s", magic, path));
        }

        try {
            long size = Files.size(path);
            if (size < CLASSIC_GLOBAL_HEADER_BYTES) {
                throw new PcapReadException(
                        "File is smaller than the 24-byte classic PCAP global header (" + size + " bytes): " + path);
            }
        } catch (IOException e) {
            if (e instanceof PcapReadException pre) {
                throw pre;
            }
            throw new PcapReadException("Failed to determine file size for PCAP file: " + path, e);
        }

        return precision;
    }

    private static TimestampPrecision resolvePrecisionFromMagic(int bigEndianMagic) {
        if (bigEndianMagic == MAGIC_MICRO_BE || bigEndianMagic == MAGIC_MICRO_LE) {
            return TimestampPrecision.MICRO;
        }
        if (bigEndianMagic == MAGIC_NANO_BE || bigEndianMagic == MAGIC_NANO_LE) {
            return TimestampPrecision.NANO;
        }
        return null;
    }

    private static void closeHandleQuietly(PcapHandle targetHandle) {
        if (targetHandle != null) {
            try {
                if (targetHandle.isOpen()) {
                    targetHandle.close();
                }
            } catch (RuntimeException e) {
                LOG.warn("Failed to close PcapHandle cleanly", e);
            }
        }
    }
}