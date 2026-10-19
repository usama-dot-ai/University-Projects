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

        // Compare LIFO (reversed) vs FIFO (original order)
        while (!stack.isEmpty() && !queue.isEmpty()) {
            char fromStack = stack.pop();
            char fromQueue = queue.dequeue();
            if (fromStack != fromQueue) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        String[] testPackets = { "REVIVER", "TXN_104_01_NXT", "10011001", "SECURITY_GATEWAY" };

        System.out.println("=== Zero-Trust Packet Symmetry Audit Log ===");
        for (int i = 0; i < testPackets.length; i++) {
            String packet = testPackets[i];
            boolean isSymmetric = auditPacketSymmetry(packet);
            System.out.println("Payload: [" + packet + "] -> Symmetry Verified: " + isSymmetric);
        }
    }
}
