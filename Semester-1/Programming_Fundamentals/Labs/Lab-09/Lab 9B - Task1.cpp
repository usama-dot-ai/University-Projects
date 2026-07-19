// This program illustrates the use of a sentinel in a while loop.
// The user is asked for monthly rainfall totals until a sentinel
// value of -1 is entered. Then the total rainfall is displayed.
// USAMA ARSHAD

#include <iostream>
using namespace std;

int main()
{
	//define and initialize to 1 the variable month
	float total = 0, rain;
	int month = 1;
	
	cout << "Enter the total rainfall for months " << endl;
	cout << "Enter -1 when you are finished" << endl;
	
	// while rain does not equal -1
	while (rain != -1)
	{
		cout << "\nEnter the total rainfall in inches for month "
		 << month << endl;
		 cin >> rain;
		 
		if (rain > 0 )
		
			total += rain ;
		
		else if ( rain = -1 )
		
			cout << endl << "Exiting " << endl ;
		
		else
		
			cout << "Kindly Enter Valid rainfall in positive number" << endl;
		
		month ++ ;
	}
	
	if ( month == 1 )
		
		cout << "No data has been entered" << endl;
	
	else
		
		cout << "\nThe total rainfall for the entered months is "<< total << " inches." << endl;
	
	return 0;
}
