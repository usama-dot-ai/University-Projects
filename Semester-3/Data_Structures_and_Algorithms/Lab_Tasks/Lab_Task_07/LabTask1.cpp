#include <iostream>
using namespace std;
int const num = 5;
class Queue
{
private:
    int array[num];
    int front, rear;

public:
    Queue() : front(0), rear(-1) {}

    bool isEmpty()
    {
        return front > rear;
    }

    bool isFull()
    {
        return rear == num - 1;
    }

    void enqueue()
    {
        int val;
        cout << "Enter value to Insert: ";
        cin >> val;

        if (isFull())
        {
            cout << "Queue is full." << endl;
            return;
        }
        /* else if (front != 0 && rear = num-1)
        {
            rear = 0;
            array[rear] = value;
        }


        */

        rear++;
        array[rear] = val;
        cout << "Value " << val << " Added." << endl;
    }

    void dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return;
        }
        /*
            else if(front==rear)
            {
                front = -1;
                rear = -1;
            }
            else if (front == num-1)
            {
                front = 0;
            }
            else
            {
                front ++;
            }
        */

        cout << "Value " << array[front] << " Removed." << endl;
        front++;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Queue elements (Front to Rear): ";

        for (int i = front; i <= rear; i++)
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