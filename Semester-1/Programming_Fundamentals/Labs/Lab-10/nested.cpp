// This program finds the average time spent programming by a student
// each day over a three day period.

// PLACE YOUR NAME HERE

#include <iostream>
using namespace std;

int main()
{
	int numStudents;
	float numHours, total, average;
	int student,day = 0; // these are the counters for the loops
	
	cout << "This program will find the average number of hours a day"
	<< " that a student spent programming over a long weekend\n\n";

	cout << "How many students are there ?" << endl << endl;
	//input from user
	
	for(student = 1; student <= numStudents; student++)
	{
	//some code here
		
		for(day = 1; day <= 3; day++)
		{
			cout << "Please enter the number of hours worked by student "
				<< student <<" on day " << day << "." << endl;
			cin >> numHours;
			
			//some code here
		}
		average = total / 3;
		
		cout << endl;
		cout << "The average number of hours per day spent programming by "
			<< "student " << student << " is " << average
			<< endl << endl << endl;g
	}
	return 0;
}