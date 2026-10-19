public class LinkedInActivityApp {
    public static void main(String[] args) {
        // Polymorphism: Superclass reference variables pointing to Subclass objects
        ActivityNotification notif1 = new ConnectionRequest("Ibtehaj Ali Mirza", "10:15 AM",
                "Hi, I'd like to join your network!");
        ActivityNotification notif2 = new JobAlert("LinkedIn Talent Solutions", "10:30 AM",
                "Cybersecurity Engineer", "Google");

        System.out.println("--- Dispatching LinkedIn Notifications ---");
        // Runtime method overriding executes appropriate subclass logic
        notif1.dispatch();
        notif2.dispatch();
    }
}