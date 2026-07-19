#include <iostream>
#include <cctype>
#include <regex>
using namespace std;

// Creating new Accounts with Unique ID number
int main()
{
	// Asking for Full Name
    string full_name;
    
    while (true) // Loop until valid input is received for full_name 
    {
        cout << "Enter Full Name: ";
        getline(cin, full_name); // Use getline to allow spaces in input

        // Check if the input is empty
        if (full_name.empty()) 
        {
            cout << "Name cannot be empty. Please try again." << endl;
            continue; // Prompt the user again
        }

        // Check if all characters are alphabets or spaces
        bool validName = true;
        for (char c : full_name) 
        {
            if (!isalpha(c) && c != ' ') 
            {
                validName = false; // Found a non-alphabet character
                break;
            }
        }

        // Validate the input
        if (validName) 
        {
            break; // Exit the loop if input is valid
        } 
        cout << "Invalid Name! Kindly enter only alphabets and spaces." << endl;
    }
    
    // Asking for Date of Birth
	string  date_of_birth;
    
    while (true) // Loop until valid input is received for Date of Birth 
	{
        cout << "Enter Date of Birth (DD-MM-YYYY): ";
        cin >> date_of_birth;

        // Regular expression to match the format DD-MM-YYYY
        regex datePattern(R"(^\d{2}-\d{2}-\d{4}$)");

        // Check if the input matches the date pattern
        if (regex_match(date_of_birth, datePattern)) 
		{
            break; // Exit the loop if the date is valid
        }
		else
		{
            cout << "Invalid Date of Birth! Kindly enter the date in the format DD-MM-YYYY using digits and hyphens only." << endl;
        }
    }
    
    // Asking for CNIC Number
    string cnic_no;
    
    while (true) // Loop until valid CNIC is received
    {
        cout << "Enter 13 Digit CNIC Number: ";
        cin >> cnic_no;

        // Check if the input is exactly 13 characters long and all are digits
        if (cnic_no.length() == 13) 
        {
            bool allDigits = true;
            for (char c : cnic_no) 
            {
                if (!isdigit(c)) 
                {
                    allDigits = false;
                    break;
                }
            }
            // Validate the input
            if (allDigits) 
            {
                break;
            }
        }
        cout << "Invalid CNIC Number! Kindly ensure it is 13 digits long and contains only numbers." << endl;
    }
	
	// Asking for E-Mail
	string e_mail;
	
	while (true) // Loop until valid E-mail is received
	{
        cout << "Enter your email address: ";
        cin >> e_mail;

        // Regular expression to validate the email format
        regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

        // Check if the input matches the email pattern
        if (regex_match(e_mail, emailPattern)) 
		{
            break; // Exit the loop if the email is valid
        } 
		else
		{
            cout << "Invalid email format. Please enter a valid email address." << endl;
        }
    }
    
    // Asking for Contact Number
    string contact_no;
    
    while (true) // loop until valid contact number is received
    {
        cout << "Enter 11 Digit Contact Number: ";
        cin >> contact_no;
        
        // Check if the input is exactly 11 characters long and all are digits
        if (contact_no.length() == 11) 
        {
            bool allDigits = true;
            for (char c : contact_no) 
            {
                if (!isdigit(c)) 
                {
                    allDigits = false;
                    break;
                }
            }
            // Validate the input
            if (allDigits) 
            {
                break;
            }
        }
        cout << "Invalid Contact Number! Kindly ensure it is 11 digits long and contains only numbers." << endl;
    }
    
    // Asking for Residential Address
    string resident_add;
    
	while (true) 
	{
		cin.ignore();
        cout << "Enter your residential address: ";
        getline(cin, resident_add); // Use getline to allow spaces in the address

        // Check if the address is empty
        if (resident_add.empty()) 
		{
            cout << "The address cannot be empty. Please enter a valid residential address." << endl;
        } 
		else 
		{
            break; // Exit the loop if the address is valid
        }
    }
    
    // Asking for Account Type
    int acc_type;
    bool validInput = false; // Flag to track valid input

    cout << "Available Account Types:" << endl;
    cout << "1. Current Account\n2. Savings Account\n3. Business Account\n4. Student Account" << endl;

    while (!validInput) // Loop until valid input is received
	{
        cout << "Enter your desired Account Type: ";
        cin >> acc_type;

        switch (acc_type) // Switch for taking the input of Account Type
		{
            case 1:
                cout << "Thank You For Choosing Current Account. " << endl;
                validInput = true;
                break;
            case 2:
                cout << "Thank You For Choosing Savings Account. " << endl;
                validInput = true; 
                break;
            case 3:
                cout << "Thank You For Choosing Business Account. " << endl;
                validInput = true; 
                break;
            case 4:
                cout << "Thank You For Choosing Student Account. " << endl;
                validInput = true;
                break;
            default:
                cout << "Invalid choice! Kindly choose from the available account types." << endl;
        }
    }
    
    // Asking for Initial Deposit
    string initial_dep;
        
    while (true) // loop until valid contact number is received
    {
        cout << "Enter you Initial Deposit: ";
        cin >> initial_dep;
		
        bool allDigits = true; // validating the initital deposit for digits only
        for (char c : initial_dep) 
        {
            if (!isdigit(c)) 
            {
                allDigits = false;
                break;
            }
        }
        // Validate the input
        if (allDigits) 
        {
            break;
        }
        cout << "Invalid Deposit Amount! Kindly enter only numbers." << endl;
    }
    
    // Generating Unique Account Numbers
    static int acc_counter = 1000;
    acc_counter++;
    string acc_number = "ACC" + to_string(acc_counter);

    // Successful input
    cout << "\nAccount Created Successfully!" << endl;
    cout << "Your Account Number is: " << acc_number << endl;
    
    return 0;
}