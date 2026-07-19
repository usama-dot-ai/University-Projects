#include <iostream>
#include <cstring>
using namespace std;

class Student
{
private:
    string std_name;
    int std_roll_no;
    float std_marks;
    friend void stdReport(const Student &s);

public:
    Student() // defualt constructor
    {
        std_name = "";
        std_roll_no = 0;
        std_marks = 0.0;
    }
    Student(string name, int roll_no, float marks) // parameterized constructor
    {
        std_name = name;
        std_roll_no = roll_no;
        std_marks = marks;
    }
};
void stdReport(const Student &s)
{
    cout << "\nStudent Report: " << endl;
    cout << "Name: " << s.std_name << endl;
    cout << "Roll No: " << s.std_roll_no << endl;
    cout << "Marks: " << s.std_marks << "%" << endl;
}
int main()
{
    string name;
    int roll_no;
    float marks;

    cout << "\nEnter Student Name: ";
    getline(cin, name);

    cout << "Enter Student's Roll No: ";
    cin >> roll_no;
    while (roll_no <= 0)
    {
        cout << "Enter Valid Roll No (Greater than 0): ";
        cin >> roll_no;
    }

    cout << "Enter Student's Marks %: ";
    cin >> marks;
    while (marks < 0 || marks > 100)
    {
        cout << "Enter Valid Marks % (Between 0 and 100): ";
        cin >> marks;
    }

    Student s1(name, roll_no, marks);
    stdReport(s1);

    cout << endl;
    return 0;
}