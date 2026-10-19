public class CIPipelineDispatcher {
    private JobQueue jobQueue;

    public CIPipelineDispatcher(int maxCapacity) {
        this.jobQueue = new JobQueue(maxCapacity);
    }

    public void submitBuildJob(String jobName) {
        System.out.println("Submitting Build Job: " + jobName);
        jobQueue.enqueue(jobName);
    }

    public void processNextJob() {
        String job = jobQueue.dequeue();
        if (job != null) {
            System.out.println("Processing & Executing: " + job + " -> Build Artifacts Generated.");
        }
    }

    public void showPipelineStatus() {
        jobQueue.displayQueue();
    }
}