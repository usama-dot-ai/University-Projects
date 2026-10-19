public class FIXOrderEvent extends TradeEvent {
    private String clOrdId;

    public FIXOrderEvent(String senderId, long timestamp, double basePrice, String clOrdId) {
        super(senderId, timestamp, basePrice);
        this.clOrdId = clOrdId;
    }

    @Override
    public void dispatch() {
        // Exchange transaction fee: 0.15%
        double fee = basePrice * 0.0015;
        double totalCost = basePrice + fee;

        System.out.println("[FIX GATEWAY] Order ID: " + clOrdId
                + " | Sender: " + senderId
                + " | Executing Stock Purchase"
                + " | Base: $" + String.format("%.2f", basePrice)
                + " | Fee (0.15%): $" + String.format("%.2f", fee)
                + " | Total Cost: $" + String.format("%.2f", totalCost));
    }
}
