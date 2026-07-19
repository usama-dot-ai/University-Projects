#include<iostream>
using namespace std;

//function prototype
int add (int a, int b); 
int add (int a, int b, int c);
float add (float a, int b, int c);

int main() //main function
{
	//variables for storing numbers
	float x ;
	int y, z ;
	
	cout << "Enter first number: " ;
	cin >> x ;
	
	cout << "Enter second number: " ;
	cin >> y ;
	
	cout << "Enter third number: " ;
	cin >> z ;
	
	//function calling
	add (x,y); 
	add (x,y,z);
	add (x,y,z);
	
	return 0;
}

int add (int a, int b) //function for sum of two numbers
{
	int sum ;
	sum = a+b ;
	cout << "Sum of two integers is: " << sum << endl;
	return sum ;
}

int add (int a, int b, int c) //function for sum of threee numbers
{
	int sum ;
	sum = a+b+c ;
	cout << "The sum of three integers is: " << sum << endl;
	return sum;
}

float add (float a, int b, int c)
{
	float sum ;
	sum = a+b+c ;
	cout << "The sum of one float and two integers is: " << sum << endl ;
	return sum ;
}