#include <iostream>
#include <cstring>
using namespace std;

class student
{
private:
    int std_id;
    string std_name;

public:
    student()
    {
        std_id = 0;
        std_name = "Unknown";
    }
    student(int id, string name)
    {
        std_id = id;
        std_name = name;
    }
    void display()
    {
        cout << "\nStudent Details: " << endl;
        cout << "ID: " << std_id << endl;
        cout << "Name: " << std_name << endl;
    }
};
class reg : public student
{
private:
    float std_marks;

public:
    reg()
    {
        std_marks = 0.0;
    }
    reg(float marks, int id, string name) : student(id, name)
    {
        std_marks = marks;
    }
    void display()
    {
        student::display();
        cout << "Marks: " << std_marks << "%" << endl;
    }
};
int main()
{
    reg r1(76, 247141, "Usama");
    r1.display();

    cout << endl;
    return 0;
}