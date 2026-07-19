// Implementing Stack using Array
#include <iostream>
using namespace std;

class Stack
{
    int array[5];
    int top;

public:
    Stack() { top = -1; } // Constructor

    void push() // Push method: Add an element to the top of the stack
    {
        int val;
        cout << "Enter Value to Push: ";
        cin >> val;

        if (top >= 4) // Check for stack overflow (array full)
        {
            cout << "Stack Overflow!" << endl;
            return;
        }
        top++; // Increment top and assign value
        array[top] = val;
        cout << "Pushed: " << val << endl;
    }

    int pop() // Pop method: Remove and return the top element
    {
        if (top < 0) // Check for stack underflow (empty stack)
        {
            cout << "Stack Underflow!" << endl;
            return -1;
        }

        int popVal;
        // Retrieve and decrement top
        popVal = array[top--];
        cout << "Popped: " << popVal << endl;
        return popVal;
    }

    int peek() // Peek method: View the top element without removing it
    {
        if (top < 0) // Check if stack is empty
        {
            cout << "Stack is Empty." << endl;
            return -1;
        }
        cout << "Top Element: " << array[top] << endl;
        return array[top];
    }

    void display() // Display method: Print all elements from top to bottom
    {
        if (top < 0) // Check if stack is empty
        {
            cout << "Stack is Empty." << endl;
            return;
        }
        cout << "Stack (Top -> Bottom): ";

        for (int i = top; i >= 0; i--) // Loop from top to bottom to display elements
        {
            cout << array[i];
            if (i != 0)
            {
                cout << " -> ";
            }
        }
        cout << endl;
    }
};

int main()
{
    Stack s;
    int choice;

    cout << "\n-: Stack Implementation using Array :-" << endl;

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
            s.push();
            break;
        case 2:
            s.pop();
            break;
        case 3:
            s.peek();
            break;
        case 4:
            s.display();
            break;
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
