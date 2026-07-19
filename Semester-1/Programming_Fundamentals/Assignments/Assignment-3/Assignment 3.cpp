#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct BookStore // Structure to store Books info
{
    int id;
    string title;
    string author;
    float price;
};

void addBook(const BookStore& book) // Function to add books 
{
    ofstream outfile("Book Store.txt"); // writing in file
    if (outfile.is_open()) // checking if file is open
	{
        outfile << book.id << "\n" << book.title << "\n" << book.author << "\n" << book.price << "\n";
        outfile.close();
        cout << "Book added successfully!\n";
    } 
	else // showing error
	{
        cout << "Error opening file for writing.\n";
    }
}

void displayBooks() // function to display books in record
{
    ifstream infile("Book Store.txt"); // reading from file
    if (infile.is_open()) // chacking if file is open
	{
		// setting width of outpput
        cout << left << setw(10) << "Book ID" << setw(30) << "Title" 
		<< setw(20) << "Author" << setw(10) << "Price" << "\n";
        cout << string(70, '-') << "\n";

        while (true) // valiadting if file is open
		{
            BookStore book;
            infile >> book.id; 
            infile.ignore(); // ignore the newline after the ID
            if (infile.eof()) break; // Break if end of file is reached
            getline(infile, book.title);
            getline(infile, book.author);
            infile >> book.price;
            infile.ignore(); // Ignore the newline after the price

            // displaying the output
			cout << left << setw(10) << book.id << setw(30) << book.title 
            << setw(20) << book.author << setw(10) << book.price << "\n";
        }
        infile.close(); // closing file
    } 
	else // showing error if occures
	{
        cout << "Error opening file for reading.\n";
    }
}

void searchBook(int id) // function to search book from inventory
{
    ifstream infile("Book Store.txt"); // reading from file
    if (infile.is_open()) // chaking if file is open
	{
        while (true) // validating if file is open
		{
            BookStore book;
            infile >> book.id;
            infile.ignore(); // Ignore the newline after the ID
            if (infile.eof()) break; // Break if end of file is reached
            getline(infile, book.title);
            getline(infile, book.author);
            infile >> book.price;
            infile.ignore(); // Ignore the newline after the price

            if (book.id == id) // comparing the ID
			{	
			// displaying the book found
                cout << "Book found:\n";
                cout << "Book ID: " << book.id << "\n" << "Title: " << book.title << "\n"
            	<< "Author: " << book.author << "\n" << "Price: " << book.price << "\n";
                infile.close(); // closing the file
                return; // retuning to main
            }
        }
        // showing that book with searched ID does not exists
        cout << "Book with ID " << id << " not found.\n";
        infile.close(); // closing file
    } 
	else // showing error if any
	{
        cout << "Error opening file for reading.\n";
    }
}

int main() // main function
{
    int choice; // variable for switch
    do 
	{	// inventory Menu
        cout << "\nBook Store Inventory System\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book by ID\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
		{
            case 1: 
			{
                BookStore newBook;

                cout << "Enter Book ID: "; 
                cin >> newBook.id;
                cin.ignore(); // Ignore the newline after the ID

                cout << "Enter Title: ";
                getline(cin, newBook.title);

                cout << "Enter Author: ";
                getline(cin, newBook.author);

                cout << "Enter Price: ";
                cin >> newBook.price;
                addBook(newBook); // calling function to add new books
                break;
            }
            case 2:
                displayBooks(); // calling function to display books in inventory
                break;
            case 3: 
			{
                int searchId;
                
                cout << "Enter Book ID to search: ";
                cin >> searchId;
                
				searchBook(searchId); // calling function to search books from inventory
                break;
            }
            case 4:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } 
	while (choice != 4);

    return 0;
}
