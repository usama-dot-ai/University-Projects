#include <iostream>
#include <cstring>
using namespace std;

struct student
{
    string name;
    int roll_no;
    float marks;
};

int main ()
{
    student s1;

    cout << "Enter Student's Name: " << endl;
    getline(cin, s1.name);

    cout << "Enter Student's Roll No: " << endl;
    cin >> s1.roll_no;
    while (s1.roll_no <= 0)
    {
        cout << "Enter Valid Roll No: " << endl;
        cin >> s1.roll_no;
    }

    cout << "Enter Student's Marks (in %): " << endl;
    cin >> s1.marks;
    while (s1.marks < 0.0)
    {
        cout << "Enter Valid Marks Percentage: " << endl;
        cin >> s1.marks;
    }

    cout << "\nStudent's Details: " << endl;
    cout << "Name: " << s1.name << endl;
    cout << "Roll No: " << s1.roll_no << endl;
    cout << "Marks: " << s1.marks << "%" << endl;

    return 0;
}