#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
    string name;
    int age;
    float marks[3];

public:
    Student() // Default constructor
    {
        name = "";
        age = 0;
        marks[0] = 0.0;
        marks[1] = 0.0;
        marks[2] = 0.0;
    }
    Student(string n, int a, float m1, float m2, float m3) // Parameterized constructor
    {
        name = n;
        age = a;
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
    }

    friend float calculateTotalMarks(Student &student); // Friend function declaration

    void displayDetails() // Function to display student details
    {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Marks in Subjects: " << marks[0] << ", " << marks[1] << ", " << marks[2] << endl;
    }
};

// Friend function definition to calculate total marks
float calculateTotalMarks(Student &student)
{
    return student.marks[0] + student.marks[1] + student.marks[2];
}

int main()
{
    int numStudents;

    cout << "\nEnter the number of students: ";
    cin >> numStudents;
    cin.ignore();

    // Dynamically allocate memory for Student objects
    Student *students = new Student[numStudents];

    for (int i = 0; i < numStudents; i++) // Input details for each student
    {
        string name;
        int age;
        float marks[3];

        cout << "Enter details for student " << (i + 1) << ":" << endl;
        cout << "Name: ";
        getline(cin, name);
        cout << "Age: ";
        cin >> age;
        cout << "Enter marks for three subjects: " << endl;
        for (int j = 0; j < 3; j++)
        {
            cout << "Subject " << (j + 1) << ": ";
            cin >> marks[j];
        }
        cin.ignore();

        // Create a Student object and store it in the array
        students[i] = Student(name, age, marks[0], marks[1], marks[2]);
    }

    // Display details of each student and calculate total marks
    cout << "\nStudent Details:" << endl;
    for (int i = 0; i < numStudents; i++)
    {
        cout << "\nDetails of Student " << (i + 1) << ":" << endl;
        students[i].displayDetails();
        float totalMarks = calculateTotalMarks(students[i]);
        cout << "Total Marks: " << totalMarks << endl;
    }

    // Free dynamically allocated memory
    delete[] students;

    cout << endl;
    return 0;
}