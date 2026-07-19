#include <iostream>
using namespace std;
int main ()
{
	int Add, Sub, Mul, Div, int1, int2 ;
	// Taking integers from user
	cout << "Enter two Integers " << endl ;
	cin >> int1 >> int2 ;
	
	// Performing Addition
	Add = int1 + int2 ;
	cout << "The Addition of Integers is " << Add << endl ;
	
	// Performing Subtraction
	Sub = int1 - int2 ;
	cout << "The Subtraction of Integers is " << Sub << endl ;
	
	// Performing Multiplication
	Mul = int1 * int2 ;
	cout << "The Multiplication of Integers is " << Mul << endl ;
	
	// Performing Division and handling with Zero
	
	if ( int2 == 0)
	{
		
		cout << "The Division is not Possible by Zero " << endl ;
	}
	else
	{
		Div = int1 / int2 ;
		cout  << "The Division of Intergers is " << Div << endl ;
	}
	return 0;
}