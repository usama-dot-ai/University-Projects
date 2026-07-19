#include <iostream>
#include <iomanip>
using namespace std;

// PLACE YOUR NAME HERE

void normalizeMoney(float rupees, int paisas = 150);
// This function takes paisas as an integer and converts it to rupees (add that to the rupees passed)
/// The default value for paisas is 150 which is converted
// to 1.50 and stored in rupees


int main()
{
	int paisas;
	float rupees=0;
	
	cout << setprecision(2) << fixed << showpoint;
	paisas = 95;
	
	cout << "\n We will now add 95 paisas to our rupees total\n";
	
	// Fill in the code to call normalizeMoney to add 95 paisas, pass rupees as 1st arg,
	// store the calculated amount back in  var rupees 
	
	cout << "Converting paisas to rupees resulted in " << rupees << " rupees\n";
	cout << "\n We will now add 193 paisas to our rupees total\n";
	
	// Fill in the code to call normalizeMoney to add 193 paisas, pass rupees as 1st arg,
	// store the calculated amount back in  var rupees 
	
	cout << "Converting paisas to rupees resulted in " << rupees << " rupees\n";
	cout << "\n We will now add the default value to our rupees total\n";
	
	// Fill in the code to call normalizeMoney to add the default value of paisas, pass rupees as 1st arg,
	// store the calculated amount back in  var rupees 
	
	cout << "Converting paisas to rupees resulted in " << rupees << " rupees\n";

return 0;
}
//
*******************************************************************************
// normalizeMoney
//
// task: This function is given a value in paisas. It will convert paisas
// to rupees and paisas which is stored in a local variable called
// total which is return back to the calling function.

// data in: rupees, paisas which is an integer
// data out: rupees
//
//
*********************************************************************************
float normalizeMoney(float currRupees, int paisas)
{
	float paisa2Rup=0;
	
	// Fill in the code to convert paisas to rupees and store in paisa2Rup
	
	//fill in the code that add paisa2Rup with currRupees and return final value

}