public class AsyncWebEngineApp {
    public static void main(String[] args) {
        // Output default JVM entry thread name
        System.out.println("Active Main Entry Thread: " + Thread.currentThread().getName());

        // 1. Thread extending Thread class
        TelemetryThread telemetryWorker = new TelemetryThread();
        telemetryWorker.setName("Telemetry-Worker");

        // 2. Thread implementing Runnable
        LogProcessorTask logTask = new LogProcessorTask();
        Thread logWorker = new Thread(logTask, "Log-Worker");

        System.out.println("Spawning asynchronous workers at 50ms...");
        telemetryWorker.start();
        logWorker.start();

        // Telemetry tracking active thread states
        System.out.println("Thread Status -> " + telemetryWorker.getName() + " isAlive: " + telemetryWorker.isAlive());
        System.out.println("Thread Status -> " + logWorker.getName() + " isAlive: " + logWorker.isAlive());

        try {
            // Main waits for clean worker shutdown
            telemetryWorker.join();
            logWorker.join();
        } catch (InterruptedException e) {
            System.out.println("Main thread interrupted.");
        }

        System.out.println("All background diagnostics terminated cleanly. Main exiting.");
    }
}
