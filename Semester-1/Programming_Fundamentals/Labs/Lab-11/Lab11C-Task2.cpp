#include <iostream>
using namespace std;

int series (int a); // Function prototype

int main() // main function
{
	int num; // variable for storing input from user
	cout << "Enter any Number greater than 0. " << endl;
	cin >> num;
	
	while (num < 1) // validating the input
	{
		cout << "Kindly Enter Correct Number: ";
		cin >> num;
	}
	
	int sum = series (num); // calling the function
	return 0; 
}

int series (int a) // Function for calculating the sum
{
	int add; // variable for storing the total sum
	
	for (int i=1 ; i <= a ; i++) // loop for adding the numbers
	{
		 add = add + i; 
	}
	cout << "\nSum of all number from 0 to " << a << " is: " << add;
	return add; //returning the total sum
}