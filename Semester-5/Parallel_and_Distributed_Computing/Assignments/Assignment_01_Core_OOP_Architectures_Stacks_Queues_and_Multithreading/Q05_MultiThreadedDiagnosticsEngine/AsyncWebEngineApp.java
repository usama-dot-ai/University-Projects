public class AsyncWebEngineApp {
    public static void main(String[] args) {
        // 1. Output default main thread name assigned by the JVM
        System.out.println("Main Thread Name: " + Thread.currentThread().getName());

        // 2. Create threads using direct inheritance and Runnable interface
        TelemetryThread telemetryWorker = new TelemetryThread();
        telemetryWorker.setName("Telemetry-Worker");

        LogProcessorTask logTask = new LogProcessorTask();
        Thread logWorker = new Thread(logTask, "Log-Worker");

        System.out.println("Starting background diagnostic threads...");

        // 3. Start threads concurrently
        telemetryWorker.start();
        logWorker.start();

        // 4. Log active execution states
        System.out.println(telemetryWorker.getName() + " is alive: " + telemetryWorker.isAlive());
        System.out.println(logWorker.getName() + " is alive: " + logWorker.isAlive());
    }
}