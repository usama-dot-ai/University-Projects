#include <iostream>
#include<iomanip>
using namespace std;

int main()
{
   const int HIGH_SCORE = 95;   
   int score1, score2, score3;  
   double average;              

   // Get the three test scores.
   cout << "Enter 3 test scores and I will average them: ";
   cin >> score1 >> score2 >> score3;
   
   // Calculate and display the average score.
   average = ( score1 + score2 + score3 ) / 3.0 ;
   cout << fixed << showpoint << setprecision ( 0 ) ;
   cout << "Your average is " << average << endl;
   
   // Showing the grade according to the average
   if ( average >= 90 )
   {
   	cout << "You got Grade A " << endl ;
   }
   else if ( average >= 80 )
   {
   	cout << "You got Grade B " << endl ;
   }
   else if (average >= 70 )
   {
   	cout << "You got Grade C " << endl ;
   }
   else if ( average >= 60 )
   {
   	cout << "You got Grade D " << endl ;
   }
   else if (average >= 50 )
   {
   	cout << "You got Grade E " << endl ;
   }
   else 
   {
   	cout << "Failed ! Try hard " << endl ;
   }
   // If the average is a high score, congratulate the user.
   if (average > HIGH_SCORE)
      cout << "Congratulations! That's a high score!\n";

   return 0;
} 