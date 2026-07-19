# include <iostream>
using namespace std;
int main ()
{
	//Declaring variables
	double amount, rate, time, interest;
	
	//Taking required data from user
	cout << "Enter Principal Amount " << endl ;
	cin >> amount ;
	
	cout << "Enter Rate of Interest ( in % ) " << endl ;
	cin >> rate ;
	
	cout << "Enter Time in Years " << endl ;
	cin >> time ;
	
	//Calculating Simple Interest 
	interest = ( amount * rate * time ) / 100 ;
	cout << "The Simple Interest is " << interest << endl ;
	
	return 0 ;
}