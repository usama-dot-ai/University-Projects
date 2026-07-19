#include <iostream>
using namespace std;
int main()
{
	// Declaring variables
	int num, dif, no_terms, term;
	
	// Taking values from user
	cout << "Enter a specific starting number. " << endl;
	cin >> num ;
	cout << "Enter the Common difference. " << endl;
	cin >> dif;
	cout << "Enter the number of terms. " << endl;
	cin >> no_terms;
	cout << "\nThe Arithmetic series is " << endl;
	
	// Using for loop for printing the results
	for (int i = 0; i < no_terms; i++) 
	{
         term = num + i * dif;
         cout << endl << term;
    }
    return 0;
}