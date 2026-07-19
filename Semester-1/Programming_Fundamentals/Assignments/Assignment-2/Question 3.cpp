#include <iostream>
using namespace std;
int main ()
{
	// Declaring variables
	int num1=0, num2=1, no_terms, next_term, counter=0;
	cout << "Enter No. Terms for Fibonacci Series. " << endl;
	cin >> no_terms ;
	cout << "\nFibonacci series are. " << endl;

	// Using while loop for the series
	while ( counter < no_terms ) 
	{
		cout << num1 << " ";
		next_term = num1 + num2 ;
		num1 = num2 ;
		num2 = next_term ;
		counter++ ;
	}

	return 0;
}