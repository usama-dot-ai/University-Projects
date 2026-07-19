#include <iostream>
#include <iomanip>
using namespace std;
int main ()
{
	//Declaring variables
	float Tsales, STtax, LCtax, StateTax, LocalTax ;
	
	// Taking total sales and tax percentage from user
	cout << "Enter Total Sale of Month " << endl ;
	cin >> Tsales ;
	cout << "Enter Percentage of State Tax in Fractions " << endl ;
	cin >> STtax ;
	cout << "Enter Percentage of Local Tax in Fractions " << endl ;
	cin >> LCtax ;
	
	cout << fixed << setprecision (2) ;
	cout << endl << "The Monthly Sale is " << Tsales << endl ;
	
	// Calculating the State Tax
	StateTax = STtax * Tsales ;
	cout << "The State Tax for Monthly Sale is " << StateTax << endl ;
	
	// Calculating the Local Tax
	LocalTax = LCtax * Tsales ;
	cout << "The Local Tax for Monthly sale is " << LocalTax << endl ;
	
	return 0 ; 
}