#include<iostream>
using namespace std;
int main()
{
	// Declaring variables
   bool trueValue, falseValue, grtr, grtr_eql, lss_eql, not_eql ;
   int x , y ;
   
   // Taking Numbers from user
    cout << "Enter 1st number " << endl ;
    cin >> x ;
	cout << "Enter 2nd number " << endl ;
	cin >> y ;
   
   // calculating the results 
   trueValue = x < y;
   falseValue = y == x;
   grtr = x > y ;
   grtr_eql = x >= y ;
   lss_eql = x<= y ;
   not_eql = x != y ;
   
   cout << endl <<"The True Value is "<< trueValue<<endl;
   cout <<"The False Value is "<< falseValue<<endl;
   
   cout << endl << "The value of " << x << " > " << y << " = " << grtr<< endl ;
   cout << "The value of " << x << " >= " << y << " = " << grtr_eql << endl ;
   cout << "The value of " << x << " <= " << y << " = " << lss_eql << endl ;
   cout << "The value of " << x << " != " << y << " = " << not_eql << endl ;
    
   return 0;
}