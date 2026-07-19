// This program will allow the user to input from the keyboard
// whether the last word to the following proverb should be party or country:
// "Now is the time for all good men to come to the aid of their "
// Inputting a 1 will use the word party. Any other number will use the word country.

// USAMA ARSHAD

#include <iostream>
#include <string>
using namespace std;

string writeProverb(int); // function prototype

int main () //main function
{
	int wordCode; //variable for storing choice of user
	
	cout << "\nGiven the phrase:" << endl;
	cout << "\nNow is the time for all good men to come to the aid of their ___"
	<< endl;
	
	cout << "\nInput a 1 if you want the sentence to be finished with word 'party'"
	<< endl;
	cout << "Input a 2 if you want the sentence to be finished with word 'country'" << endl;
	
	cout << "\nPlease input your choice now" << endl;
	cin >> wordCode;
	cout << endl;
	
	while (wordCode != 1 && wordCode!= 2) //validating the input
	{
		cout << "Kindly Enter 1 or 2." << endl;
		cin >> wordCode;
	}
	
	cout << "Now is the time for all good men to come to the aid of their" <<
	writeProverb(wordCode); // displaying output

	return 0;
}

string writeProverb (int number) //function for showing the chosen word
{
	string p = " party", c = " country";
	if (number == 1)
	{
		return p;
	}
	else
	{
		return c;
	}
}