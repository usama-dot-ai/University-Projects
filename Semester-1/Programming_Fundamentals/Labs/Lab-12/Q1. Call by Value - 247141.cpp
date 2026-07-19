#include <iostream>
using namespace std;

void SwapByValue (int a, int b); //function prototype

int main ()
{
	int num1, num2; //variables for storing integers
	
	cout << "Enter First Integer: " ; //taking integers from user
	cin >> num1;
	cout << "Enter Second Integer: " ;
	cin >> num2;
	
	//displaying output before swapping
	cout << "\nValues in Main Before Swapping: " <<
	endl << "First Integer = " << num1 << " Second Integer = " << num2 << endl;
	
	SwapByValue (num1, num2); //calling function
	
	//displaying output after swapping by value
	cout << "\nValues in Main After Swapping: " <<
	endl << "First Integer = " << num1 << " Second Integer = " << num2 << endl;
	
	return 0;
}

void SwapByValue (int a, int b) // function for swapping by value
{
	int temp = a;
	a = b;
	b = temp;
}