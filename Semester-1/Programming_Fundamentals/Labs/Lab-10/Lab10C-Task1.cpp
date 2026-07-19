#include <iostream>
using namespace std;
int main()
{
	// declaring variables
	int num;
	
	// taking input from user
	cout << "Enter number you want to see tables. " ;
	cin >> num;
	
	// checking validation
	while (num<1)
	{
		cout << "Kindly enter any number greater than 0. " << endl;
	}
	
	// printing tables
	for (int i=1 ; i <= num ; i++)
	{
		for (int j=1 ; j<=10 ; j++)
		{
		cout << "\t" << i*j << " " ;
		}
		cout << endl;
	}
	return 0;
}