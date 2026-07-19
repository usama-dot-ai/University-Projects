#include <iostream>
using namespace std;

class name
{
private:
    string std_name;

public:
    void setName(string name)
    {
        std_name = name;
    }
    void displayName()
    {
        cout << "\nName: " << std_name << endl;
    }
};

class age : public name
{
private:
    int std_age;

public:
    void setAge(int age)
    {
        std_age = age;
    }
    void displayAge()
    {
        cout << "Age: " << std_age << endl;
    }
};

int main()
{
    age a1;
    
    a1.setName("Usama");
    a1.displayName();
    a1.setAge(20);
    a1.displayAge();

    cout << endl;
    return 0;
}