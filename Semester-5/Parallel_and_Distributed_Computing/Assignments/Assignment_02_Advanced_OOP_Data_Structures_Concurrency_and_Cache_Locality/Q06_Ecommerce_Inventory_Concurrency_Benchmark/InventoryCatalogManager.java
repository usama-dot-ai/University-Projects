import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class InventoryCatalogManager {

    private static final int THREAD_COUNT = 10;
    private static final int OPERATIONS_PER_THREAD = 50000;
    private static final String PRODUCT_KEY = "PROD_LAPTOP_01";

    public void benchmarkSynchronizedMap() {
        // Coarse-grained: Entire map locked under single mutex
        Map<String, Integer> syncMap = Collections.synchronizedMap(new HashMap<String, Integer>());
        syncMap.put(PRODUCT_KEY, 1000000);

        Thread[] workers = new Thread[THREAD_COUNT];
        long startTime = System.currentTimeMillis();

        for (int i = 0; i < THREAD_COUNT; i++) {
            workers[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int j = 0; j < OPERATIONS_PER_THREAD; j++) {
                        Integer stock = syncMap.get(PRODUCT_KEY);
                        syncMap.put(PRODUCT_KEY, stock != null ? stock - 1 : 0);
                    }
                }
            });
            workers[i].start();
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            try {
                workers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long duration = System.currentTimeMillis() - startTime;
        long totalOps = (long) THREAD_COUNT * OPERATIONS_PER_THREAD;
        double throughput = (totalOps / (double) duration) * 1000;

        System.out.println("Collections.synchronizedMap (Coarse-Grained Locking):");
        System.out.println("  Time Taken: " + duration + " ms");
        System.out.println("  Throughput: " + String.format("%.2f", throughput) + " ops/sec\n");
    }

    public void benchmarkConcurrentMap() {
        // Fine-grained: Segment / bucket-level locking
        ConcurrentHashMap<String, Integer> concurrentMap = new ConcurrentHashMap<String, Integer>();
        concurrentMap.put(PRODUCT_KEY, 1000000);

        Thread[] workers = new Thread[THREAD_COUNT];
        long startTime = System.currentTimeMillis();

        for (int i = 0; i < THREAD_COUNT; i++) {
            workers[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int j = 0; j < OPERATIONS_PER_THREAD; j++) {
                        Integer stock = concurrentMap.get(PRODUCT_KEY);
                        concurrentMap.put(PRODUCT_KEY, stock != null ? stock - 1 : 0);
                    }
                }
            });
            workers[i].start();
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            try {
                workers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long duration = System.currentTimeMillis() - startTime;
        long totalOps = (long) THREAD_COUNT * OPERATIONS_PER_THREAD;
        double throughput = (totalOps / (double) duration) * 1000;

        System.out.println("ConcurrentHashMap (Fine-Grained Bucket Locking):");
        System.out.println("  Time Taken: " + duration + " ms");
        System.out.println("  Throughput: " + String.format("%.2f", throughput) + " ops/sec\n");
    }
