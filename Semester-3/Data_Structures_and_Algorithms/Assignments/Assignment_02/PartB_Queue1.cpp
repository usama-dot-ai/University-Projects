#include <iostream>
using namespace std;

class Queue
{
private:
    static const int num = 5;
    int array[num];
    int front, rear;

public:
    Queue() : front(0), rear(-1) {} // Constructor

    bool isEmpty() // Check if queue is empty
    {
        return front > rear; // Empty if front exceeds rear
    }

    bool isFull() // Check if queue is full
    {
        return rear == num - 1; // Full if rear at last index
    }

    void enqueue() // Add an element to the rear of the queue
    {
        int val;
        cout << "Enter value to Insert: ";
        cin >> val;

        if (isFull()) // Check if queue is full
        {
            cout << "Queue is full." << endl;
            return;
        }
        // Increment rear and insert value
        rear++;
        array[rear] = val;
        cout << "Value " << val << " Added." << endl;
    }

    void dequeue() // Remove an element from the front of the queue
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is empty." << endl;
            return;
        }
        // Output dequeued value and increment front
        cout << "Value " << array[front] << " Removed." << endl;
        front++;
    }

    void display() // Print all elements from front to rear
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Queue elements (Front to Rear): ";

        for (int i = front; i <= rear; i++) // Display elements from front to rear
        {
            cout << array[i];
            if (i < rear)
            {
                cout << " -> ";
            }
        }
        cout << endl;
    }
};

int main()
{
    Queue q;
    int choice, value;

    cout << "\n-: Simple Queue Implementation using Array :-" << endl;

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