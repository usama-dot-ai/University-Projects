// implementing Stack using Linked List
#include <iostream>
using namespace std;

class Node // Node class for linked list elements
{
public:
    int data;
    Node *next;

    Node(int d) : data(d), next(nullptr) {}
};

class Stack // Stack class implementing LIFO using linked list
{
private:
    Node *TopNode;

public:
    Stack() { TopNode = nullptr; } // Constructor: Initialize top to null

    void push(int val) // Push: Add element to top of stack
    {
        Node *newNode = new Node(val);
        if (TopNode == nullptr)
        {
            TopNode = newNode;
        }
        else
        {
            newNode->next = TopNode;
            TopNode = newNode;
        }
        cout << "Pushed: " << val << endl;
    }

    int pop() // Pop: Remove and return top element
    {
        if (TopNode == nullptr)
        {
            cout << "Stack is Empty. Cannot pop." << endl;
            return -1;
        }

        int value = TopNode->data;
        Node *temp = TopNode;
        TopNode = TopNode->next;
        delete temp;
        cout << "Popped: " << value << endl;
        return value;
    }

    int peek() // Peek: View top element without removing
    {
        if (TopNode == nullptr)
        {
            cout << "Stack is Empty. No top element." << endl;
            return -1;
        }

        cout << "\nTop element: " << TopNode->data << endl;
        return TopNode->data;
    }

    void display() // Display: Show all elements from top to bottom
    {
        if (TopNode == nullptr)
        {
            cout << "The Stack is Empty." << endl;
            return;
        }

        cout << "\nStack (Top -> Bottom): ";
        Node *current = TopNode;
        while (current != nullptr)
        {
            cout << current->data;
            if (current->next != nullptr)
            {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
    }

    ~Stack() // Destructor: Clean up memory
    {
        while (TopNode != nullptr)
        {
            Node *temp = TopNode;
            TopNode = TopNode->next;
            delete temp;
        }
    }
};

int main()
{
    cout << endl;
    Stack s1;
    int choice, value;

    cout << "\n-: Stack Implementation using Linked List :-" << endl;
    do
    {
        cout << "\nMenu:" << endl;
        cout << "1. PUSH (Insert an element)" << endl;
        cout << "2. POP (Delete an element)" << endl;
        cout << "3. PEEK (View the top element)" << endl;
        cout << "4. DISPLAY (Show all elements)" << endl;
        cout << "5. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter value to push: ";
            cin >> value;
            s1.push(value);
            break;
        case 2:
            s1.pop();
            break;
        case 3:
            s1.peek();
            break;
        case 4:
            s1.display();
            break;
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    cout << endl;
    return 0;
}