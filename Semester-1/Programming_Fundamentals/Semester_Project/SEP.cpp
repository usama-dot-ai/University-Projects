#include <iostream>
#include <cctype>
#include <regex>
#include <string>
#include <fstream>  // For file handling
using namespace std;

int acc_number = 0;
int initial_deposit = 0;
int balance = 0;
string full_name;
int deposit = 0;
int withdrawl_amount = 0;
int balance_after_withdrawl = 0;

// File to store bank account information
ofstream bank_file("bank_account.txt", ios::app); // Open file for appending

// Function to write to file
void write_to_file(const string& action) 
{
    if (bank_file.is_open()) 
	{
        bank_file << action << endl;  // Write action to file
    } 
	else 
	{
        cout << "Error opening file!" << endl;  // File error
    }
}

// Creating new Accounts with Unique ID number
void account_creation() 
{
    // Asking for Full Name
    while (true) 
	{
        cout << "Enter Full Name: ";
        cin.ignore();
        getline(cin, full_name);  // Get full name

        // Check if the input is empty
        if (full_name.empty()) 
		{
            cout << "Name cannot be empty. Please try again." << endl; // Name empty check
            continue;
        }

        // Check if all characters are alphabets or spaces
        bool validName = true;
        for (char c : full_name) 
		{
            if (!isalpha(c) && c != ' ') 
			{  // Name must be alphabet
                validName = false;
                break;
            }
        }

        if (validName) 
		{
            break;  // Valid name entered
        }
        cout << "Invalid Name! Kindly enter only alphabets and spaces." << endl; // Invalid name
    }

    // Asking for Date of Birth
    string date_of_birth;
    while (true) 
	{
        cout << "Enter Date of Birth (DD-MM-YYYY): ";
        cin >> date_of_birth;  // Get date of birth

        // Regular expression to match the format DD-MM-YYYY
        regex datePattern(R"(^\d{2}-\d{2}-\d{4}$)");

        if (regex_match(date_of_birth, datePattern)) 
		{
            break;  // Valid date format
        } else {
            cout << "Invalid Date of Birth! Kindly enter the date in the format DD-MM-YYYY." << endl;  // Invalid date
        }
    }

    // Asking for CNIC Number
    string cnic_no;
    while (true) 
	{
        cout << "Enter 13 Digit CNIC Number: ";
        cin >> cnic_no;  // Get CNIC number

        if (cnic_no.length() == 13) 
		{  // CNIC length check
            bool allDigits = true;
            for (char c : cnic_no) 
			{
                if (!isdigit(c)) 
				{  // CNIC must be digits
                    allDigits = false;
                    break;
                }
            }
            if (allDigits) 
			{
                break;  // Valid CNIC entered
            }
        }
        cout << "Invalid CNIC Number! Kindly ensure it is 13 digits long and contains only numbers." << endl;  // Invalid CNIC
    }

    // Asking for E-Mail
    string e_mail;
    while (true) 
	{
        cout << "Enter your email address: ";
        cin >> e_mail;  // Get email address

        // Regular expression to validate the email format
        regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

        if (regex_match(e_mail, emailPattern)) 
		{
            break;  // Valid email entered
        } 
		else 
		{
            cout << "Invalid email format. Please enter a valid email address." << endl;  // Invalid email
        }
    }

    // Asking for Contact Number
    string contact_no;
    while (true) 
	{
        cout << "Enter 11 Digit Contact Number: ";
        cin >> contact_no;  // Get contact number

        if (contact_no.length() == 11) 
		{  // Check length of number
            bool allDigits = true;
            for (char c : contact_no) 
			{
                if (!isdigit(c)) 
				{  // Number must be digits
                    allDigits = false;
                    break;
                }
            }
            if (allDigits) {
                break;  // Valid contact number
            }
        }
        cout << "Invalid Contact Number! Kindly ensure it is 11 digits long and contains only numbers." << endl;  // Invalid contact number
    }

    // Asking for Residential Address
    string resident_add;
    while (true) 
	{
        cin.ignore();  // Ignore previous newline character
        cout << "Enter your residential address: ";
        getline(cin, resident_add);  // Get address

        if (resident_add.empty()) {
            cout << "The address cannot be empty. Please enter a valid residential address." << endl;  // Empty address check
        } else {
            break;  // Valid address entered
        }
    }

    // Asking for Account Type
    int acc_type;
    bool validInput = false;

    cout << endl << "Available Account Types:" << endl;
    cout << "1. Current Account\n2. Savings Account\n3. Business Account\n4. Student Account" << endl; // Show account types

    while (!validInput) {
        cout << "Enter your desired Account Type: ";
        cin >> acc_type;  // Get account type

        switch (acc_type) {
            case 1:
                cout << endl << "Thank You For Choosing Current Account. " << endl;  // Current account
                validInput = true;
                break;
            case 2:
                cout << endl << "Thank You For Choosing Savings Account. " << endl;  // Savings account
                validInput = true;
                break;
            case 3:
                cout << endl << "Thank You For Choosing Business Account. " << endl;  // Business account
                validInput = true;
                break;
            case 4:
                cout << endl << "Thank You For Choosing Student Account. " << endl;  // Student account
                validInput = true;
                break;
            default:
                cout << endl << "Invalid choice! Kindly choose from the available account types." << endl;  // Invalid account type
        }
    }

    // Asking for Initial Deposit
    while (true) {
        cout << endl << "Enter your Initial Deposit: ";
        cin >> initial_deposit;  // Get initial deposit

        if (initial_deposit >= 0) {  // Validate deposit amount
            balance = initial_deposit;
            break;  // Valid deposit
        } else {
            cout << "Invalid Deposit Amount! Kindly enter a valid deposit amount." << endl;  // Invalid deposit amount
        }
    }

    // Generating Unique Account Numbers
    static int acc_counter = 1000;
    acc_counter++;  // Increment account counter
    acc_number = acc_counter;

    // Write account details to file
    string action = "\nAccount Created Successfully!\nAccount Number: " + to_string(acc_number) + "\nBalance: " + to_string(balance) + "\n";
    write_to_file(action);  // Write action to file
    cout << action;  // Display action
}

