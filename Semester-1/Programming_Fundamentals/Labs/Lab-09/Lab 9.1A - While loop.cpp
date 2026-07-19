#include <iostream>

using namespace std;

int main()
{
	char letter = 'a';
	
	cout<<"This program allows the user to enter a letter and then print on screen"<<endl;
	cout<<"\nInput a character or x for exiting this program"<<endl;
	
	while (true)
	{
	
		cout<<"\nPlease enter any letter : " << endl;
		cin>>letter;
	
		if (letter == 'x')
		{
			cout << "You entered x. \nNow Program is exiting" << endl;
			break;
		}
		
		cout << "\nYou entered the letter " << letter << endl;
		cout << "\nType 'x' to Exit the program " << endl;
	}
	return 0;
}