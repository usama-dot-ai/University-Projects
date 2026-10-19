// Base Class
class Order {
    protected String orderId;
    protected double basePrice;

    // Base Constructor
    public Order(String orderId, double basePrice) {
        this.orderId = orderId;
        this.basePrice = basePrice;
    }

    // Method to be overridden by subclasses
    public double calculateTotal() {
        return this.basePrice;
    }

    public void printSummary() {
        System.out.printf("Order ID: %s | Base Price: $%.2f | Final Billable Total: $%.2f%n",
                orderId, basePrice, calculateTotal());
    }
}

// Subclass 1: Standard Order (Flat 5% shipping fee)
class StandardOrder extends Order {

    public StandardOrder(String orderId, double basePrice) {
        // Forward shared attributes to parent constructor
        super(orderId, basePrice);
    }

    @Override
    public double calculateTotal() {
        double shippingFee = this.basePrice * 0.05;
        return this.basePrice + shippingFee;
    }
}

// Subclass 2: Flash Sale Order (10% discount on base price + $10 rush shipping)
class FlashSaleOrder extends Order {

    public FlashSaleOrder(String orderId, double basePrice) {
        // Forward shared attributes to parent constructor
        super(orderId, basePrice);
    }

    @Override
    public double calculateTotal() {
        double discountedPrice = this.basePrice * 0.90; // 10% discount
        double rushShippingFee = 10.00;
        return discountedPrice + rushShippingFee;
    }
}

// Driver class demonstrating polymorphism
public class CheckoutSystem {
    public static void main(String[] args) {
        // Using base class references to demonstrate runtime polymorphism
        Order standardOrder = new StandardOrder("ORD-STD-101", 100.00);
        Order flashSaleOrder = new FlashSaleOrder("ORD-FLS-202", 80.00);

        System.out.println("=== Checkout Pipeline Processing ===");
        standardOrder.printSummary();
        flashSaleOrder.printSummary();
    }
}