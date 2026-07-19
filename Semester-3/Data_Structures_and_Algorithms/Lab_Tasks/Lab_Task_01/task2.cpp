
#include <iostream>
using namespace std;

// Changes the value pointed to by p to 10
void change(int *p)
{
    *p = 10;
}

// Computes factorial of n recursively
int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

// Main: Demonstrates pointer modification and factorial calculation
int main()
{
    int c = 0;

    change(&c); // Changes c to 10 via pointer
    cout << c << endl;

    int fact = factorial(5); // Compute 5!
    cout << fact << endl;

    return 0;
}
