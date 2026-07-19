#include <iostream>
using namespace std;
int main ()
{
	// declaring variables
	int minutes, seconds ;
	
	//taking inputs
	cout << "Enter number of minutes " << endl;
	cin >> minutes ;
	
	//type validation
	while ( minutes<0 )
	{
		cout << "Kindly enter atleast 0 minutes " << endl;
		cin >> minutes ;
	}
	cout << "Enter number of seconds " << endl;
	cin >> seconds ;
	while ( seconds<1 )
	{
		cout << "Kindly enter seconds between (0 - 59) " << endl;
		cin >> seconds ;
	}
	
	//loop for timer
	for (int i=minutes ; i>=0 ; i-- )
	{
		for (int j=seconds ; j>=0 ; j-- )
		{
			cout << i << " Minutes " << j << " Seconds " << endl;
			cout.flush();
		}
		seconds = 59;
	}
	return 0;
}