#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	// Declaring Variables
	int num, ratio, no_terms, series;
	
	// Taking values from user
	cout << "Enter starting number for Geometric series. " << endl;
	cin >> num;
	cout << "Enter the common ratio. " << endl;
	cin >> ratio;
	cout << "Enter No. of Terms. " << endl;
	cin >> no_terms;
	
	cout << "\nThe Geometric Series is " << endl;
	
	// Using for loop for series
	for ( int i = 0 ; i < no_terms ; i++ )
	{
		series = num * pow ( ratio, i ) ;
		cout << endl<< series ;
	}
	return 0;
}