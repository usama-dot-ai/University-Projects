// This program takes daily sales figures over a period of time
// and calculates their total.
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
   double total = 0.0;  // Accumulator, initialized with 0
   int days;
   float sale;
   // Get the number of days.
  cout << "Enter the number of days " << endl;
  cin >> days ;
 
    // Get the sales for each day and accumulate a total.
   
   int i=1;
   
   while( i <= days) //complete the expression
   {
      //input sale figure of the day
      cout << "Enter the sales for day " << i << endl;//....
      cin >> sale;
          
     // Accumulate the running total.
      total += sale;
      //update counter vairable
      i++;
   }
   
   // Display the total sales.
   cout << fixed << showpoint << setprecision(2)<<endl;
   cout << "The total sales are $" << total << endl;
   return 0;
}