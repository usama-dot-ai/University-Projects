#include <iostream>
using namespace std;

class Node // Represents a single element in the doubly linked list
{
public:
    int data;
    Node *next;
    Node *prev;
};

class DoublyList // Implements a doubly linked list with various operations
{
private:
    Node *head = nullptr;
    Node *tail = nullptr;
    int counter = 0;

public:
    Node *createNode() // Function to create a new node with user input
    {
        int val;
        cout << "Enter a Number: ";
        cin >> val;

        Node *newNode = new Node(); // Allocate memory for new node
        newNode->data = val;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        counter++;
        return newNode;
    }

    void add_At_Beginning() // Function to add a new node at the beginning of the list
    {
        Node *newNode = createNode();

        if (head == nullptr) // If list is empty
        {
            head = newNode; // Set head to new node
            tail = newNode; // Also set tail to new node
        }
        else
        {
            newNode->next = head;    // Point new node to current head
            head->prev = newNode;    // Set prev of current head to new node
            newNode->prev = nullptr; // New node's prev is nullptr
            head = newNode;          // Update head to new node
        }
        cout << "New Node Added at Beginning." << endl;
        system("pause");
    }

    void add_At_End() // Function to add a new node at the end of the list
    {
        Node *newNode = createNode();

        if (tail == nullptr) // If list is empty
        {
            head = newNode; // Set head to new node
            tail = newNode; // Set tail to new node
        }
        else
        {
            tail->next = newNode;    // Link current tail to new node
            newNode->prev = tail;    // Set prev of new node to current tail
            tail = newNode;          // Update tail to new node
            newNode->next = nullptr; // New node's next is nullptr
        }
        cout << "New Node Added at End." << endl;
        system("pause");
    }

    void add_At_Position() // Function to add a new node at a specific position in the list
    {
        int pos;
        cout << "Enter Desired Location: ";
        cin >> pos;

        if (head == nullptr) // If list is empty
        {
            cout << "List is Empty." << endl;
            system("pause");
            return;
        }
        if (pos < 1 || pos > counter + 1) // Check for invalid position
        {
            cout << "Invalid Position." << endl;
            system("pause");
            return;
        }
        if (pos == 1) // If position is 1, add at beginning
        {
            add_At_Beginning();
            return;
        }
        if (pos == counter + 1) // If position is at end, add at end
        {
            add_At_End();
            return;
        }

        Node *current = head; // Start from head
        int index = 1;
        while (current->next != nullptr && index++ < pos - 1) // Traverse to position-1
        {
            current = current->next;
        }
        Node *newNode = createNode();
        newNode->next = current->next; // Link new node to next
        if (current->next != nullptr)  // If there's a node after current
        {
            current->next->prev = newNode; // Set prev of the next node
        }
        current->next = newNode;      // Link current to new node
        newNode->prev = current;      // Set prev of new node
        if (newNode->next == nullptr) // If new node is now the last
        {
            tail = newNode; // Update tail
        }
        cout << "Node Added at Desired Location." << endl;
        system("pause");
    }

    void delete_By_Position() // Function to delete a node at a specific position
    {
        int pos;
        cout << "Enter Desired Position: ";
        cin >> pos;

        if (pos <= 0) // Invalid position check
        {
            cout << "Invalid Position (must be greater than 0)." << endl;
            system("pause");
            return;
        }

        if (head == nullptr) // If list is empty
        {
            cout << "List is Empty." << endl;
            system("pause");
            return;
        }

        if (pos == 1) // If deleting head
        {
            Node *temp = head;
            head = head->next;
            if (head != nullptr)
            {
                head->prev = nullptr; // Set prev of new head to nullptr
            }
            else
            {
                tail = nullptr; // List is now empty, so tail is nullptr
            }
            delete temp;
            counter--;
            cout << "Node at Position " << pos << " Deleted Successfully." << endl;
            system("pause");
            return;
        }

        Node *current = head;
        int index = 1;
        while (current != nullptr && index < pos) // Traverse to the position
        {
            current = current->next;
            index++;
        }

        if (current == nullptr) // If position not found
        {
            cout << "Invalid Position." << endl;
            system("pause");
            return;
        }

        if (current->next != nullptr)
        {
            current->next->prev = current->prev; // Update prev of next node
        }
        else
        {
            tail = current->prev; // Update tail if deleting the last node
        }
        current->prev->next = current->next; // Bypass the node
        delete current;
        counter--;
        cout << "Node at Position " << pos << " Deleted Successfully." << endl;
        system("pause");
    }

