public class JobQueue {
    private String[] queueArray;
    private int front;
    private int rear;
    private int size;
    private int capacity;

    public JobQueue(int capacity) {
        this.capacity = capacity;
        this.queueArray = new String[capacity];
        this.front = 0;
        this.rear = -1;
        this.size = 0;
    }

    public boolean isFull() {
        return size == capacity;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    // Add pending jobs at the rear
    public void enqueue(String jobId) {
        if (isFull()) {
            System.out.println("Queue Overflow: Job queue is full.");
            return;
        }
        rear++;
        queueArray[rear] = jobId;
        size++;
    }

    // Remove job from front and shift elements left
    public String dequeue() {
        if (isEmpty()) {
            System.out.println("Queue Underflow: No jobs to process.");
            return null;
        }
        String removedJob = queueArray[front];

        // Shift elements to maintain standard 0-indexed queue
        for (int i = 0; i < rear; i++) {
            queueArray[i] = queueArray[i + 1];
        }
        rear--;
        size--;
        return removedJob;
    }

    public String peek() {
        if (isEmpty()) {
            return null;
        }
        return queueArray[front];
    }

    public void displayQueue() {
        if (isEmpty()) {
            System.out.println("Job Queue is empty.");
            return;
        }
        System.out.print("Pending Jobs [ ");
        for (int i = 0; i <= rear; i++) {
            System.out.print(queueArray[i] + (i < rear ? " -> " : ""));
        }
        System.out.println(" ]");
    }
}