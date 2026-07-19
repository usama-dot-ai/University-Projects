#include <iostream>
#include <iomanip>
using namespace std;
int main ()
{
	// Declaring prices of Chairs
	float Achair = 85.00, Mchair = 57.50, Fchair = 127.75 ;
	
	// Declaring variabes
	int nofAC, nofMC, nofFC ;
	
	// Taking number of sales from user
	cout << "Enter number of American Chairs sold " << endl ;
	cin >> nofAC ;
	cout << "Enter number of Modern Chairs sold " << endl ;
	cin >> nofMC ;
	cout << "Enter number of French Chairs sold " << endl ;
	cin >> nofFC ;
	
	// Prinitng the sale of each Chair type seperately
	float saleAC, saleFC, saleMC, totalsale ;
	cout << fixed << setprecision (2) ;
	saleAC = Achair * nofAC ;
	cout << endl << "Total sales of American Chairs are " << saleAC << endl ;
	saleMC = Mchair * nofMC ;
	cout << "Total sale of Modern Chairs are " << saleMC << endl ;
	saleFC = Fchair * nofFC ;
	cout << "Total sales of French Chairs are " << saleFC << endl ;
	
	// Printing the Total sale of All Chairs
	totalsale = saleAC + saleMC + saleFC ;
	cout << endl << "The Total sale of all chairs is " << totalsale << endl ;
	
	return 0 ;
}