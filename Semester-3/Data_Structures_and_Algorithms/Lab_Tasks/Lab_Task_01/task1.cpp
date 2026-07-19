#include <iostream>
using namespace std;

// Simple dynamic array-based list implementation
class arrayList
{
private:
    int *arr;     // Pointer to the array
    int capacity; // Maximum capacity of the array
    int size;     // Current number of elements

public:
    // Constructor: Initializes the array with given capacity
    arrayList(int cap)
    {
        capacity = cap;
        size = 0;
        arr = new int[capacity];
    }

    // Checks if the list is empty
    int isEmpty()
    {
        return size == 0;
    }

    // Checks if the list is full
    int isFull()
    {
        return size == capacity;
    }

    // Inserts a value at the end if not full
    void insert(int val)
    {
        if (!isFull())
        {
            arr[size] = val;
            size++;
        }
        else
        {
            cout << "Array is Full. " << endl;
        }
    }

    // Removes the last element if not empty
    void remove()
    {
        if (!isEmpty())
        {
            size--;
        }
        else
        {
            cout << "Array is Empty. " << endl;
        }
    }

    // Displays all elements in the list
    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }
    }
};

// Main function: Demonstrates the arrayList usage
int main()
{
    arrayList a(5); // Create list with capacity 5
    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.insert(4);
    a.insert(5); // Insert 5 elements

    a.remove();  // Remove the last element
    a.display(); // Display remaining elements

    return 0;
}
