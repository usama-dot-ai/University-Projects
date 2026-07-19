#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node *next;
    Node *prev;
};

class CircularList
{
private:
    Node *head = nullptr;
    Node *tail = nullptr;

public:
    Node *createNode()
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

    void add_At_Beginning()
    {
        Node *newNode = createNode();

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            newNode->next = head;
            newNode->prev = tail;
        }
        else
        {
            newNode->next = head;
            newNode->prev = tail;
            head->prev = newNode;
            tail->next = newNode;
            head = newNode;
        }
        cout << "New Node Added at Beginning." << endl;
        system("pause");
    }

    void add_At_End()
    {
        Node *newNode = createNode();

        if (tail == nullptr)
        {
            head = newNode;
            tail = newNode;
            newNode->next = head;
            newNode->prev = tail;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            tail->next = head;
            head->prev = tail;
        }
        cout << "New Node Added at End." << endl;
        system("pause");
    }

    void delete_At_First()
    {
        if (head == nullptr)
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
            head = head->next;
            tail->next = head;
            head->prev = tail;
        }

        delete temp;
        cout << "Node deleted at first." << endl;
        system("pause");
    }

    void delete_At_Last()
    {
        if (tail == nullptr)
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
            tail = tail->prev;
            tail->next = head;
            head->prev = tail;
        }

        delete temp;
        cout << "Node deleted at last." << endl;
        system("pause");
    }

    void displayList()
    {
        if (head == nullptr)
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
        } while (current != head);
        cout << endl;
    }
};

int main()
{
    CircularList list;
    int choice;

    while (true)
    {
        cout << "\n1. Add at Beginning\n"
             << "2. Add at End\n"
             << "3. Delete at First\n"
             << "4. Delete at Last\n"
             << "5. Display List\n"
             << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            list.add_At_Beginning();
            break;
        case 2:
            list.add_At_End();
            break;
        case 3:
            list.delete_At_First();
            break;
        case 4:
            list.delete_At_Last();
            break;
        case 5:
            list.displayList();
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}