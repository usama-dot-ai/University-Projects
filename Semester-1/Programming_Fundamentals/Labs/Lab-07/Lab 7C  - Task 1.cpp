
// USAMA ARSHAD

#include <iostream>
using namespace std;
int main()
{
	char grade;
	
	cout << "What grade did you earn in PF ?" << endl;
	cin >> grade;
	
	if (grade == 'A')
	{
	 cout << "an A - excellent work !" << endl;
	}
	else if (grade == 'B') 
	{
	cout << "you got a B - good job" << endl;
	}
	else if (grade == 'C')
	{
		cout << "You got C - satisfactory " << endl;
	}
	else if (grade == 'D')
	{
		cout << "There is Problem, D is passing " << endl;
	}
	else if (grade == 'F')
	{
		cout << "You are FAILED ! Better Luck Next Time " << endl;
	}
	else 
	{
		cout << "Enter Correct Grade (A, B, C, D, F) " << endl;
	}
	return 0 ;
}