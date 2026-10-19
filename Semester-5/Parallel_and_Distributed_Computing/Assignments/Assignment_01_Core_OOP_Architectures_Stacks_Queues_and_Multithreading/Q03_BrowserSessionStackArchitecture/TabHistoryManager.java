public class TabHistoryManager {
    private URLStack backStack;
    private URLStack forwardStack;
    private String currentURL;

    public TabHistoryManager(int maxCapacity) {
        backStack = new URLStack(maxCapacity);
        forwardStack = new URLStack(maxCapacity);
        currentURL = "about:blank";
    }

    public void visitURL(String url) {
        if (currentURL != null && !currentURL.equals("about:blank")) {
            backStack.push(currentURL);
        }
        currentURL = url;
        // Clear forward history on new navigation
        while (!forwardStack.isEmpty()) {
            forwardStack.pop();
        }
        System.out.println("Visited: " + currentURL);
    }

    public void goBack() {
        if (backStack.isEmpty()) {
            System.out.println("Cannot go back: Back history is empty.");
            return;
        }
        forwardStack.push(currentURL);
        currentURL = backStack.pop();
        System.out.println("Went Back to: " + currentURL);
    }

    public void goForward() {
        if (forwardStack.isEmpty()) {
            System.out.println("Cannot go forward: Forward history is empty.");
            return;
        }
        backStack.push(currentURL);
        currentURL = forwardStack.pop();
        System.out.println("Went Forward to: " + currentURL);
    }

    public String getCurrentURL() {
        return currentURL;
    }
}