#include <iostream>

using namespace std;
const int NUMBEROFTESTS = 5;

int main()
{
		
	int score, test, num=1 ; // the individual score read in
	float total = 0.0; // the total of the scores
	float average; // the average of the scores
	
	while ( num <=5 )
	{
		cout << "\nEnter your score on test " << num << ": " << endl;
		cin >> score ;
	
		if ( score >= 0 && score <= 100)
		{
		
		total += score ;
		
		num ++;
		}
		else 
		{
			cout << "\nKindly Enter Value ranging from (0 - 100) " << endl;
		}
	}
	
	//calculate the average of 5 scores
	average = total / 5 ;
	cout << "Your average based on " << NUMBEROFTESTS
	<< " test scores is " << average << endl;
	return 0;
}