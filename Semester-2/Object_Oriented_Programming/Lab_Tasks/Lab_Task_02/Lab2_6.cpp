#include <iostream>
#include <string>
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
    string getName() 
    {
        return name;
    }
    int getAge() 
    {
        return age;
    }
    int getRollNo() 
    {
        return roll_no;
    }
};

void show( student &s)
{
    cout << "\nStudent Details: " << endl;
    cout << "Name: " << s.getName() << endl;
    cout << "Age: " << s.getAge() << endl;
    cout << "Roll No: " << s.getRollNo() << endl;
}

int main()
{
    student s1("Usama", 20, 247141);
    show(s1);

    cout << endl;
    return 0;
}