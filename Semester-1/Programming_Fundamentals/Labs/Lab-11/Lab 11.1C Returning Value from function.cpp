#include <iostream>
#include <cmath>
using namespace std;

float getSqrt(int); // function prototype

int main() // main function
{
	int num; //variable for storing number
	cout << "Enter Number to Find Square Root. " << endl;
	cin >> num;
	
	float sqrt; // storing square root in a variable
	sqrt = getSqrt(num);
	
	// displaying square root
	cout << "\nSquare Root of the Entered Number is: " << sqrt << endl;
	return 0;
}

float getSqrt(int x) //function for calculating square root
{
	return sqrt(x);
}