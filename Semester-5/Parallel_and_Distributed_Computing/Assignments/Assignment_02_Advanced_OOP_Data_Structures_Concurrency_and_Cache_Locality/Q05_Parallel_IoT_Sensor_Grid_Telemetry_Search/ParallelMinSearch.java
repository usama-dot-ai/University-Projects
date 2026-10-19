public class ParallelMinSearch extends SensorSearchEngine implements Runnable {
    private double minResult;

    public ParallelMinSearch(double[] sensorData, int startIdx, int endIdx) {
        super(sensorData, startIdx, endIdx);
        this.minResult = Double.MAX_VALUE;
    }
