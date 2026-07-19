#include <iostream>
#include <cstring>
using namespace std;

class Person
{
private:
    string Name;
    static int count_person;

public:
    Person() : Name("Unknown")
    {
        count_person++;
    }
    Person(string name) : Name(name)
    {
        count_person++;
    }
    void displayInfo() const
    {
        cout << "\nName: " << Name << endl;
    }
    static int getCount()
    {
        return count_person;
    }
};

int Person::count_person = 0;

class Employee : public Person
{
protected:
    int employeeID;

public:
    Employee() : Person(), employeeID(0)
    {
    }

    Employee(string name, int emp_id) : Person(name), employeeID(emp_id)
    {
    }
    void displayInfo()
    {
        Person::displayInfo();
        cout << "id: " << employeeID << endl;
    }
};

int main ()
{
    Employee emp1("Usama Arshad", 247141);
    Employee emp2("Asad Farooq", 247171);

    emp1.displayInfo();
    emp2.displayInfo();
    cout << "\nTotal Objects Created: " << Person::getCount() << endl;

    cout << endl;
    return 0;
}