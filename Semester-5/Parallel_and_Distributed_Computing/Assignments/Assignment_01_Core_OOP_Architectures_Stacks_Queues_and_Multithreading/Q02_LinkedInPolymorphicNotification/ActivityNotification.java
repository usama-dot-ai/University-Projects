public class ActivityNotification {
    protected String senderName;
    protected String timestamp;

    public ActivityNotification(String senderName, String timestamp) {
        this.senderName = senderName;
        this.timestamp = timestamp;
    }

    public void dispatch() {
        System.out.println("[" + timestamp + "] Generic Notification from " + senderName);
    }
}