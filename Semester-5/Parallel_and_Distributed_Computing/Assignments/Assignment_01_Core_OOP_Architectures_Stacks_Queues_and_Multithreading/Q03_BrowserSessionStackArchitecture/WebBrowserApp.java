public class WebBrowserApp {
    public static void main(String[] args) {
        TabHistoryManager browserTab = new TabHistoryManager(10);

        System.out.println("--- Browser Navigation Simulation ---");
        browserTab.visitURL("https://google.com");
        browserTab.visitURL("https://github.com");
        browserTab.visitURL("https://stackoverflow.com");

        System.out.println("\nCurrent Active Tab: " + browserTab.getCurrentURL());

        System.out.println("\n--- Testing Navigation History ---");
        browserTab.goBack(); // Returns to github.com
        browserTab.goBack(); // Returns to google.com
        browserTab.goForward(); // Returns to github.com

        System.out.println("\nFinal Active Tab: " + browserTab.getCurrentURL());
    }
}