#include <iostream>
using namespace std;

class First
{
public:
    virtual void display()
    {
        cout << "\nThis is First Class. " << endl;
    }
};
class Second : public First
{
public:
    void display()
    {
        cout << "This is Second Class. " << endl;
    }
};
class Third : public First
{
public:
    void display()
    {
        cout << "This is Third Class. " << endl;
    }
};
int main()
{
    First *p;
    Second s;
    Third t;

    p->First::display();

    p = &s;
    p->display();

    p = &t;
    p->display();

    cout << endl;
    return 0;
}