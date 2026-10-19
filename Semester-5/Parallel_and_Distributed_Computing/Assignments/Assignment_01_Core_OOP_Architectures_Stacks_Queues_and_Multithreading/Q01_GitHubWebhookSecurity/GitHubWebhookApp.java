public class GitHubWebhookApp {
    public static void main(String[] args) {
        CommitSecurityUtils utils = new CommitSecurityUtils();

        String sampleHash = "a1b2c3d4";
        String samplePayload = "commit_id_9021";

        System.out.println("--- GitHub Webhook Security Telemetry ---");

        // 1. Reverse Hash
        String reversed = utils.reverseHash(sampleHash);
        System.out.println("Original Hash: " + sampleHash);
        System.out.println("Reversed Hash: " + reversed);

        // 2. Payload Digit Sum
        int digitSum = utils.calculatePayloadSum(samplePayload);
        System.out.println("Payload: " + samplePayload);
        System.out.println("Extracted Digits Sum: " + digitSum);

        // 3. Security Token (Factorial)
        long token = utils.generateSecurityToken(digitSum);
        System.out.println("Generated Security Token (Factorial of " + digitSum + "): " + token);
    }
}