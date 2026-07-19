#include <iostream>
using namespace std;
int main()
{
	int num1, num2, num3 ;
	cout << "Enter three numbers " << endl ;
	cin >> num1 >> num2 >> num3 ;
	
	if(num1>num2 )
	{
		cout << "The greatest number is " << num1 << endl ;
	}
	if (num1>num3)
	{
		cout << "The greatest number is " << num1 << endl ;
	}
	if (num2 > num1)
	{
		cout << "The greatest number is " << num2 << endl ;
	}
	if (num2 > num3)
	{
		cout << "The greatest number is " << num2 << endl ;
	}
	if (num3 >  num1)
	{
		cout << "The greatest number is " << num3 << endl ;
	}
	if (num3 >num2)
	{
		cout << "The greatest number is " << num3 << endl ;
	}
	
	return 0 ;
}