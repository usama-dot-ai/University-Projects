#include <iostream>
using namespace std;

template <typename T>
T add(T a, T b) 
{
    return a + b;
}
int main ()
{
    cout << endl << add(5, 7) << endl;
    cout << add(5.5, 7.7) << endl;

    cout << endl;
    return 0;
}
