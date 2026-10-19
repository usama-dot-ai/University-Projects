public class URLStack {
    private String[] stackArray;
    private int top;
    private int capacity;

    public URLStack(int capacity) {
        this.capacity = capacity;
        this.stackArray = new String[capacity];
        this.top = -1;
    }

    public boolean isFull() {
        return top == capacity - 1;
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public void push(String url) {
        if (isFull()) {
            System.out.println("Stack Overflow: Cannot push " + url);
            return;
        }
        top++;
        stackArray[top] = url;
    }

    public String pop() {
        if (isEmpty()) {
            System.out.println("Stack Underflow: No URLs to pop.");
            return null;
        }
        String popped = stackArray[top];
        top--;
        return popped;
    }

    public String peek() {
        if (isEmpty()) {
            return null;
        }
        return stackArray[top];
    }
}