    void delete_Value() // Function to delete a node by value
    {
        int val;
        cout << "Enter Value for Deletion: ";
        cin >> val;

        if (head == nullptr) // If list is empty
        {
            cout << "List is Empty." << endl;
            system("pause");
            return;
        }

        Node *current = head;

        while (current != nullptr && current->data != val) // Search for the value
        {
            current = current->next;
        }

        if (current == nullptr) // If value not found
        {
            cout << "Value " << val << " Not Found in the List." << endl;
            system("pause");
            return;
        }

        if (current->prev != nullptr) // If not the head
        {
            current->prev->next = current->next; // Update next of previous node
        }
        else // If it is the head
        {
            head = current->next; // Update head
            if (head != nullptr)
            {
                head->prev = nullptr; // Set prev of new head to nullptr
            }
            else
            {
                tail = nullptr; // List is now empty
            }
        }

        if (current->next != nullptr) // If not the last node
        {
            current->next->prev = current->prev; // Update prev of next node
        }
        else
        {
            tail = current->prev; // Update tail if deleting the last node
        }

        delete current;
        counter--;
        cout << "Value " << val << " Deleted Successfully." << endl;
        system("pause");
    }

    void insert_before_value() // Insert a new node before the first node with a specified value
    {
        int x, new_val;
        cout << "Enter the value to insert before: ";
        cin >> x;
        cout << "Enter the new value: ";
        cin >> new_val;

        if (head == nullptr) // If list is empty
        {
            cout << "List is Empty. Cannot insert." << endl;
            system("pause");
            return;
        }

        Node *current = head;
        while (current != nullptr && current->data != x) // Search for the value x
        {
            current = current->next;
        }

        if (current == nullptr) // If value x is not found
        {
            cout << "Value " << x << " Not Found in the List." << endl;
            system("pause");
            return;
        }

        Node *newNode = new Node(); // Create a new node
        newNode->data = new_val;
        newNode->next = current;       // Link new node to current
        newNode->prev = current->prev; // Link new node's prev to current's prev

        if (current->prev != nullptr) // If current is not the head
        {
            current->prev->next = newNode; // Update previous node's next
        }
        else // If current is the head
        {
            head = newNode; // Update head to new node
        }
        current->prev = newNode;      // Update current's prev to new node
        if (newNode->next == nullptr) // If new node is now the last
        {
            tail = newNode; // Update tail
        }
        counter++; // Increment counter
        cout << "New Node Inserted Before Value " << x << "." << endl;
        system("pause");
    }

    void delete_before_value() // Function to delete the node before the first node with a specified value
    {
        int x;
        cout << "Enter the value before which to delete: ";
        cin >> x;

        if (head == nullptr) // If list is empty
        {
            cout << "List is Empty. Cannot delete." << endl;
            system("pause");
            return;
        }

        Node *current = head;
        while (current != nullptr && current->data != x) // Search for the value x
        {
            current = current->next;
        }

        if (current == nullptr) // If value x is not found
        {
            cout << "Value " << x << " Not Found in the List." << endl;
            system("pause");
            return;
        }

        if (current->prev == nullptr) // If the node with x is the head
        {
            cout << "No node before the first occurrence of " << x << "." << endl;
            system("pause");
            return;
        }

        Node *nodeToDelete = current->prev; // Node to delete is the one before current

        if (nodeToDelete->prev != nullptr) // If the node to delete is not the head
        {
            nodeToDelete->prev->next = nodeToDelete->next; // Update previous node's next
        }
        else // If the node to delete is the head
        {
            head = nodeToDelete->next; // Update head
            head->prev = nullptr;      // Set new head's prev to nullptr
        }
        nodeToDelete->next->prev = nodeToDelete->prev; // Update next node's prev
        if (nodeToDelete == tail)                      // If the node to delete was the tail
        {
            tail = nodeToDelete->prev; // Update tail
        }
        delete nodeToDelete; // Delete the node
        counter--;           // Decrement counter
        cout << "Node Before Value " << x << " Deleted Successfully." << endl;
        system("pause");
    }