void check_balance() {
    // Prepare balance info for file and display
    string action = "The balance of " + full_name + " (" + to_string(acc_number) + ") is: " + to_string(balance);
    write_to_file(action);  // Write to file
    cout << action << endl;  // Display balance
}

void make_deposit() {
    cout << endl << "Enter the amount you want to deposit: ";
    cin >> deposit;  // Get deposit amount

    balance += deposit;  // Add deposit to balance
    string action = "Deposit Successful! New Balance: " + to_string(balance);
    write_to_file(action);  // Write deposit action to file
    cout << action << endl;  // Display updated balance
}

void withdraw_cash() {
    cout << endl << "Enter the amount you want to withdraw from account " << acc_number << ": ";
    cin >> withdrawl_amount;  // Get withdrawal amount

    while (withdrawl_amount > balance) {
        cout << "Insufficient funds! Enter a lower withdrawal amount: ";
        cin >> withdrawl_amount;  // Check sufficient funds
    }

    while (withdrawl_amount % 500 != 0) {
        cout << "Withdrawal amount must be a multiple of 500. Re-enter: ";
        cin >> withdrawl_amount;  // Withdrawal must be multiple of 500
    }

    balance_after_withdrawl = balance - withdrawl_amount;  // Deduct withdrawal
    balance = balance_after_withdrawl;

    string action = "Withdrawal Successful! Remaining Balance: " + to_string(balance);
    write_to_file(action);  // Log withdrawal action to file
    cout << action << endl;  // Display updated balance
}

void send_money() {
    string receiver_acc;
    int amount;
    cout << endl << "Enter receiver's account number: ";
    cin >> receiver_acc;  // Get receiver's account number
    cout << "Enter the amount you want to send: ";
    cin >> amount;  // Get transfer amount

    if (amount > balance) {
        cout << "Insufficient funds to send money!" << endl;  // Check funds
    } else {
        balance -= amount;  // Deduct transfer amount
        string action = "Money Sent to Account " + receiver_acc + ". Amount: " + to_string(amount) + ". Remaining Balance: " + to_string(balance);
        write_to_file(action);  // Write transaction to file
        cout << action << endl;  // Display transaction success
    }
}

int main() {
    string user_response;

    do {
        cout << "Welcome to the FYP BANK." << endl;
        cout << "Select from the following options: " << endl;
        cout << "1. Create a Bank Account." << endl;
        cout << "2. Balance inquiry." << endl;
        cout << "3. Deposit." << endl;
        cout << "4. Withdraw." << endl;
        cout << "5. Send Money." << endl;
        cout << "6. Exit." << endl;

        int choice;
        cout << "Please enter your choice (1-6): ";
        cin >> choice;  // Get user choice

        if (choice >= 1 && choice <= 6) {
            switch (choice) {
                case 1:
                    account_creation();  // Create account
                    break;
                case 2:
                    check_balance();  // Check balance
                    break;
                case 3:
                    make_deposit();  // Make deposit
                    break;
                case 4:
                    withdraw_cash();  // Withdraw funds
                    break;
                case 5:
                    send_money();  // Send money
                    break;
                case 6:
                    cout << "Exiting.\nThank you for your time." << endl;  // Exit program
                    bank_file.close();  // Close file before exiting
                    return 0;
            }
        } else {
            cout << "Invalid choice! Please select a valid option (1-6)." << endl;  // Invalid choice
        }

        cout << endl << "Would you like to continue (Yes/No): ";
        cin >> user_response;  // Ask to continue

    } while (user_response == "Yes" || user_response == "yes" || user_response == "YES" || user_response == "y" || user_response == "Y");

    bank_file.close();  // Close file before exiting
    return 0;
}
