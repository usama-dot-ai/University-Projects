import java.util.concurrent.RecursiveTask;

public class ParallelRiskCalculator extends RecursiveTask<Double> {
    private static final int THRESHOLD = 1000;
    private double[] assets;
    private int start;
    private int end;

    public ParallelRiskCalculator(double[] assets, int start, int end) {
        this.assets = assets;
        this.start = start;
        this.end = end;
    }

    @Override
    protected Double compute() {
        int length = end - start;
        if (length <= THRESHOLD) {
            // Sequential base case computation
            double sum = 0.0;
            for (int i = start; i < end; i++) {
                sum += assets[i];
            }
            return sum;
        } else {
            // Recursive divide-and-conquer
            int mid = start + (length / 2);
            ParallelRiskCalculator leftTask = new ParallelRiskCalculator(assets, start, mid);
            ParallelRiskCalculator rightTask = new ParallelRiskCalculator(assets, mid, end);

            leftTask.fork(); // Asynchronous execution
            double rightResult = rightTask.compute();
            double leftResult = leftTask.join(); // Await result

            return leftResult + rightResult;
        }
    }
}
