#include <iostream>
using namespace std;
int main ()
{
	//Declaring required variables
	float Length, Width, Area ;
	
	//Taking Length and Width from user
	cout << "Enter Length of the Rectangle " << endl ;
	cin >> Length ;
	
	cout << "Enter Width of the Rectange " << endl ;
	cin >> Width ;
	
	//Calculating the Area by given formula
	Area = Length * Width ;
	cout << "The Area of Rectange is " << Area << endl ;
	
	return 0 ;
}