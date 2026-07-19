
// USAMA ARSHAD

#include <iostream>
using namespace std;

int main()
{
	char year;
	float gpa;
	
	cout << "In which year you are studying ?" << endl;
	cout << endl << "Enter 1 (freshman), 2 (sophomore),"
	 "3 (junior), or 4 (senior) " << endl << endl;
	cin >> year;
	
	cout << endl << "Now enter your GPA" << endl;
	cin >> gpa;
	
	if ( gpa >= 2.0 && year == '4')
		cout << endl << "It is time to graduate soon" << endl;
		
	else 
		cout << endl << "You need more schooling " << endl;
		
	return 0;
}