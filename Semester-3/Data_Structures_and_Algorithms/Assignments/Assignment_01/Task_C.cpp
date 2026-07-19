#include <iostream>
using namespace std;

class Node // Node class for doubly linked list elements
{
public:
    int data;
    Node *next;
    Node *prev;
};

class CircularList // Circular doubly linked list class
{
private:
    Node *head = nullptr;
    Node *tail = nullptr;

public:
    Node *createNode() // Creates a new node with user input
    {
        int val;
        cout << "Enter a Number: ";
        cin >> val;

        Node *newNode = new Node();
        newNode->data = val;
        newNode->next = nullptr;
        newNode->prev = nullptr;

        return newNode;
    }

    void add_At_End() // Adds a new node at the end of the list
    {
        Node *newNode = createNode();

        if (tail == nullptr) // If list is empty
        {
            head = newNode;       // Set head to new node
            tail = newNode;       // Set tail to new node
            newNode->next = head; // Point next to itself
            newNode->prev = tail; // Point prev to itself
        }
        else // List not empty
        {
            tail->next = newNode; // Link new node after tail
            newNode->prev = tail; // Set new node's prev to tail
            tail = newNode;       // Update tail to new node
            tail->next = head;    // Maintain circular link
            head->prev = tail;    // Update head's prev
        }
        cout << "New Node Added at End." << endl;
        system("pause");
    }

    void delete_value() // Deletes a node with the specified value
    {
        if (head == nullptr) // Check if list is empty
        {
            cout << "List is empty. Cannot delete." << endl;
            system("pause");
            return;
        }
        int value;
        cout << "Enter the value to delete: ";
        cin >> value;
        Node *current = head;
        do
        {
            if (current->data == value)
            {
                if (head == tail) // Only one node
                {
                    head = nullptr;
                    tail = nullptr;
                }
                else if (current == head) // Deleting head
                {
                    if (head == nullptr) // Redundant check
                    {
                        cout << "List is empty. Cannot delete." << endl;
                        return;
                    }

                    Node *temp = head;

                    if (head == tail)
                    {
                        head = nullptr;
                        tail = nullptr;
                    }
                    else
                    {
                        head = head->next; // Move head to next node
                        tail->next = head; // Update tail's next to new head
                        head->prev = tail; // Update new head's prev to tail
                    }

                    delete temp;
                    cout << "Node deleted at first." << endl;
                    system("pause");
                    return;
                }
                else if (current == tail) // Deleting tail
                {
                    if (tail == nullptr) // Redundant check
                    {
                        cout << "List is empty. Cannot delete." << endl;
                        return;
                    }

                    Node *temp = tail;

                    if (head == tail)
                    {
                        head = nullptr;
                        tail = nullptr;
                    }
                    else
                    {
                        tail = tail->prev; // Move tail to previous node
                        tail->next = head; // Update new tail's next to head
                        head->prev = tail; // Update head's prev to new tail
                    }

                    delete temp;
                    cout << "Node deleted at last." << endl;
                    system("pause");
                    return;
                }
                else // Deleting a middle node
                {
                    current->prev->next = current->next; // Bypass current node in next direction
                    current->next->prev = current->prev; // Bypass current node in prev direction
                }
                delete current;
                cout << "Node with value " << value << " deleted." << endl;
                system("pause");
                return;
            }
            current = current->next; // Move to next node
        } while (current != head); // Loop until back to head

        cout << "Value " << value << " not found in the list." << endl;
        system("pause");
    }

    void displayList() // Displays the list
    {
        if (head == nullptr) // Check if list is empty
        {
            cout << "List is empty." << endl;
            return;
        }

        Node *current = head;
        cout << "Circular Linked List: ";
        do
        {
            cout << current->data << " ";
            current = current->next;
        } while (current != head); // Traverse until back to head
        cout << endl;
        system("pause");
    }
};

int main()
{
    CircularList list;
    int choice;

    while (true) // Infinite loop for menu
    {
        cout << "\n1. Add at End\n"
             << "2. Delete Value\n"
             << "3. Display List\n"
             << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            list.add_At_End();
            break;
        case 2:
            list.delete_value();
            break;
        case 3:
            list.displayList();
            break;
        case 4:
            return 0;
        default:
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}
