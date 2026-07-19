#include <iostream>
using namespace std;
int main()
{
	//Declaring Variables
	int choice, i=1, coffee_sum=0, tea_sum=0, coke_sum=0, juice_sum=0 ;

	do
	{
		//Asking for favourite Beverages
		cout << "\nEnter favourite beverage for Person #" << i <<" :" << endl;
		cout << "Choose 1.Coffee 2.Tea 3.Coke 4.Orange Juice \nor Choose -1 to Exit" << endl;
		cin >> choice ;
		i++ ;
		
		if (choice == 1)
		{
			coffee_sum = coffee_sum + choice ;
		}
		if (choice == 2)
		{
			tea_sum = tea_sum + (choice - 1) ;
		}
		if (choice == 3)
		{
			coke_sum = coke_sum + (choice - 2) ;
		}
		if (choice == 4)
		{
			juice_sum = juice_sum + (choice - 3) ;
		}
	}	
	while (choice!=-1);
	
	cout << "\nTotal Number of people Surveyed is " << i-2 << endl;
	cout << "\nBeverage \tNumber of Votes " << endl;
	cout << "*********************************" << endl;
	cout << "Coffee" << "\t\t\t" << coffee_sum << endl;
	cout << "Tea" << "\t\t\t" << tea_sum << endl;
	cout << "Coke" << "\t\t\t" << coke_sum << endl;
	cout << "Orange Juice" << "\t\t" << juice_sum << endl;
	
	return 0;
}