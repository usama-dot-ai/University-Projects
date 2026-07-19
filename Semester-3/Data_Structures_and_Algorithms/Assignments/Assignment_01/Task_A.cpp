#include <iostream>
using namespace std;

class Node // Node class represents a single element in the singly linked list
{
public:
    int data;
    Node *next;
};

class SinglyList // SinglyList class implements a singly linked list with various operations
{
private:
    Node *head = nullptr;
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
        counter++;

        return newNode;
    }

    void add_At_Beginning() // Function to add a new node at the beginning of the list
    {
        Node *newNode = createNode();

        if (head == nullptr) // If list is empty
        {
            head = newNode; // Set head to new node
        }
        else
        {
            newNode->next = head; // Point new node to current head
            head = newNode;       // Update head to new node
        }
        cout << "New Node Added at Beginning. " << endl;
        system("pause");
    }

    void add_At_End() // Function to add a new node at the end of the list
    {
        Node *newNode = createNode();

        if (head == nullptr) // If list is empty
        {
            head = newNode; // Set head to new node
        }
        else
        {
            Node *current = head; // Start from head

            while (current->next != nullptr) // Traverse to the last node
            {
                current = current->next;
            }
            current->next = newNode; // Link new node at the end
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
            cout << "Playlist is Empty. " << endl;
            system("pause");
            return;
        }
        if (pos < 1 || pos > counter + 1) // Check for invalid position
        {
            cout << "Invalid Position. " << endl;
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

        while (current->next != nullptr && index++ < pos) // Traverse to position-1
        {
            current = current->next;
        }
        Node *temp = createNode();

        temp->next = current->next; // Link new node to next
        current->next = temp;       // Link current to new node

        cout << "Node Added at Desired Location. " << endl;
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
            delete temp;
            counter--;
            cout << "Node at Position " << pos << " Deleted Successfully." << endl;
            system("pause");
            return;
        }

        Node *current = head;
        Node *previous = nullptr;
        int index = 1;

        while (current != nullptr && index < pos) // Traverse to the position
        {
            previous = current;
            current = current->next;
            index++;
        }

        if (current == nullptr) // If position not found
        {
            cout << "Invalid Position." << endl;
            system("pause");
            return;
        }

        previous->next = current->next; // Bypass the node
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
        Node *previous = nullptr;

        while (current != nullptr && current->data != val) // Search for the value
        {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) // If value not found
        {
            cout << "value " << val << " Not Found in the List." << endl;
            system("pause");
            return;
        }

        if (previous == nullptr) // If deleting head
        {
            head = head->next;
        }
        else
        {
            previous->next = current->next;
        }
        delete current;

        cout << "Value " << val << " Deleted Successfully." << endl;
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
            cout << "\n\tSingly List" << endl;
            Node *temp = head;

            while (temp != nullptr) // Traverse and print each node
            {
                cout << temp->data << endl;
                temp = temp->next;
            }
            cout << "End of List. " << endl;
        }
        system("pause");
    }

    void reverse_Iterative() // Function to reverse the list iteratively
    {
        Node *current = head;
        Node *previous = nullptr;
        Node *nextNode = nullptr;

        while (current != nullptr) // Iterate through the list
        {
            nextNode = current->next; // Store next node
            current->next = previous; // Reverse the link
            previous = current;       // Move previous forward
            current = nextNode;       // Move current forward
        }
        head = previous; // Update head to new first node

        cout << "\nList successfully reversed Iterativley. " << endl;
        display_list();
    }

    Node *reverse_Helper(Node *current, Node *previous) // Helper function for recursive reversal
    {
        if (current == nullptr) // Base case: end of list
        {
            return previous;
        }
        Node *nextNode = current->next;           // Store next
        current->next = previous;                 // Reverse link
        return reverse_Helper(nextNode, current); // Recurse
    }

    void reverse_Recursively() // Function to reverse the list recursively
    {
        head = reverse_Helper(head, nullptr); // Call helper with head and null

        cout << "\nList successfully reversed recursively." << endl;
        display_list();
    }
};

int main()
{
    SinglyList list1; // Create an instance of SinglyList

    while (1) // Infinite loop for menu
    {
        system("cls"); // Clear screen

        cout << "***    Singly list    ***\n"
             << "1. Add Node at Beginning.\n"
             << "2. Add Node at End.\n"
             << "3. Add Node at Specific Location. \n"
             << "4. Delete Node from Specific Location.\n"
             << "5. Delete Value from Index.\n"
             << "6. Count Nodes.\n"
             << "7. Display list.\n"
             << "8. Display Iterative Reverse.\n"
             << "9. Display Recursive Reverse.\n"
             << "0. Exit\n"
             << "\nEnter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Ignore newline

        switch (choice) // Handle user choice
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
        case 0:
            exit(0); // Exit program
        default:
            cout << "Invalid choice" << endl;
            system("pause");
            break;
        }
    }
    cout << endl;
    return 0;
}
