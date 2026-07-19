#include <iostream>
#include <iomanip>
using namespace std;

float convert (float a); //function prototype

int main () //main function
{
	float speed_1,speed_2 ; // variables for storing input from user
	
	cout << "Enter First Speed in Kilometers. " << endl;
	cin >> speed_1 ;
	
	while (speed_1 < 0) // validating input
	{
		cout << "Kindly Enter Speed Greater than or equal to 0." << endl;
		cin >> speed_1;
	}
	
	cout << "Enter Last Speed in Kilometers. " << endl;
	cin >> speed_2;
	
	while(speed_2 < speed_1) // validating input
	{
		cout << "Kindly Enter Last Speed Greater Than First Speed." << endl;
		cin >> speed_2;
	}
	
	cout << fixed << setprecision (1) ;
	cout << endl;
	cout << "KPH                MPH " << endl;	
	
	for (int i = speed_1 ; i <= speed_2 ; i = i+5 ) // loop for displaying the conversion
	{
		cout << i << "                 " << convert (i)<< endl;
	}
	return 0;
}
float convert (float a) // function for conversion
{
	float miles = a * 0.6214;
	return  miles;
}