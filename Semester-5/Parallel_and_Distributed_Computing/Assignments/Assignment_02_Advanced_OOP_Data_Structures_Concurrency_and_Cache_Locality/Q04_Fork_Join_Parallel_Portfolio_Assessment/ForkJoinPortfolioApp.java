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
