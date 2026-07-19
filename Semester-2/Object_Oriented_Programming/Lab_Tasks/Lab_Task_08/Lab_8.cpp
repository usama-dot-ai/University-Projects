#include <iostream>
#include <cstring>
using namespace std;

class Student
{
private:
    string std_name;
    int std_rollNo, std_age;

public:
    void setdata(string name, int rollNo, int age)
    {
        std_name = name;
        std_rollNo = rollNo;
        std_age = age;
    }
    void display()
    {
        cout << "\n ---Student Details ---" << endl;
        cout << "Name: " << std_name << endl;
        cout << "Age: " << std_age << endl;
        cout << "Roll No: " << std_rollNo << endl;
    }
};

class Exams : virtual public Student
{
protected:
    float exam_marks;

public:
    void setdata(float marks)
    {
        exam_marks = marks;
    }

    void display()
    {
        cout << "Exam Marks: " << exam_marks << endl;
    }
};

class Sport : virtual public Student
{
protected:
    float sport_marks;

public:
    void setdata(float marks)
    {
        sport_marks = marks;
    }
    void display()
    {
        cout << "Sport Marks: " << sport_marks << endl;
    }
};

class Result : public Exams, public Sport
{
private:
    float total_marks;

public:
    void setdata(float t_marks, float e_marks, float s_marks, string name, int rollNo, int age)
    {
        total_marks = t_marks;
        Sport::setdata(s_marks);
        Exams::setdata(e_marks);
        Student::setdata(name, rollNo, age);
    }
    void getdata()
    {
        float e_marks, s_marks;
        int rollNo, age;
        string name;

        cout << "\nEnter Student Name: ";
        getline(cin, name);

        cout << "Enter Student Age: ";
        cin >> age;
        while (age < 0)
        {
            cout << "kindly Enter Positive Age: ";
            cin >> age;
        }

        cout << "Enter Student Roll No: ";
        cin >> rollNo;
        while (rollNo < 0)
        {
            cout << "kindly Enter Positive Roll No: ";
            cin >> rollNo;
        }

        cout << "Enter Exam Marks: ";
        cin >> e_marks;
        while (e_marks < 0)
        {
            cout << "Enter Positive Exam Marks: ";
            cin >> e_marks;
        }

        cout << "Enter Sport Marks: ";
        cin >> s_marks;
        while (s_marks < 0)
        {
            cout << "Enter Positive Sports Marks: ";
            cin >> s_marks;
        }

        setdata(total(), e_marks, s_marks, name, rollNo, age);
    }
    float total()
    {
        return total_marks = exam_marks + sport_marks;
    }
    void display()
    {
        Student::display();
        Exams::display();
        Sport::display();
        cout << "Total Marks: " << total() << endl;
    }
};

int main()
{
    Result r1;
    r1.getdata();
    r1.display();

    cout << endl;
    return 0;
}
