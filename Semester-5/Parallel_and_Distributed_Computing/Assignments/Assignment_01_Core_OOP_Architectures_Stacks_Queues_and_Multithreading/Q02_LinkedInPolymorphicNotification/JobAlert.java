public class JobAlert extends ActivityNotification {
    private String jobTitle;
    private String companyName;

    public JobAlert(String senderName, String timestamp, String jobTitle, String companyName) {
        super(senderName, timestamp);
        this.jobTitle = jobTitle;
        this.companyName = companyName;
    }

    @Override
    public void dispatch() {
        System.out.println("[" + timestamp + "] Job Alert from " + senderName
                + ": New opening for " + jobTitle + " at " + companyName);
    }
}