    void display_reverse() // Function to display the list in reverse order
    {
        if (head == nullptr) // If list is empty
        {
            cout << "\nList is Empty." << endl;
            system("pause");
            return;
        }

        Node *current = tail; // Start from tail for reverse display
        cout << "\n\tDoubly List in Reverse:" << endl;
        while (current != nullptr) // Traverse backwards
        {
            cout << current->data << endl;
            current = current->prev;
        }
        cout << "End of Reverse List." << endl;
        system("pause");
    }

    void count_Nodes() // Function to count and display the number of nodes
    {
        cout << "\nNumber of Nodes: " << counter << endl;
        system("pause");
    }

    void display_list() // Function to display the entire list
    {
        if (head == nullptr) // If list is empty
        {
            cout << "\nList is Empty." << endl;
            system("pause");
            return;
        }
        else
        {
            cout << "\n\tDoubly List" << endl;
            Node *temp = head;
            while (temp != nullptr) // Traverse and print each node
            {
                cout << temp->data << endl;
                temp = temp->next;
            }
            cout << "End of List." << endl;
        }
        system("pause");
    }

    void reverse_Iterative() // Function to reverse the list iteratively
    {
        Node *current = head;
        Node *temp = nullptr;

        while (current != nullptr)
        {
            temp = current->prev;          // Save prev
            current->prev = current->next; // Swap prev and next
            current->next = temp;          // Complete swap
            current = current->prev;       // Move to the original next (now prev)
        }

        if (temp != nullptr)
        {
            head = temp->prev;            // Update head
            tail = head;                  // Reset tail to original head (now last)
            while (tail->next != nullptr) // Traverse to new tail
            {
                tail = tail->next;
            }
        }
        else
        {
            tail = nullptr; // List is empty
        }

        cout << "\nList successfully reversed Iteratively." << endl;
        display_list();
    }

    Node *reverse_Helper(Node *current) // Helper function for recursive reversal
    {
        if (current == nullptr) // Base case
            return nullptr;

        // Swap next and prev
        Node *temp = current->next;    // Store original next pointer
        current->next = current->prev; // Swap next to previous node
        current->prev = temp;          // Swap prev to original next

        if (current->prev == nullptr) // If this is the new head
        {
            tail = current; // Set tail to current (now last node)
            return current; // Return the new head
        }

        return reverse_Helper(current->prev); // Recurse with the original next
    }

    void reverse_Recursively() // Function to reverse the list recursively
    {
        head = reverse_Helper(head); // Get the new head
        cout << "\nList successfully reversed recursively." << endl;
        display_list();
    }
};

int main()
{
    DoublyList list1; // Create an instance of DoublyList

    while (1) // Infinite loop for menu
    {
        system("cls"); // Clear screen

        cout << "***    Doubly Linked List    ***\n"
             << "1. Add Node at Beginning.\n"
             << "2. Add Node at End.\n"
             << "3. Add Node at Specific Location.\n"
             << "4. Delete Node from Specific Location.\n"
             << "5. Delete Value from Index.\n"
             << "6. Count Nodes.\n"
             << "7. Display list.\n"
             << "8. Display Iterative Reverse.\n"
             << "9. Display Recursive Reverse.\n"
             << "10. Display List in Reverse.\n"
             << "11. Insert Before Value.\n"
             << "12. Delete Before Value.\n"
             << "0. Exit\n"
             << "\nEnter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Ignore newline

        switch (choice)
        {
        case 1:
            list1.add_At_Beginning();
            break;
        case 2:
            list1.add_At_End();
            break;
        case 3:
            list1.add_At_Position();
            break;
        case 4:
            list1.delete_By_Position();
            break;
        case 5:
            list1.delete_Value();
            break;
        case 6:
            list1.count_Nodes();
            break;
        case 7:
            list1.display_list();
            break;
        case 8:
            list1.reverse_Iterative();
            break;
        case 9:
            list1.reverse_Recursively();
            break;
        case 10:
            list1.display_reverse();
            break;
        case 11:
            list1.insert_before_value();
            break;
        case 12:
            list1.delete_before_value();
            break;
        case 0:
            exit(0);
        default:
            cout << "Invalid choice" << endl;
            system("pause");
            break;
        }
    }
    return 0;
}
