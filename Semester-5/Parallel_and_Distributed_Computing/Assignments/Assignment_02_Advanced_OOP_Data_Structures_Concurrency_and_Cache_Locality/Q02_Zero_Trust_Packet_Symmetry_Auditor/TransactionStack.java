public class TransactionStack {
    private char[] stackArray;
    private int top;
    private int capacity;

    public TransactionStack(int capacity) {
        this.capacity = capacity;
        this.stackArray = new char[capacity];
        this.top = -1;
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public boolean isFull() {
        return top == capacity - 1;
    }
