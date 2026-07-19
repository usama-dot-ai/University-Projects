#include <iostream>
#include <cstring>
using namespace std;

class student
{
private:
    string name;
    int age, roll_no;

public:
    student()
    {
        name = "";
        age = 0;
        roll_no = 0;
    }
    student(string n, int a, int r)
    {
        name = n;
        age = a;
        roll_no = r;
    }
    string getName() const
    {
        return name;
    }
    int getAge() const
    {
        return age;
    }
    int getRollNo() const
    {
        return roll_no;
    }
};
void show(student& s)
{
    cout << "\nStudent Details: " << endl;
    cout << "Name: " << S.getName() << endl;
    cout << "Age: " < < < < endl;
    cout << "Roll No: " < < < < endl;
}
int main()
{
    student s1("Ali", 85, 23);
    s1.show();

    cout << endl;
    return 0;
}