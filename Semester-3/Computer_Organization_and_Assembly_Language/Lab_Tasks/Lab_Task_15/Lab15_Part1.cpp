#include <iostream>
using namespace std;

int add(int a, int b) // Inline assembly function
{
    int result;

    asm volatile(
        "movl %1, %%eax;"
        "addl %2, %%eax;"
        "movl %%eax, %0;"
        : "=r"(result)
        : "r"(a), "r"(b)
        : "%eax");

    return result;
}

int main()
{
    int x = 5, y = 7;
    cout << "\nSum = " << add(x, y) << endl;
    return 0;
}