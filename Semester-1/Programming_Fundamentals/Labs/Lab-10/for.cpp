// This program has the user input a number n and then finds the
// mean of the first n positive integers
// USAMA ARSHAD
#include <iostream>
using namespace std;

int main()
{
	int value; // value is some positive number n
	int total = 0; // total holds the sum of the first n positive numbers
	 // the amount of numbers
	 // the average of the first n positive numbers
	 float mean=0.0;
	 int _1stNo, _2ndNo;
	
	cout << "Enter Two positive Integers to find their mean" << endl;
	cout << "\nEnter first positive integer" << endl;
	cin >> _1stNo;
	cout << "Enter second positive integer" << endl;
	cin>>_2ndNo;
	
	if ( _1stNo > 0 && _2ndNo > 0)
	{
		for (int number = _1stNo; number <= _2ndNo; number++)
		{
			total = total + number;
		}
			mean = static_cast<float>(total) / ((_2ndNo-_1stNo) +1 ); // note the use of the typecast
			
		cout << "The mean  of the positive integers " << _1stNo <<" and "<<_2ndNo
		<< "  is " << mean << endl;
	}
	else
		cout << "Invalid input - integer must be positive" << endl;
	
	return 0;
}