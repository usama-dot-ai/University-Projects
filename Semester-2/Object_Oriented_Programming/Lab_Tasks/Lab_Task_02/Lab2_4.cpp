#include <iostream>
#include <cstring>
using namespace std;

class student
{
private:
    string name;
    int id;
    float subject_1, subject_2, subject_3;

public:
    student()  // default constructor
    {
        name = "Unknown";
        id = 0;
        subject_1 = 0.0;
        subject_2 = 0.0;
        subject_3 = 0.0;
    };

    void setdata(string n, int i, float s1, float s2, float s3)
    {
        name = n;
        id = i;
        subject_1 = s1;
        subject_2 = s2;
        subject_3 = s3;
    }
    float total()
    {
        return subject_1 + subject_2 + subject_3;
    }
    void display()
    {
        cout << "\nStudent Details: " << endl;
        cout << "\nName: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Marks Obtained: " << total() << endl;
    }
};

int main()
{
    student s1;

    string name;
    int id;
    float sub_1, sub_2, sub_3;

    cout << "\nEnter Student Name: ";
    getline(cin, name);

    cout << "Enter Student ID: ";
    cin >> id;
    while (id <= 0)
    {
        cout << "Enter Valid ID: ";
        cin >> id;
    }

    cout << "Enter Marks for Subject 1: ";
    cin >> sub_1;
    while (sub_1 < 0 || sub_1 > 100)
    {
        cout << "Enter Valid Marks: ";
        cin >> sub_1;
    }

    cout << "Enter Marks for Subject 2: ";
    cin >> sub_2;
    while (sub_2 < 0 || sub_2 > 100)
    {
        cout << "Enter Valid Marks: ";
        cin >> sub_2;
    }

    cout << "Enter Marks for Subject 3: ";
    cin >> sub_3;
    while (sub_3 < 0 || sub_3 > 100)
    {
        cout << "Enter Valid Marks: ";
        cin >> sub_3;
    }

    s1.setdata(name, id, sub_1, sub_2, sub_3);
    s1.total();
    s1.display();

    cout << endl;
    return 0;
}