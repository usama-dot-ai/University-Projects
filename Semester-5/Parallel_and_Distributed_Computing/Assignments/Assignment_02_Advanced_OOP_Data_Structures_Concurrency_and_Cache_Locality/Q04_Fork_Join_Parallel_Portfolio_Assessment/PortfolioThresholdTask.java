import java.util.concurrent.RecursiveAction;

public class PortfolioThresholdTask extends RecursiveAction {
    private static final int THRESHOLD = 1000;
    private double[] assets;
    private int start;
    private int end;
    private double riskCap;

    public PortfolioThresholdTask(double[] assets, int start, int end, double riskCap) {
        this.assets = assets;
        this.start = start;
        this.end = end;
        this.riskCap = riskCap;
    }

    @Override
    protected void compute() {
        int length = end - start;
        if (length <= THRESHOLD) {
            // In-place 5% penalty adjustment on values exceeding riskCap
            for (int i = start; i < end; i++) {
                if (assets[i] > riskCap) {
                    assets[i] += assets[i] * 0.05;
                }
            }
        } else {
            int mid = start + (length / 2);
            PortfolioThresholdTask left = new PortfolioThresholdTask(assets, start, mid, riskCap);
            PortfolioThresholdTask right = new PortfolioThresholdTask(assets, mid, end, riskCap);

            left.fork();
            right.compute();
            left.join();
        }
    }
}
