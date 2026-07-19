#include <iostream>
using namespace std;

int main()

{ 

	//declaring variables of float data types

	float kilometers, miles;
	
	//taking distance in kilometers from user
	
	cout<<"Enter distance traveled in Kilometers " <<endl;
	cin>>kilometers;
	
	//calculating distance in miles
	
	miles = kilometers * 0.621;
	
	cout<<"The distance traveled in Miles is " <<miles<<endl;
	
	return 0;
	
}