class CustomQueue {
    private int[] data;
    private int front;
    private int rear;
    private int size;
    private int capacity;

    public CustomQueue(int capacity) {
        this.capacity = capacity;
        data = new int[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    // Insert task into buffer
    public void enqueue(int task) {
        if (size == capacity) {
            System.out.println("Buffer Overflow: Cannot enqueue task " + task);
            return;
        }
        rear++;
        data[rear] = task;
        size++;
    }

    // Process task from buffer
    public int dequeue() {
        if (isEmpty()) {
            System.out.println("Buffer Underflow: No tasks to process.");
            return -1;
        }
        int task = data[front];
        front++;
        size--;
        return task;
    }

    public boolean isEmpty() {
        return size == 0;
    }
}

public class MessageQueueBuffer {
    public static void main(String[] args) {
        // Raw incoming request batch
        int[] rawBatch = {501, 502, 503, 504, 505};
        int n = rawBatch.length;

        CustomQueue buffer = new CustomQueue(n);

        // 1. Enqueue elements from the array sequentially
        System.out.println("--- Buffering Incoming Tasks ---");
        for (int i = 0; i < n; i++) {
            System.out.println("Enqueuing task: " + rawBatch[i]);
            buffer.enqueue(rawBatch[i]);
        }

        // 2. Dequeue and consume tasks in FIFO order
        System.out.println("\n--- Processing Tasks in Arrival Order ---");
        while (!buffer.isEmpty()) {
            int task = buffer.dequeue();
            System.out.println("Worker consumed task: " + task);
        }
    }
}