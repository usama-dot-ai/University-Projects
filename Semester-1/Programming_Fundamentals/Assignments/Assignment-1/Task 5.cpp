#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	//Declaring variables
	float side1, side2, side3, semi, area;
	
	//Taking length of each side from user
	cout << "Enter Length of First Side " << endl ;
	cin >> side1 ;
	
	cout << "Enter Length of Second Side " << endl ;
	cin >> side2 ;
	
	cout << "Enter Length of Third Side " << endl ;
	cin >> side3 ;
	
	//using Heron's Formula
			 
	//Calculating the semi perimeter first
	semi = ( side1 + side2 + side3 ) / 2 ;
	cout << "The Semi-Perimeter is " << semi << endl ;
	
	//Using the semi perimeter to find the area
	area = sqrt ( semi * ( semi - side1 ) * ( semi - side2 ) * ( semi - side3 ) ) ;
	cout << endl << "The Area of Triangle is " << area ;
	
	return 0 ;
}