#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Employee
{
private:
    string employee_Name;
    int employee_Number;
    string hire_Date;

public:
    Employee()
    {
        employee_Name = "Unknown";
        employee_Number = 0;
        hire_Date = "Unknown";
    }
    void setdata(string name, int number, string date)
    {
        employee_Name = name;
        employee_Number = number;
        hire_Date = date;
    }
    void display()
    {
        cout << "\n --- Employee Details --- " << endl;
        cout << "Employee Name: " << employee_Name << endl;
        cout << "Employee Number: " << employee_Number << endl;
        cout << "Hire Date: " << hire_Date << endl;
    }
};

class ProductionWorker : public Employee
{
private:
    int shift;
    double hourly_Pay;

public:
    ProductionWorker() : Employee()
    {
        shift = 0;
        hourly_Pay = 0.0;
    }

    void setdata(string name, int number, string date, int s, double pay)
    {
        Employee::setdata(name, number, date);
        shift = s;
        hourly_Pay = pay;
    }
    void getdata()
    {
        string name, date;
        int number, shift;
        double pay;

        cout << "\nEnter Employee Name: ";
        getline(cin, name);

        cout << "Enter Employee Number: ";
        cin >> number;
        while (number <= 0)
        {
            cout << "Enter a valid Employee Number: ";
            cin >> number;
        }
        cin.ignore();

        cout << "Enter Hire Date (DD-MM-YYYY): ";
        getline(cin, date);

        cout << "Enter Shift Number (1 or 2): ";
        cin >> shift;
        while (shift != 1 && shift != 2)
        {
            cout << "Enter a valid Shift Number (1 or 2): ";
            cin >> shift;
        }

        cout << "Enter Hourly Pay Rate: ";
        cin >> pay;
        while (pay < 0)
        {
            cout << "Enter Positive Hourly Pay Rate: ";
            cin >> pay;
        }
        setdata(name, number, date, shift, pay);
    }
    void display()
    {
        cout << fixed << setprecision(2) << endl;
        Employee::display();
        cout << "Shift: " << shift << endl;
        cout << "Hourly Pay Rate: $" << hourly_Pay << endl;
    }
};

int main()
{
    ProductionWorker worker;
    worker.getdata();
    worker.display();

    cout << endl;
    return 0;
}
