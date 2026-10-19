import java.util.concurrent.ForkJoinPool;

public class ForkJoinPortfolioApp {
    public static void main(String[] args) {
        int size = 10000;
        double[] assets = new double[size];

        for (int i = 0; i < size; i++) {
            assets[i] = 100.0 + (i % 500); // Synthetic asset values
        }

        ForkJoinPool pool = new ForkJoinPool();

        System.out.println("=== Financial Portfolio Fork/Join Evaluation ===");

        // 1. Compute Cumulative Risk Score
        long startTime = System.nanoTime();
        ParallelRiskCalculator riskTask = new ParallelRiskCalculator(assets, 0, size);
        double totalRisk = pool.invoke(riskTask);
        long endTime = System.nanoTime();

        System.out.println("Total Portfolio Risk Score: " + String.format("%.2f", totalRisk));
        System.out.println("Parallel Evaluation Time: " + (endTime - startTime) / 1e6 + " ms");

        // 2. Apply 5% Volatility Stress Test to assets exceeding riskCap = 450.0
        PortfolioThresholdTask stressTask = new PortfolioThresholdTask(assets, 0, size, 450.0);
        pool.invoke(stressTask);
        System.out.println("In-place 5% Volatility Stress Test completed successfully.");

        // Re-evaluate post-stress risk
        double postStressRisk = pool.invoke(new ParallelRiskCalculator(assets, 0, size));
        System.out.println("Post-Stress Cumulative Risk Score: " + String.format("%.2f", postStressRisk));
    }
}
