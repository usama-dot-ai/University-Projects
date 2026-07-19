#include <iostream>
using namespace std;
int main ()
{
	// declaring varuables
	int pay, exc_amnt1, exc_amnt2, exc_amnt3, exc_amnt4, exc_amnt5, exc_amnt6 ;
	float tax ;
	
	// Taking Annual salary from user
	cout << "Enter your annual salary " << endl ;
	cin >> pay ;
	
	// Calculation of Tax
	if ( pay >= 600000 && pay < 1200000 )
	{
		exc_amnt1 = pay - 600000 ;
		tax = exc_amnt1 * ( 2.5 / 100 ) ;
	}
	else if ( pay >= 1200000 && pay <= 2400000 )
	{
		exc_amnt2 = pay - 1200000 ;
		tax = ( exc_amnt2 *  12.5 / 100 ) + 15000 ;
	}
	else if ( pay >= 2400000 && pay <= 3600000 )
	{
		exc_amnt3 = pay - 2400000 ;
		tax = ( exc_amnt3 * 20 / 100 )  + 165000 ;
	}
	else if ( pay >= 3600000 && pay <= 6000000 )
	{
		exc_amnt4 = pay - 3600000 ;
		tax = ( exc_amnt4 *  25 / 100 ) + 405000 ;
	}
	else if ( pay >= 6000000 && pay<= 12000000 )
	{
		exc_amnt5 = pay - 6000000 ;
		tax = ( exc_amnt5 *  32.5 / 100 ) + 1005000 ;
	}
	else if ( pay >= 12000000 )
	{
		exc_amnt6 = pay - 12000000 ;
		tax = ( exc_amnt6 * 35 / 100) + 2955000 ;
	}
	else 
	{
		cout << "ENJOY ! You are not eligible to pay Tax " << endl ;
	}
	cout << endl << "Your Annual Income tax is " << tax << endl ;
	return 0 ;
}