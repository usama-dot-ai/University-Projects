// Array-based Stack for characters
class CharStack {
    private char[] arr;
    private int top;

    public CharStack(int capacity) {
        arr = new char[capacity];
        top = -1;
    }

    public void push(char ch) {
        arr[++top] = ch;
    }

    public char pop() {
        return arr[top--];
    }
}

// Array-based Queue for characters
class CharQueue {
    private char[] arr;
    private int front;
    private int rear;

    public CharQueue(int capacity) {
        arr = new char[capacity];
        front = 0;
        rear = -1;
    }

    public void enqueue(char ch) {
        arr[++rear] = ch;
    }

    public char dequeue() {
        return arr[front++];
    }
}

public class IntegrityVerifier {
    public static void main(String[] args) {
        String payload = "RADAR";
        int len = payload.length();

        CharStack stack = new CharStack(len);
        CharQueue queue = new CharQueue(len);

        // 1. Populate both Stack and Queue simultaneously
        for (int i = 0; i < len; i++) {
            char ch = payload.charAt(i);
            stack.push(ch);
            queue.enqueue(ch);
        }

        // 2. Compare LIFO (stack) and FIFO (queue) outputs
        boolean isSymmetric = true;
        for (int i = 0; i < len; i++) {
            char fromStack = stack.pop();
            char fromQueue = queue.dequeue();

            if (fromStack != fromQueue) {
                isSymmetric = false;
                break;
            }
        }

        // 3. Log results
        System.out.println("Inspected Payload: " + payload);
        if (isSymmetric) {
            System.out.println("Verification SUCCESS: Payload exhibits bi-directional symmetry.");
        } else {
            System.out.println("Verification FAILED: Payload is asymmetric.");
        }
    }
}