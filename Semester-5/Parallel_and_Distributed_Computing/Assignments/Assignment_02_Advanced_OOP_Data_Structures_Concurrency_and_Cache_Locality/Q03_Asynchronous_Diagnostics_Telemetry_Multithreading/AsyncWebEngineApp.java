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
