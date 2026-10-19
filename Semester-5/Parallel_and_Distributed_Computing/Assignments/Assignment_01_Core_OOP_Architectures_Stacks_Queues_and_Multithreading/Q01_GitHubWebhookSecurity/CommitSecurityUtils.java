public class CommitSecurityUtils {

    // Method to reverse a commit hash string using a basic loop
    public String reverseHash(String commitHash) {
        String reversed = "";
        for (int i = commitHash.length() - 1; i >= 0; i--) {
            reversed += commitHash.charAt(i);
        }
        return reversed;
    }

    // Method to extract digits and sum them up
    public int calculatePayloadSum(String payload) {
        int sum = 0;
        for (int i = 0; i < payload.length(); i++) {
            char ch = payload.charAt(i);
            if (ch >= '0' && ch <= '9') {
                sum += (ch - '0'); // Convert char digit to integer
            }
        }
        return sum;
    }

    // Method to compute factorial of the sum
    public long generateSecurityToken(int number) {
        long factorial = 1;
        for (int i = 1; i <= number; i++) {
            factorial *= i;
        }
        return factorial;
    }
}