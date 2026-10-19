class TransactionStack {
    private int[] stackArray;
    private int top;
    private int capacity;

    // Constructor to initialize stack
    public TransactionStack(int size) {
        capacity = size;
        stackArray = new int[capacity];
        top = -1; // -1 indicates an empty stack
    }

    // Push operation to add a transaction ID
    public void push(int transactionId) {
        if (top == capacity - 1) {
            System.out.println("Stack Overflow: Cannot log transaction " + transactionId);
            return;
        }
        stackArray[++top] = transactionId;
        System.out.println("Logged Transaction: TXN_" + transactionId);
    }

    // Pop operation to unwind the stack
    public int pop() {
        if (isEmpty()) {
            System.out.println("Stack Underflow: No transactions to unwind.");
            return -1;
        }
        return stackArray[top--];
    }

    // Check if the stack is empty
    public boolean isEmpty() {
        return top == -1;
    }
}

public class TransactionProcessor {
    public static void main(String[] args) {
        TransactionStack stack = new TransactionStack(5);

        // 1. Ingest transaction IDs
        System.out.println("--- Pushing Transactions onto Stack ---");
        stack.push(1001);
        stack.push(1002);
        stack.push(1003);
        stack.push(1004);

        // 2. Unwind stack in reverse chronological order
        System.out.println("\n--- Unwinding Execution Trace (LIFO) ---");
        while (!stack.isEmpty()) {
            int txn = stack.pop();
            System.out.println("Recovered / Rolled Back: TXN_" + txn);
        }
    }
}