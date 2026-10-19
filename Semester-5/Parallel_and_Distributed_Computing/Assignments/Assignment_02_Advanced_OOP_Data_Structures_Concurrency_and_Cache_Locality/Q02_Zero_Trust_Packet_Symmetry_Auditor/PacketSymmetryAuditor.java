public class PacketSymmetryAuditor {

    public static boolean auditPacketSymmetry(String payload) {
        int length = payload.length();
        TransactionStack stack = new TransactionStack(length);
        RequestQueue queue = new RequestQueue(length);

        // Load characters into LIFO stack and FIFO queue
        for (int i = 0; i < length; i++) {
            char c = payload.charAt(i);
            stack.push(c);
            queue.enqueue(c);
        }
