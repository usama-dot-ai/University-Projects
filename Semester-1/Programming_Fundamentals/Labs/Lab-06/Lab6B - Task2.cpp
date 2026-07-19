
// USAMA ARSHAD

#include <iostream>
using namespace std;
int main( )
{
	
	int num1, num2;
	
	// Taking numbners from user
	cout << "Enter 1st integer " << endl ;
	cin >> num1;
	cout << "Enter 2nd integer " << endl ;
	cin >> num2 ;
	cout << "num1 = " << num1 << " and num2 = " << num2 << endl;
	
	// Handling if numbers are not equal
	if ( num1 != num2)
	{
		cout << endl << "The numbers are not equal " << endl ;
	}
	
	// Rpinting output if numbers are equal
	if (num1 == num2)
		{
		cout<<"The values are same"<<endl;
		cout<<"Hey that's the coincident"<<endl;
		}
		
	return 0;
}
