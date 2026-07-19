#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main()
{
    string text;
    ofstream outfile;

    outfile.open("output.txt", ios::app);

    if (!outfile.is_open())
    {
        cout << "Error occured in File Opening. " << endl;
    }

    cout << "\nHello! This is Lab task. " << endl;
    outfile << "\nHello! This is Lab task. " << endl;

    cout << "Enter Any Text: " << endl;
    getline(cin, text);

    outfile << text;
    outfile.close();

    cout << endl;
    
    ifstream infile;
    infile.open ("Output.txt");
    
    if (!infile.is_open())
    {
        cout << "Error occured in File Opening. " << endl;
    }

    string line;
    
    while (getline(infile, line))
    cout << line << endl;

    cout << endl;
    return 0;
}