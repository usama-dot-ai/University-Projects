public class TelemetryThread extends Thread {
    @Override
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println("[" + Thread.currentThread().getName() + "] System Metrics Capture (CPU/Mem) Iteration: "
                    + i + "/5");
