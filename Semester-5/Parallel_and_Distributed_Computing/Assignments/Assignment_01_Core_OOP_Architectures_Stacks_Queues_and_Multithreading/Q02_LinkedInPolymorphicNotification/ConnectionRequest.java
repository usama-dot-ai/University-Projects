public class ConnectionRequest extends ActivityNotification {
    private String connectionNote;

    public ConnectionRequest(String senderName, String timestamp, String connectionNote) {
        super(senderName, timestamp);
        this.connectionNote = connectionNote;
    }

    @Override
    public void dispatch() {
        System.out.println("[" + timestamp + "] Connection Request from " + senderName
                + " | Note: \"" + connectionNote + "\"");
    }
}