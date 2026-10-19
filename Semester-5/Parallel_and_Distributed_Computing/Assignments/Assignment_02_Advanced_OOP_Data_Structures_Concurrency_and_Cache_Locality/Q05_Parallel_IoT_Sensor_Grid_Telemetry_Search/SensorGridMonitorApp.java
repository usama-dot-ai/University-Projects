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

        // Partition and fire 4 threads for Min and 4 threads for Max
        for (int i = 0; i < numPartitions; i++) {
            int start = i * partitionSize;
            int end = (i == numPartitions - 1) ? totalSensors : start + partitionSize;

            minTasks[i] = new ParallelMinSearch(telemetry, start, end);
            maxTasks[i] = new ParallelMaxSearch(telemetry, start, end);

            minThreads[i] = new Thread(minTasks[i]);
            maxThreads[i] = new Thread(maxTasks[i]);

            minThreads[i].start();
            maxThreads[i].start();
        }

        // Await all workers
        try {
            for (int i = 0; i < numPartitions; i++) {
                minThreads[i].join();
                maxThreads[i].join();
            }
        } catch (InterruptedException e) {
            System.out.println("Parallel processing interrupted.");
        }

        // Global Reduction
        double globalMin = Double.MAX_VALUE;
        double globalMax = -Double.MAX_VALUE;

        for (int i = 0; i < numPartitions; i++) {
            if (minTasks[i].getMinResult() < globalMin) {
                globalMin = minTasks[i].getMinResult();
            }
            if (maxTasks[i].getMaxResult() > globalMax) {
                globalMax = maxTasks[i].getMaxResult();
            }
        }
