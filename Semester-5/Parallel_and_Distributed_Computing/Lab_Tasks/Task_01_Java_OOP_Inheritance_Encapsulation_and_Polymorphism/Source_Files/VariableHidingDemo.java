// Superclass
class P {
    int a = 30;
}

// Child class inheriting from P
class Q extends P {
    int a = 50; // Hides P.a
}

public class VariableHidingDemo {
    public static void main(String[] args) {
        // Instantiate Q using reference variable q of type Q
        Q q = new Q();

        // Assign q to a superclass reference variable p of type P
        P p = q;

        // Access instance variables through both references
        System.out.println("Value accessed via q.a: " + q.a);
        System.out.println("Value accessed via p.a: " + p.a);
    }
}