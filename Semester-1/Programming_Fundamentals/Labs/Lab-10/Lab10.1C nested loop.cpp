// This program finds the average time spent programming by a student
// each day over a three day period.
// USAMA ARSHAD

#include <iostream>
using namespace std;

int main()
{
	int num_students;
	float num_hours, average, total_avg=0;
	int student,day = 0; // these are the counters for the loops
	
	cout << "This program will find the average number of hours a day"
	<< " that a student spent programming over a long weekend\n";

	cout << "\nHow many students are there ?" << endl;
	cin >> num_students ;

	for(student = 1; student <= num_students; student++)
	{
		float total=0;
		for(day = 1; day <= 3; day++)
		{
			cout << "\nEnter number of hours worked by student "
				<< student <<" on day " << day << "." << endl;
			cin >> num_hours;
			
			total += num_hours ;
		}
		average = total / 3;
		total_avg += average ;
		cout << "\nThe average number of hours per day spent programming by "
			<< "student " << student << " is " << average
			<< endl;
	}
	cout << "\nThe average number of hours programmed by students is " << total_avg << endl;		
	return 0;
}