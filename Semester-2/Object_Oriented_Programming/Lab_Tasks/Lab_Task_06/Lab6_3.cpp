#include <iostream>
using namespace std;

class Parent
{
    public:
    void displayParent()
    {
        cout << "\nParent Class. " << endl;
    }
};

class Child: public Parent
{
    public:
    void displayChild()
    {
        cout << "Child Class. " << endl;
    }
};

class SuperChild: public Child
{
    public:
    void displaySuperChild()
    {
        cout << "Super Child Class. " << endl;
    }
};

int main ()
{
    SuperChild sc1;
    sc1.displayParent();
    sc1.displayChild();
    sc1.displaySuperChild();

    cout << endl;
    return 0;
}