public class RequestQueue {
    private char[] queueArray;
    private int front;
    private int rear;
    private int size;
    private int capacity;

    public RequestQueue(int capacity) {
        this.capacity = capacity;
        this.queueArray = new char[capacity];
        this.front = 0;
        this.rear = -1;
        this.size = 0;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public boolean isFull() {
        return size == capacity;
    }

    public void enqueue(char c) {
        if (!isFull()) {
            rear = (rear + 1) % capacity;
            queueArray[rear] = c;
            size++;
        } else {
            System.out.println("Queue Overflow");
        }
    }

    public char dequeue() {
        if (!isEmpty()) {
            char val = queueArray[front];
            front = (front + 1) % capacity;
            size--;
            return val;
        }
        return '\0';
    }
}
