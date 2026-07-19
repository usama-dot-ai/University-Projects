#include <iostream>
using namespace std;

class Node
{
public:
    int rollNo;
    string name;
    Node *next;
};
class List
{
private:
    Node *head;

public:
    List() { head = nullptr; }
    Node *createNode()
    {
        int rollNo;
        cout << "Enter Roll No: ";
        cin >> rollNo;
        string name;
        cout << "Enter Name: ";
        cin >> name;

        Node *newNode = new Node();
        newNode->rollNo = rollNo;
        newNode->name = name;
        newNode->next = nullptr;
        return newNode;
    }
    void insertAtFirst()
    {
        Node *newNode = createNode();
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }
    void insertAtLast()
    {
        Node *newNode = createNode();
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node *temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    void removeFromFirst()
    {
        if (head == nullptr)
        {
            cout << "List is Empty. " << endl;
            system("pause");
            return;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
    void removeFromLast()
    {
        if (head == nullptr)
        {
            cout << "List is Empty. " << endl;
            system("pause");
            return;
        }
        else
        {
            Node *temp = head;
            while (temp->next->next != nullptr)
            {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
    }
    void displayAll()
    {
        if (head == nullptr)
        {
            cout << "List is Empty. " << endl;
            system("pause");
            return;
        }
        else
        {
            Node *temp = head;
            while (temp != nullptr)
            {
                cout << temp->rollNo << " + " << temp->name << " -> ";
                temp = temp->next;
            }
            cout << "Null ptr. " << endl;
            system("pause");
        }
    }
};
int main()
{
    int num;
    List list1;
    while (1)
    {
        system("cls");
        cout << "   Link List           " << endl;
        cout << "                       " << endl;
        cout << "1. Insert at First     " << endl;
        cout << "2. Insert at Last      " << endl;
        cout << "3. Remove from First   " << endl;
        cout << "4. Remove from Last    " << endl;
        cout << "5. Display all Nodes   " << endl;
        cout << "6. Exit the Program    " << endl;
        cout << "                       " << endl;
        cout << "                       " << endl;
        cout << "Enter Your Choice: ";
        cin >> num;
        switch (num)
        {
        case 1:
            list1.insertAtFirst();
            break;
        case 2:
            list1.insertAtLast();
            break;
        case 3:
            list1.removeFromFirst();
            break;
        case 4:
            list1.removeFromLast();
            break;
        case 5:
            list1.displayAll();
            break;
        case 6:
            return -1;
        default:
            break;
        }
    }

    return 0;
}