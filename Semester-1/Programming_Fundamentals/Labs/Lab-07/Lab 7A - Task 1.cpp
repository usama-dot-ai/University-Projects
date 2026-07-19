#include <iostream>
using namespace std;
int main ()
{
	
	// Deaclaration of variables
	float bill_1, bill_2, bill_3, bill_4, bill_avg ;
	
	// Taking Bill from user
	cout << "Enter your Bill for 1st Quarter " << endl;
	cin >> bill_1 ;
	cout << "Enter Your Bill for 2nd Quarter " << endl;
	cin >> bill_2 ;
	cout << "Enter your Bill for 3rd Quarter " << endl;
	cin >> bill_3 ;
	cout << "Enter your Bill for 4th Quarter " << endl;
	cin >> bill_4 ;
	
	// Calculating average of Bills
	bill_avg = ( bill_1 + bill_2 + bill_3 + bill_4 ) / 12 ;
	cout << "The Average of all Bills = " << bill_avg << endl;
	
	// Deploying Conditions for Water usage
	if ( bill_avg >= 75)
	{
		cout << "You are using excessive Amount of Water " << endl;
	}
	else if ( bill_avg >= 25 && bill_avg < 75 )
	{
		cout << "You are using typical Amount of Water " << endl;
	}
	else
	{
		cout << "We appreciate you Conserve Water" << endl;
	}	
	return 0 ;
}