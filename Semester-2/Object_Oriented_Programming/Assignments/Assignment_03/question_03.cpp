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

class ShiftSupervisor : public Employee
{
private:
    double annual_salary;
    double annual_bonus;
    double annual_production_goal; 

public:
    ShiftSupervisor() : Employee()
    {
        annual_salary = 0.0;
        annual_bonus = 0.0;
        annual_production_goal = 0.0;
    }
    ShiftSupervisor(string name, int number, string date, double salary, double goal) : Employee()
    {
        setdata(name, number, date);
        annual_salary = salary;
        annual_bonus = 0.0;
        annual_production_goal = goal;
    }
    void setAnnualSalary(double salary)
    {
        annual_salary = salary;
    }
    void setAnnualProductionGoal(double goal)
    {
        annual_production_goal = goal;
    }
    double calculateBonus(double productionAchieved)
    {
        // Calculate 10% bonus if production goal is reached
        if (productionAchieved >= annual_production_goal)
        {
            annual_bonus = 0.10 * annual_salary;
            return annual_bonus;
        }
        else
        {
            annual_bonus = 0.0;
            return annual_bonus;
        }
    }
    void display()
    {
        cout << fixed << setprecision (2) << endl;
        Employee::display();
        cout << "Annual Salary: $" << annual_salary << endl;
        cout << "Annual Bonus: $" << annual_bonus << endl;
    }
};

int main()
{
    string name, date;
    int number;
    double production_Achieved, salary;

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
    cout << "Enter Date of Hire: ";
    getline(cin, date);

    cout << "Enter Annual Salary: ";
    cin >> salary;
    while (salary < 0)
    {
        cout << "Enter Valid Annual Salary: ";
        cin >> salary;
    }

    cout << "Enter Annual Production (Units) Achieved: ";
    cin >> production_Achieved;
    while (production_Achieved < 0)
    {
        cout << "Enter Positive Annual Production (Units) Achieved: ";
        cin >> production_Achieved;
    }

    ShiftSupervisor supervisor(name, number, date, salary, 10000);

    double bonus = supervisor.calculateBonus(production_Achieved);
    if (bonus > 0)
    {
        cout << "Bonus validated! Annual Bonus: $" << bonus << endl;
    }
    else
    {
        cout << "Production goal not achieved. No bonus granted." << endl;
    }

    supervisor.display();

    cout << endl;
    return 0;
}
