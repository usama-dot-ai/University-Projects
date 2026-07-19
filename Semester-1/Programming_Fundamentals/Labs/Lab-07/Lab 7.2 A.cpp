
// USAMA ARSHAD

#include <iostream>
using namespace std;

int main()
	{
		
	float average; // holds the grade average
	
	// Taking average from user
	cout << "Input your average:" << endl;
	cin >> average;
	
	// Showing category accprding to average
	if ( average > 100)
		cout << "Enter Valid Average ( 1 - 100 ) " << endl ;
	
	else if ( average >= 90)
		cout << "You are in A category " << endl ;
		
	else if ( average >= 80)
		cout << "You are in B category " << endl ;
		
	else if ( average >= 70)
		cout << "You are in C category " << endl ;
		
	else if (average >= 60)
		cout << "You Pass" << endl;
	
	else
		cout << "You Fail" << endl;
	
	return 0;
}
