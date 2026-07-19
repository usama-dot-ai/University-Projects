#include <iostream>
using namespace std;
int main()
{
	//variable declaration
	int num, numFact=1 ;
	
	//taking input from user
	cout << "Enter number for Factorial " << endl;
	cin >> num ;
	
	//checking validation and calculating factorial
	if (num > 0)
	{
	 while (num > 0)
	 {
		numFact = numFact * num ;
		num -- ;
	 } 
	 cout << "The Factorial of number is " << numFact << endl;
	}
	else
	{
		cout << "Kindly Enter Positive number " << endl;
	}
	return 0;
}