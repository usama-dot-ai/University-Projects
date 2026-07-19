#include <iostream>
using namespace std;

class ADD
{
    int x, y;

public:
    ADD()
    {
        x = 0;
        y = 0;
    }
    ADD(int a, int b)
    {
        x = a;
        y = b;
    }
    ADD operator+(ADD &obj)
    {
        ADD sum;
        sum.x = x + obj.x;
        sum.y = y + obj.y;
        return sum;
    }
    ADD operator++(int)
    {
        x++;
        y++;
        return *this;
    }
    void display()
    {
        cout << "\nSum of First Number: " << x << endl;
        cout << "Sum of Second Number: " << y << endl;
    }
};

int main()
{
    ADD a1(15, 30);
    ADD a2(45, 60);

    ADD a3 = a1 + a2;
    a3.display();

    cout << "\nAfter incrementing a3:" << endl;
    a3++;
    a3.display();

    cout << endl;
    return 0;
}
