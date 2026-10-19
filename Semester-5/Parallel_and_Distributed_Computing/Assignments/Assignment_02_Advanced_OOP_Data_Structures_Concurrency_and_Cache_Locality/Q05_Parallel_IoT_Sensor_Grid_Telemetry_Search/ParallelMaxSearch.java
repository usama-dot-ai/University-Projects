public class ParallelMaxSearch extends SensorSearchEngine implements Runnable {
    private double maxResult;

    public ParallelMaxSearch(double[] sensorData, int startIdx, int endIdx) {
        super(sensorData, startIdx, endIdx);
        this.maxResult = -Double.MAX_VALUE;
    }
