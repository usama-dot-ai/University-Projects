#include <iostream>
#include <cstring>
using namespace std;

struct student
{
    string name;
    int roll_no;
    float marks;
};

void display(const student s[], int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << "\nStudent " << (i+1) << " Details: " << endl;
        cout << "Name: " << s[i].name << endl;
        cout << "Roll No: " << s[i].roll_no << endl;
        cout << "Marks: " << s[i].marks << "%" << endl;   
        cout << endl;
    }
}

int main ()
{
    const int max_students = 3;
    student s1 [max_students];

    for (int i = 0; i < max_students; i++)
    {
        cout << "Enter Student " << (i+1) << " Name: " << endl;
        getline(cin, s1[i].name);

        cout << "Enter Student " << (i+1) << " Roll No: " << endl;
        cin >> s1[i].roll_no;
        while (s1[i].roll_no <= 0)
        {
        cout << "Enter Valid Roll No: " << endl;
        cin >> s1[i].roll_no;
        }

        cout << "Enter Student " << (i+1) << " Marks (in %): " << endl;
        cin >> s1[i].marks;
        while (s1[i].marks < 0.0)
        {
            cout << "Enter Valid Marks Percentage: " << endl;
            cin >> s1[i].marks;
        }
        cin.ignore();
    }

    display (s1, max_students);
    
    return 0;
}