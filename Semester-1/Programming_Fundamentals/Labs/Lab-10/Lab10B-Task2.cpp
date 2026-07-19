#include <iostream>
using namespace std;
int main ()
{
	//declaring variables
	int speed, time;
	float distance;
	
	//asking user for speed and time and performing input validation
	cout << "What is the speed of vehicle in mph ?" << endl;
	cin >> speed ;
	while (speed <= 0)
	{
		cout << "Kindly Enter speed atleast 1 mph." << endl;
		cin >> speed ;
	}
	cout << "How many hours the vehicle has traveled ?" << endl;
	cin >> time ;
	while (time < 1)
	{
		cout << "Kindly Enter Time atleast 1 hour." << endl;
		cin >> time;
	}
	
	//calculating total distance
	distance = speed * time ;
	cout << "\nTotal Distance traveled is " << distance << " miles" << endl;
	
	//printing pattern
	cout << "\nHours\t Distance Traveled" << endl;
	cout << "--------------------------" << endl;
		
		//calculating distance covered per each hour	
		for (int i=1 ; i<=time ; i++)
		{
			distance = speed * i ;

			cout << i << "\t\t" << distance << endl;
		}
	return 0;
}