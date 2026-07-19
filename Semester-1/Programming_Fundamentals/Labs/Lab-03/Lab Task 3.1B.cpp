#include <iostream>
#include <string>
using namespace std;

int main()

{
	//declaration of variables
	
	string firstname, lastname, address, city, state, zip, phone;
	
	// taking input from user
	
	cout << "Enter your first and lastname " << endl;
	cin >> firstname >> lastname;
	
	cout << "Enter your address " << endl;
	cin >> address;
	
	cout << "Enter your city, state and zip code " << endl;
	cin >> city >> state >> zip;
	
	cout << "Enter your phone number " << endl;
	cin >> phone;
	
	//printing output of received data
	
	cout << endl << firstname << " " << lastname << endl;
	cout << address << endl;
	cout << city << state << zip << endl;
	cout << phone << endl;
		
	return 0;
	
}