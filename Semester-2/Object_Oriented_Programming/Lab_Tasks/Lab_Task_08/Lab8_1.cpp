#include <iostream>
using namespace std;

class A
{
public:
    A()
    {
        cout << "\nConstructor of Class A. " << endl;
    }
    ~A()
    {
        cout << "Desctructor of Class A. " << endl;
    }
    void display()
    {
        cout << "This is Class A. " << endl;
    }
};
class B
{
private:
    A a;

public:
    B()
    {
        cout << "Constructor of Class B. " << endl;
    }
    ~B()
    {
        cout << "Desctructor of Class B. " << endl;
    }
    void display()
    {
        a.display();
        cout << "This is Class B. " << endl;
    }
};

int main()
{
    B b1;
    b1.display();

    return 0;
}