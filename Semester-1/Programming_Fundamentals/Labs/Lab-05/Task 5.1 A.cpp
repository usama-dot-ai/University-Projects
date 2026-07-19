#include <iostream>
#include <cmath>
using namespace std;
int main ()
{
	//Declaring variables
	float base, hyp, per ;
	
	//Taking input from user
	cout << "Enter Base of Triangle " << endl ;
	cin >> base ;
	cout << "Enter perpendicular of Triangle " << endl ;
	cin >> per ;
	
	//Showing the Data user entered
	cout << "The Base and perpendicular are " << base 
	<< " , " << per << endl ;
	
	//Using Pythagoras Formula
	hyp = sqrt ( pow ( base , 2 ) + pow ( per , 2 ) ) ;
	cout << "The hypotenuse of Trianlge is " << hyp << endl ;
	return 0 ;
}