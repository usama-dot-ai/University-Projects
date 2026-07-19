#include <iostream>
using namespace std;
int main()
{
	//Declaring variables
	double celsius, fahrenheit ;
	
	//Taking temperature from user
	cout << "Enter Temperature in Celsius " << endl ;
	cin >> celsius ;
	
	//Calculating temperature in Fahrenheit
	fahrenheit = ( celsius *  9 / 5 ) + 32 ;
	cout << "The Temperature in Fahrenheit is " 
		<< fahrenheit << endl ;
	
	return 0 ;
}