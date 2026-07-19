// This program will input an undetermined number of student names
// and a number of grades for each student. The number of grades is
// given by the user. The grades are stored in an array.
// Two functions are called for each student.
// One function will give the numeric average of their grades.
// The other function will give a letter grade to that average.
// 90-100 A 80-89 B 70-79 C 60-69 D Below 60 F

// PLACE YOUR NAME HERE

#include <iostream>
#include <iomanip>
using namespace std;

const int MAXTESTS = 25; // maximum number of TESTs per student
const int MAXCHAR = 30; // maximum characters used in a name


// two arrays of characters defined names having 30 characters or less.
char firstname[MAXCHAR + 1]; 
char lastname[MAXCHAR + 1]; 
float tests[MAXTESTS]; // one dimensional integer for storing scores


float findGradeAvg(int); // finds grade average by taking array of
// tests and number of tests as parameters
char findLetterGrade(float); // finds letter grade from average given
// to it as a parameter

int main()
{
	
	int numOfTests; // holds the number of tests
	float average; // holds the average of a student's grade
	char moreInput; // determines if there is more input
	
	cout << setprecision(2) << fixed << showpoint;
	
	// Input the number of tests for each student
	<< "This must be a number between 1 and " << MAXTESTS << " inclusive”
	<< endl;
	cin >> numOfTests;
	
	//input validation
	while (numOfTests > MAXTESTS || numOfTests < 1)
	{
		cout << "Please input the number of tests for each student." << endl
		<< "This must be a number between 1 and " << MAXTESTS
		<< " inclusive\n";
		cin >> numOfTests;
	}
	
	// Input names and grades for each student
	cout << "Please input a y if you want to input more students"
		<< " any other character will stop the input" << endl;
	cin >> moreInput;
	
	while (moreInput == 'y' || moreInput == 'Y')
	{
		cout << "Please input the first name of the student" << endl;
		cin >> firstname; //reading directly into a char array
		cout << endl << "Please input the last name of the student" << endl;
		cin >> lastname;
		
		for (int count = 0; count < numOfTests; count++)
		{
			cout << endl << "Please input a grade" << endl;
			// Fill in the input statement to place grade in the array
		}
		
		cout << firstname << " " << lastname << " has an average of ";
		
		// Fill in code to get and print average of student to screen
		// Fill in call to get and print letter grade of student to screen
		
		cout << endl << endl << endl;
		
		cout << "Please input a y if you want to input more students"
		<< " any other character will stop the input" << endl;
		cin >> moreInput;
	}
	return 0;
}

//***********************************************************************
// findGradeAvg
//
// task: This function finds the average of the
// numbers stored in an array.
//
// data in:  integer number
// data returned: the average of all numbers in the array
//
//***********************************************************************
float findGradeAvg( int numGrades)
{
// Fill in the code for this function

}

//***********************************************************************
// findLetterGrade
//
// task: This function finds the letter grade for the number
// passed to it by the calling function
//
// data in: a floating point number
// data returned: the grade (based on a 10 point spread) based on the number
// passed to the function
//
//***********************************************************************
char findLetterGrade(float numGrade)
{
// Fill in the code for this function

}


	