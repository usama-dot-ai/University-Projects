// Implementing Queue using Array (Circular Queue)
#include <iostream>
using namespace std;

class Queue
{
private:
    static const int num = 5;
    int array[num];
    int front, rear;

public:
    Queue() // Constructor
    {
        front = -1;
        rear = -1;
    }

    bool isEmpty() // Check if queue is empty
    {
        return front == -1;
    }

    bool isFull() // Check if queue is full using circular logic
    {
        return (rear + 1) % num == front;
    }

    void enqueue() // Add an element to the rear of the queue
    {
        int val;
        cout << "Enter Value to Insert: ";
        cin >> val;

        if (isFull()) // Check if queue is full
        {
            cout << "Queue is Full." << endl;
            return;
        }
        if (isEmpty()) // If queue is empty, set front to 0
        {
            front = 0;
        }

        // Move rear circularly and insert value
        rear = (rear + 1) % num;
        array[rear] = val;
        cout << "Value " << val << " Added." << endl;
    }

    void dequeue() // Remove an element from the front of the queue
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is Empty." << endl;
            return;
        }

        cout << "Value " << array[front] << " Removed." << endl;

        if (front == rear) // If only one element, reset to empty
        {
            front = rear = -1;
        }
        else
        {
            // Move front circularly
            front = (front + 1) % num;
        }
    }

    void display() // Print all elements from front to rear
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is Empty." << endl;
            return;
        }

        cout << "Values (Front to Rear): ";
        int i = front;

        while (true) // Loop through the circular queue
        {
            cout << array[i];
            if (i == rear) // Break if reached rear
            {
                break;
            }
            cout << " -> ";
            // Move to next index circularly
            i = (i + 1) % num;
        }
        cout << endl;
    }
};

int main()
{
    Queue q;
    int choice;
    cout << "\n-: Circular Queue Implementation using Array :-" << endl;

    do
    {
        cout << "\nMenu:" << endl;
        cout << "1. Enqueue (Insert an element)" << endl;
        cout << "2. Dequeue (Delete an element)" << endl;
        cout << "3. Display (Show all elements)" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            q.enqueue();
            break;
        case 2:
            q.dequeue();
            break;
        case 3:
            q.display();
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);
    return 0;
}
