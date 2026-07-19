#include <iostream>
#include <cstring>
using namespace std;

class Student
{
private:
    string std_name;
    int std_age;
    float std_marks;
    friend void Display(const Student& s);

public:
    Student() // default constructor
    {
        std_name = "";
        std_age = 0;
        std_marks = 0.0;
    }
    Student(string name, int age, float marks) // parameterized constructor
    {
        std_name = name;
        std_age = age;
        std_marks = marks;
    }
};
void Display(const Student& s)
{
    cout << "\nStudent Details: " << endl;
    cout << "Name: " << s.std_name << endl;
    cout << "Age: " << s.std_age << endl;
    cout << "Marks: " << s.std_marks << "%" << endl;
}
int main()
{
    string name;
    int age;
    float marks;

    cout << "\nEnter Student Name: ";
    getline(cin, name);

    cout << "Enter Student Age: ";
    cin >> age;
    while (age <= 0)
    {
        cout << "Enter Valid Age (Greater Than 0): ";
        cin >> age;
    }

    cout << "Enter Student Marks (%): ";
    cin >> marks;
    while (marks < 0 || marks > 100)
    {
        cout << "Enter Valid Marks % (Between 0 and 100): ";
        cin >> marks;
    }

    Student s1(name, age, marks);
    Display(s1);

    cout << endl;
    return 0;
}