#include <iostream>
#include <cstring>

using namespace std;

class person
{
private:
    string name;
    int age;

public:
    void setdata(string n, int a)
    {
        name = n;
        age = a;
    }
    void getdata()
    {
        cout << "\nName: " << name << endl;
        cout << "Age: " << age << endl;
    }
};

int main()
{
    person p1;
    string name;
    int age;

    cout << "\nEnter Name: ";
    getline(cin, name);

    cout << "Enter Age: ";
    cin >> age;

    while (age <= 0)
    {
        cout << "Enter Valid Age: ";
        cin >> age;
    }

    p1.setdata(name, age);
    p1.getdata();

    cout << endl;
    return 0;
}