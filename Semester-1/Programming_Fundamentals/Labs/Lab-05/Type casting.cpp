#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    const int AT_BAT = 421;
    const int HITS = 123;
    float batAvg;

    // Cast HITS to float to ensure floating-point division
    batAvg = ( float ) HITS / AT_BAT ; // Corrected assignment statement
    cout << "The batting average is " << batAvg << endl; // Output the result

    return 0;
}