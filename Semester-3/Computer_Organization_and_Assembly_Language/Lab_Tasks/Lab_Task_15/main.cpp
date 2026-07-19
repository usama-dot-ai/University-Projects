#include <iostream>
using namespace std;

extern "C" int add(int a, int b);

int main()
{
    int a = 10, b = 20;
    cout << "\nSum = " << add(a, b) << endl;
    return 0;
}