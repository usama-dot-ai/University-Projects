// Implementing Queue using Linked List
#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node *next;

    Node(int d) : data(d), next(nullptr) {} // Constructor
};

class Queue
{
private:
    Node *front;
    Node *rear;

public:
    Queue() : front(nullptr), rear(nullptr) {} // Constructor

    bool isEmpty() // Check if queue is empty
    {
        return front == nullptr;
    }

    void enqueue() // Add an element to the rear of the queue
    {
        int val;
        cout << "Enter Value to Insert: ";
        cin >> val;

        Node *newNode = new Node(val);

        if (isEmpty()) // If queue is empty, set front and rear to new node
        {
            front = rear = newNode;
        }
        else
        {
            // Link new node to rear and update rear
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Value " << val << " Added." << endl;
    }

    void dequeue() // Remove an element from the front of the queue
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is Empty." << endl;
            return;
        }

        Node *temp = front;
        int val = front->data;
        // Move front to next node
        front = front->next;
        delete temp; // Free memory

        cout << "Value " << val << " Removed." << endl;
    }

    void display() // Print all elements from front to rear
    {
        if (isEmpty()) // Check if queue is empty
        {
            cout << "Queue is Empty." << endl;
            return;
        }

        Node *current = front;
        cout << "Values (Front to Rear): ";
        // Loop through the queue to display elements
        while (current)
        {
            cout << current->data;
            if (current->next)
            {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
    }

    ~Queue() // Clean up memory by dequeuing all elements
    {
        // Dequeue until queue is empty
        while (!isEmpty())
        {
            dequeue();
        }
    }
};

int main()
{
    Queue q;
    int choice;

    cout << "\n-: Queue Implementation using Linked List :-" << endl;

    do
    {
        cout << "\nMenu:" << endl;
        cout << "1. Enqueue (Insert an element)" << endl;
        cout << "2. Dequeue (Delete an element)" << endl;
        cout << "3. DISPLAY (Show all elements)" << endl;
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
