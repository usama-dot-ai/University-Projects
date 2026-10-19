public class CacheLocalityBenchmark {

    private static final int ROWS = 2000;
    private static final int COLS = 2000;
    private static final int ITERATIONS = 10;

    // Row-Major: Contiguous Memory Access (Stride 1) -> Maximizes Cache Line hits
    public static double traverseRowMajor(double[][] matrix) {
        double sum = 0.0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                sum += matrix[i][j];
            }
        }
        return sum;
    }

    // Column-Major: Non-Contiguous Memory Access -> High Cache Miss frequency
    public static double traverseColumnMajor(double[][] matrix) {
        double sum = 0.0;
        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS; i++) {
                sum += matrix[i][j];
            }
        }
        return sum;
    }

    public static void main(String[] args) {
        double[][] matrix = new double[ROWS][COLS];

        // Initialize matrix
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                matrix[i][j] = 1.0;
            }
        }

        System.out.println("=== CPU Cache Line Spatial Locality Benchmark ===");
        System.out.println("Matrix Size: " + ROWS + " x " + COLS + " (4,000,000 double elements)");
        System.out.println("Benchmark Iterations: " + ITERATIONS + "\n");

        // Benchmark Row-Major Traversal
        long startRow = System.nanoTime();
        for (int k = 0; k < ITERATIONS; k++) {
            traverseRowMajor(matrix);
        }
        long endRow = System.nanoTime();
        double avgRowMs = ((endRow - startRow) / 1e6) / ITERATIONS;

        // Benchmark Column-Major Traversal
        long startCol = System.nanoTime();
        for (int k = 0; k < ITERATIONS; k++) {
            traverseColumnMajor(matrix);
        }
        long endCol = System.nanoTime();
        double avgColMs = ((endCol - startCol) / 1e6) / ITERATIONS;

        double speedup = avgColMs / avgRowMs;

        System.out.println("Results:");
        System.out.printf("  Average Row-Major Time:    %.3f ms\n", avgRowMs);
        System.out.printf("  Average Column-Major Time: %.3f ms\n", avgColMs);
        System.out.printf("  Row-Major Cache Speedup:   %.2fx faster\n\n", speedup);
    }
}
