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
