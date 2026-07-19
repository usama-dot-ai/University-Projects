#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node *next;
    Node(int d) : data(d), next(nullptr) {}
};

class Stack
{
private:
    Node *top;

public:
    Stack() : top(nullptr) {}

    bool isEmpty() { return top == nullptr; }

    void push(int val)
    {
        Node *newNode = new Node(val);
        newNode->next = top;
        top = newNode;
    }
    int pop()
    {
        if (isEmpty())
        {
            return -1;
        }
        Node *temp = top;
        int val = top->data;
        top = top->next;
        delete temp;
        return val;
    }
};

class Queue
{
private:
    Node *front;
    Node *rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    bool isEmpty() { return front == nullptr; }

    void enqueue(int val)
    {
        Node *newNode = new Node(val);
        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
    int dequeue()
    {
        if (isEmpty())
        {
            return -1;
        }
        Node *temp = front;
        int val = front->data;
        front = front->next;
        delete temp;
        return val;
    }
    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is Empty." << endl;
            return;
        }
        Node *current = front;
        cout << "Values (Front to Rear): ";
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
    void reverse()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return;
        }
        Stack s;
        while (!isEmpty())
        {
            int val = dequeue();
            if (val != -1)
            {
                s.push(val);
            }
        }
        while (!s.isEmpty())
        {
            int val = s.pop();
            if (val != -1)
            {
                enqueue(val);
            }
        }
        cout << "Queue reversed." << endl;
    }
};

int main()
{
    Queue q;

    q.enqueue(7);
    q.enqueue(8);
    q.enqueue(9);
    cout << "\nAfter Inserting 7, 8, 9:" << endl;
    q.display();

    // Reverse the queue
    q.reverse();
    cout << "After reversing:" << endl;
    q.display();

    cout << endl;
    return 0;
}
