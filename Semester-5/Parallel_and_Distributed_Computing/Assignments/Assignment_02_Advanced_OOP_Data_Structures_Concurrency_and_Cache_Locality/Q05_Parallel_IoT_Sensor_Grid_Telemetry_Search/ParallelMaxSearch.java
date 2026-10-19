public class ParallelMaxSearch extends SensorSearchEngine implements Runnable {
    private double maxResult;

    public ParallelMaxSearch(double[] sensorData, int startIdx, int endIdx) {
        super(sensorData, startIdx, endIdx);
        this.maxResult = -Double.MAX_VALUE;
    }

    @Override
    public double processGrid() {
        double localMax = sensorData[startIdx];
        for (int i = startIdx + 1; i < endIdx; i++) {
            if (sensorData[i] > localMax) {
                localMax = sensorData[i];
            }
        }
        return localMax;
    }
