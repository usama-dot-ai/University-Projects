#include <iostream>
using namespace std;
int main()
{
	//Declaring Variables
	char c, d;
	int i;
	
	//Taking input from user
	cout << "Enter Any Character " << endl ;
	cin >> c ;
	
	//Printing ASCII code
	cout << "ASCII value of character is " 
	<< int ( c ) << endl ;
	
	//Converting ASCII code to Character
	cout << "\n Enter ASCII code of any Character " << endl ;
	cin >> i ;
	d = i ;
	cout << "The Character of entered ASCII value is " << d << endl ;
	
	return 0 ;
}