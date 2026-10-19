public class ParallelMinSearch extends SensorSearchEngine implements Runnable {
    private double minResult;

    public ParallelMinSearch(double[] sensorData, int startIdx, int endIdx) {
        super(sensorData, startIdx, endIdx);
        this.minResult = Double.MAX_VALUE;
    }

    @Override
    public double processGrid() {
        double localMin = sensorData[startIdx];
        for (int i = startIdx + 1; i < endIdx; i++) {
            if (sensorData[i] < localMin) {
                localMin = sensorData[i];
            }
        }
        return localMin;
    }
