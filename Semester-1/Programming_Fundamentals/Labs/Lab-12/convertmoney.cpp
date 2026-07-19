#include <iostream>
#include <iomanip>
using namespace std;

// This program will input American money and convert it to foreign currency
// PLACE YOUR NAME HERE

// Prototypes of the functions
void convertMulti(float dollars, float& euros, float& rupees);
void convertMulti(float dollars, float& euros, float& rupees, float& yen);

float convertToYen(float dollars);
float convertToEuros(float dollars);
float convertToRupees(float dollars);

// Define exchange rates
const float EURO_RATE = 0.85;    // Example rate
const float RUPEE_RATE = 74.28; // Example rate
const float YEN_RATE = 110.49;  // Example rate

int main()
{
    float dollars;
    float euros;
    float rupees;
    float yen;

    cout << fixed << showpoint << setprecision(2);

    cout << "Please input the amount of American Dollars you want converted "
         << endl;
    cout << "to euros and rupees" << endl;
    cin >> dollars;

    // Call convertMulti with dollars, euros, and rupees
    convertMulti(dollars, euros, rupees);
    cout << dollars << " dollars = " << euros << " euros and " << rupees << " rupees." << endl;

    cout << "Please input the amount of American Dollars you want converted\n";
    cout << "to euros, rupees and yen" << endl;
    cin >> dollars;

    // Call convertMulti with dollars, euros, rupees, and yen
    convertMulti(dollars, euros, rupees, yen);
    cout << dollars << " dollars = " << euros << " euros, " << rupees << " rupees, and " << yen << " yen." << endl;

    cout << "Please input the amount of American Dollars you want converted\n";
    cout << "to yen" << endl;
    cin >> dollars;

    // Call convertToYen
    yen = convertToYen(dollars);
    cout << dollars << " dollars = " << yen << " yen." << endl;

    cout << "Please input the amount of American Dollars you want converted\n";
    cout << " to euros" << endl;
    cin >> dollars;

    // Call convertToEuros
    euros = convertToEuros(dollars);
    cout << dollars << " dollars = " << euros << " euros." << endl;

    cout << "Please input the amount of American Dollars you want converted\n";
    cout << " to rupees " << endl;
    cin >> dollars;

    // Call convertToRupees
    rupees = convertToRupees(dollars);
    cout << dollars << " dollars = " << rupees << " rupees." << endl;

    return 0;
}

// **************************************************************************
// convertMulti
//
// task: This function takes a dollar value and converts it to euros
// and rupees
// data in: dollars
// data out: euros and rupees
// **************************************************************************
void convertMulti(float dollars, float& euros, float& rupees)
{
    euros = convertToEuros(dollars);
    rupees = convertToRupees(dollars);
}

// ************************************************************************
// convertMulti
//
// task: This function takes a dollar value and converts it to euros,
// rupees, and yen
// data in: dollars
// data out: euros, rupees, yen
// ***********************************************************************
void convertMulti(float dollars, float& euros, float& rupees, float& yen)
{
    euros = convertToEuros(dollars);
    rupees = convertToRupees(dollars);
    yen = convertToYen(dollars);
}

// ****************************************************************************
// convertToYen
//
// task: This function takes a dollar value and converts it to yen
// data in: dollars
// data returned: yen
// ***************************************************************************
float convertToYen(float dollars)
{
    return dollars * YEN_RATE;
}

// ****************************************************************************
// convertToEuros
//
// task: This function takes a dollar value and converts it to euros
// data in: dollars
// data returned: euros
// ***************************************************************************
float convertToEuros(float dollars)
{
    return dollars * EURO_RATE;
}

// *****************************************************************************
// convertToRupees
//
// task: This function takes a dollar value and converts it to rupees
// data in: dollars
// data returned: rupees
// ****************************************************************************
float convertToRupees(float dollars)
{
    return dollars * RUPEE_RATE;
}
