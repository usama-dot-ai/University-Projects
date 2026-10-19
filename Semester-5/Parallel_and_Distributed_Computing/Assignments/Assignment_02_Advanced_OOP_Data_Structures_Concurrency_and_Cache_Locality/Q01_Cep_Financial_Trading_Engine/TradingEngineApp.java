public class TradingEngineApp {
    public static void main(String[] args) {
        System.out.println("=== Complex Event Processing (CEP) Engine Dispatcher ===");

        long now = System.currentTimeMillis();

        // Polymorphic instantiations using superclass references
        TradeEvent fixOrder = new FIXOrderEvent("FIX_GW_NYC", now, 150000.00, "ORD_US_99214");
        TradeEvent reutersQuote = new ReutersQuoteEvent("REUTERS_LDN", now + 15, 240.50, 240.45, 240.55);
