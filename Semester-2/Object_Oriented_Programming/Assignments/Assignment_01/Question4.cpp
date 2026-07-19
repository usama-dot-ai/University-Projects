#include <iostream>
#include <string>

using namespace std;

class Book 
{
public:
    string title;
    string author;
    float price;
};

void displayBookDetails(const Book &b) 
{
    cout << "\nBook Details:\n";
    cout << "Title: " << b.title << endl;
    cout << "Author: " << b.author << endl;
    cout << "Price: $" << b.price << endl;
    cout << endl;
}

int main() {

    Book myBook;    // Book Object

    // Take input from the user
    cout << "Enter book title: ";
    getline(cin, myBook.title);
    
    cout << "Enter author name: ";
    getline(cin, myBook.author);
    
    cout << "Enter price: ";
    cin >> myBook.price;
    while (myBook.price < 0)
    {
        cout << "Enter Valid Price: ";
        cin >> myBook.price;
    }
    
    displayBookDetails(myBook);

    return 0;
}