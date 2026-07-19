#include <iostream>
using namespace std;

class Area
{
private:
    float length;
    float width;

public:
    Area() // default constructor
    {
        length = 1.0;
        width = 1.0;
    }
    Area(float l, float w) // parameterized constructor
    {
        length = l;
        width = w;
    }
    float calculate(float l, float w)
    {
        return l * w;
    }
    void display()
    {
        cout << "\nReceived Values: " << endl;
        cout << "Length: " << length << endl;
        cout << "Width: " << width << endl;
        cout << "Area: " << calculate(length, width) << endl;
    }
};

int main()
{
    Area a1(23.12, 45.67);
    a1.calculate(23.12, 45.67);
    a1.display();

    cout << endl;
    return 0;
}