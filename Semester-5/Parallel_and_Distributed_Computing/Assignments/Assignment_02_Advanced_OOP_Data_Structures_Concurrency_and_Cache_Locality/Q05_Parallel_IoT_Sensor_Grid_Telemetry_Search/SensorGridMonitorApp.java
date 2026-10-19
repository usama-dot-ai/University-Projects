public class SensorGridMonitorApp {
    public static void main(String[] args) {
        int totalSensors = 50000;
        double[] telemetry = new double[totalSensors];

        // Seed synthetic telemetry data
        for (int i = 0; i < totalSensors; i++) {
            telemetry[i] = 20.0 + (i % 150) * 0.5; // Range: 20.0 to 94.5
        }
        telemetry[12500] = -12.4; // Extreme min temperature anomaly
        telemetry[37200] = 198.8; // Peak vibration spike anomaly

        int numPartitions = 4;
        int partitionSize = totalSensors / numPartitions;

        ParallelMinSearch[] minTasks = new ParallelMinSearch[numPartitions];
        ParallelMaxSearch[] maxTasks = new ParallelMaxSearch[numPartitions];
        Thread[] minThreads = new Thread[numPartitions];
        Thread[] maxThreads = new Thread[numPartitions];

        long startTime = System.currentTimeMillis();
