// This program prints the proverb
// "Now is the time for all good men to come to the aid of their party"
// in a function (procedure) called writeProverb that is called by the main function

//USAMA ARSHAD

#include <iostream>
using namespace std;

void writeProverb(); //This is the prototype for the writeProverb function

int main()
{
	writeProverb(); //calling the proverb function
	return 0;
}

void writeProverb() //function for displaying proverb
{
	cout << "\nNow is the time for all good men to come to the aid of their party. " << endl;
}