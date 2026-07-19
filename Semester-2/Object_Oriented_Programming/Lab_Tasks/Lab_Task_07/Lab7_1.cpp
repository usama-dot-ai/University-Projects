#include <iostream>
#include <cstring>
using namespace std;

class student
{
private:
    int std_id;
    string std_name;

public:
    void setdata(int id, string name);
    void getdata();
};
void student ::setdata(int id, string name)
{
    std_id = id;
    std_name = name;
}
void student::getdata()
{
    cout << "\nStudent Details: " << endl;
    cout << "ID: " << std_id << endl;
    cout << "Name: " << std_name << endl;
}
class marks
{
private:
    float OOP, DLD, AP;

public:
    void setdata(float oop, float dld, float ap);
    void getdata();
};
void marks ::setdata(float oop, float dld, float ap)
{
    OOP = oop;
    DLD = dld;
    AP = ap;
}
void marks::getdata()
{
    cout << "OOP Marks: " << OOP << endl;
    cout << "DLD Marks: " << DLD << endl;
    cout << "Physics Marks: " << AP << endl;
}
class display : public student, public marks
{
public:
    void getdata()
    {
        student ::getdata();
        marks ::getdata();
    }
};
int main()
{
    display d1;
    d1.student ::setdata(247141, "Usama");
    d1.marks ::setdata(78, 85, 60);
    d1.getdata();

    cout << endl;
    return 0;
}