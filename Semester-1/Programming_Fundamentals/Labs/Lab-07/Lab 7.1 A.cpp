#include <iostream>
using namespace std;

// USAMA ARSHAD

#include <iostream>
using namespace std;
int main( )
{
	// Declaring two variables
	int num1, num2 ;
	
	// Taking integers from user
	cout << "Enter two integers to check equality " << endl;
	cin >> num1 >> num2 ;

	cout << "num1 = " << num1 << " and num2 = " << num2 << endl;

	// Checking the equality
	if (num1 == num2)
	{
	
		cout << endl << "The Values are same " << endl ;
		cout << "Hey, that's a coincidence!" << endl;
	}
	// Handling the inequality
	else
	{
	cout << endl << "The values are not the same" << endl;
	}
	return 0;
}