#include <iostream>
#include <iomanip>
using namespace std ;
int main ()
{
	//declaration of variables
	float g1, g2, g3, avg ;
	
	//Taking grades from user
	cout << "Enter first Grade " << endl ;
	cin >> g1 ;
	cout << "Enter second Grade " << endl ;
	cin >> g2 ;
	cout << "Enter third Grade " << endl ;
	cin >> g3 ;
	
	//Calculating the average
	avg = ( g1 + g2 + g3 ) / 3 ;
	
	//Setting precision for upto  two decimal places
	cout << fixed << setprecision (2) ;
	
	//showing output
	cout << "The Average of Grades is " << avg << endl ;
	
	return 0 ;
}