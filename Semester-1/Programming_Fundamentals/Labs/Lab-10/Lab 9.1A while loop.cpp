// This program displays a hot beverage menu and prompts the user to
// make a selection. A switch statement determines which item the user
// has chosen. A do-while loop repeats until the user selects item E
// from the menu.

// USAMA ARSHAD

#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
	// Fill in the code to define an integer variable called number,
	// a floating point variable called cost,
	// and a character variable called beverage
	int number;
	float cost;
	char beverage;
	bool validBeverage;
	
	cout << fixed << showpoint << setprecision(2);
	
	do
	{
		cout << "\nA: Coffee $1.00" << endl;
		cout << "B: Tea $ 0.75" << endl;
		cout << "C: Hot Chocolate $1.25" << endl;
		cout << "D: Cappuccino $2.50" << endl;
		cout << "\nEnter the beverage A,B,C, or D you desire "
		"\nOtherwise Enter E to EXIT" << endl;
		cin >> beverage;
		// Fill in the code to read in beverage
		switch(beverage)
		{
			case 'A': cout << "Coffee $1.00 selected " << endl ;
				break;
			case 'B': cout << "Tea $0.75 selected " << endl;
				break;
			case 'C': cout << "Hot Chocolate $1.25 selected " << endl;
				break;
			case 'D': cout << "Capuccino $2.50 selected " << endl;
				break;
			default: cout << "Invalid Selection " << endl;
		}
		if (beverage =='A'||beverage =='B'||beverage =='C'||beverage =='D')
		{
			cout << "How many cups would you like?" << endl;
			cin >> number ;
			
		switch (beverage)
		{
		case 'A': cost = number * 1.0;
			cout << "The total cost is $ " << cost << endl;
			break;
		case 'B': cost = number * 0.75;
			cout << "The total cost is $ " << cost << endl;
			break;	
		case 'C': cost = number * 1.25;
			cout << "The total cost is $ " << cost << endl;
			break;
		case 'D': cost = number * 2.50;
			cout << "The total cost is $ " << cost << endl;
			break;
		default: cout << " Try again please" << endl;
		}
		}	
	}
	while(beverage !='E' && beverage!='e');
	{
		cout << "Beverage is not Available " << endl;
	}
	return 0;
}