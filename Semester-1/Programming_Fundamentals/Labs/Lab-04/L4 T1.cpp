#include <iostream>
using namespace std;
int main()
{
	//Declaring different data types with variables
	int i;
	float f;
	double d;
	char c;
	bool b;
	
	//Taking values from user
	cout<<"Enter an Integeral value " <<endl;
	cin >> i;
	cout << "Enter a Floating Value " <<endl;
	cin >> f;
	cout << "Enter a double value " <<endl;
	cin >> d;
	cout << "Enter a Character " <<endl;
	cin >> c;
	cout << "Enter a bool value " <<endl;
	cin >> b;
	
	//printing out the values
	cout << endl << "Integer is " << i <<endl ;
	cout << "Float is " << f << endl ;
	cout << "Double is " << d << endl;
	cout << "Character is " << c << endl;
	cout << "Bool is " << b << endl;
	
	return 0;
}