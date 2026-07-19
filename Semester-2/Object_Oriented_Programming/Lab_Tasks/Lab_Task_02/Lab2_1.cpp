#include <iostream>
#include <cstring>
using namespace std;

class student
{
public:
    string name;
    int age;
    student() // default constructor
    {
        name = "\nUnknown ";
        age = 0;
    }
    student(string n, int a) // parameterized constructor
    {
        name = n;
        age = a;
    }
};
int main()
{
    student s1;
    cout << s1.name << endl;
    cout << s1.age << endl;

    student s2("\nUsama", 20);
    cout << s2.name << endl;
    cout << s2.age << endl;
    
    cout << endl;
    return 0;
}