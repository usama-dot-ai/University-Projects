public class GitHubActionsApp {
    public static void main(String[] args) {
        CIPipelineDispatcher dispatcher = new CIPipelineDispatcher(5);

        System.out.println("--- Submitting Continuous Integration Jobs ---");
        dispatcher.submitBuildJob("Job_101_LintCheck");
        dispatcher.submitBuildJob("Job_102_UnitTests");
        dispatcher.submitBuildJob("Job_103_IntegrationTests");
        dispatcher.submitBuildJob("Job_104_DockerBuild");

        System.out.println("\n--- Current Queue Status ---");
        dispatcher.showPipelineStatus();

        System.out.println("\n--- Executing Two Build Jobs (FIFO) ---");
        dispatcher.processNextJob();
        dispatcher.processNextJob();

        System.out.println("\n--- Updated Queue Status ---");
        dispatcher.showPipelineStatus();
    }
}