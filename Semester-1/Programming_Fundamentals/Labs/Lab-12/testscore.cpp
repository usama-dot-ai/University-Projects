// This program will read in a group of test scores (positive integers from 1 to 100)
// from the keyboard and then calculate and output the average score
// as well as the highest and lowest score. There will be a maximum of 100 scores.

// PLACE YOUR NAME HERE

#include <iostream>
using namespace std;


int scores[100]; // an integer array of 100 elements


float findAverage (int); // finds average of all grades
int findHighest (int); // finds highest of all grades
int findLowest (int); // finds lowest of all grades

int main()
{
	
	
	int numberOfGrades; // the number of grades read.
	int pos; // index to the array element.
	
	float avgOfGrades; // contains the average of the grades.
	int highestGrade; // contains the highest grade.
	int lowestGrade; // contains the lowest grade.
	
	// Read in the values into the array
	pos = 0;
	
	cout << "Please input a grade from 1 to 100, (or -99 to stop)" << endl;
	cin >> scores[pos];
	while (scores[pos] != -99) //-99 is sentinel
	{
	// Fill in the code to read the scores
	}
	
	
	numberOfGrades = _________; // Fill blank with appropriate identifier, pay attension to bound errors
	
	// call to the function to find average
	avgOfGrades = findAverage(numberOfGrades);
	cout << endl << "The average of all the scores is " << avgOfGrades << endl;
	
	// Fill in the call to the function that calculates highest grade
	
	cout << endl << "The highest grade is " << highestGrade << endl;
	
	// Fill in the call to the function that calculates lowest grade
	// Fill in code to write the lowest to the screen
	
	return 0;
}

//********************************************************************************
// findAverage
//
// task: This function receive size as the no of elements in the globally defined array.
// It finds and returns the average of the numbers in the array
// data in:  number as size
// data returned: average of the numbers in the array
//
//********************************************************************************
____ findAverage (int size)
{
	float _____ = 0; // holds the sum of all the numbers

	for (int i = 0; i < size; i++)
		sum = sum + scores[i];
		
	_______ (sum / size); //returns the average
}

//****************************************************************************
// findHighest
//
// task: This function receive size as the no of elements in the globally defined array.
// It finds and returns the highest value of the numbers in the array
// data in:  number
// data returned: highest value of the numbers in the array
//
//****************************************************************************
______ findHighest (________)
{
	
	// Fill in the code for this function

}

//****************************************************************************
// findLowest
//
// task: This function receive size as the no of elements in the globally defined array.
// It finds and returns the lowest value of the numbers in the array
// data in: number
// data returned: lowest value of the numbers in the array
//
//****************************************************************************
______ findLowest (_____)
{
	
	// Fill in the code for this function

}