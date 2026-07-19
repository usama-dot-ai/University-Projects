#include <iostream>
using namespace std;

class A
{
public:
    void display()
    {
        cout << "\nThis is first class. " << endl;
    }
};
class B : virtual public A
{
};
class C : virtual public A
{
};
class D : public B, public C
{
};
int main()
{
    D d1;
    d1.display();

    cout << endl;
    return 0;
}