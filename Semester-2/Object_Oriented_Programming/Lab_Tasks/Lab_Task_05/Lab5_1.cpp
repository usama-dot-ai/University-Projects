#include <iostream>
#include <cstring>
using namespace std;

class student
{
private:
    string *std_name;
    int std_age;

public:
    student()
    {
        *std_name = "Unknown";
        std_age = 0;
    }
    student(string name, int age)
    {
        std_name = new string(name);
        std_age = age;
    }
    student(student &obj)
    {
        std_name = new string (*(obj.std_name));
        std_age = obj.std_age;
    }
    void display()
    {
        cout << "Student Details: " << endl;
        cout << "Name: " << *std_name << endl;
        cout << "Age: " << std_age << endl;
    }
    ~student()
    {
        delete std_name;
        cout << "Destructor is called. " << endl;
    }
};

int main()
{
    student s1("Usama", 20);
    s1.display();

    student s2(s1);
    s2.display();

    cout << endl;
    return 0;
}