public class LogProcessorTask implements Runnable {
    @Override
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println("[" + Thread.currentThread().getName() + "] Batch Log Analysis Iteration: " + i + "/5");
            try {
                Thread.sleep(1000); // 1000ms delay
            } catch (InterruptedException e) {
                System.out.println(Thread.currentThread().getName() + " interrupted.");
            }
        }
