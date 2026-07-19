#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

class Shape
{
private:
    string shape_color;

public:
    Shape()
    {
        shape_color = "Undefined";
    }
    void setdata(string clr)
    {
        shape_color = clr;
    }
    virtual double area()
    {
        return -1;
    }
    virtual void display()
    {
        cout << "Color: " << shape_color << endl;
    }
};

class Triangle : public Shape
{
private:
    double tri_base, tri_height, tri_sides;

public:
    Triangle()
    {
        tri_base = 0.0;
        tri_height = 0.0;
    }
    void setdata(double b, double h, double ns, string clr)
    {
        tri_base = b;
        tri_height = h;
        tri_sides = ns;
        Shape::setdata(clr);
    }
    void getdata()
    {
        double base, height, sides;
        string color;

        cout << "\nEnter Color of Triangle: ";
        getline(cin, color);

        cout << "Enter base of triangle: ";
        cin >> base;
        while (base < 1)
        {
            cout << "Kindly Enter Positive Base: ";
            cin >> base;
        }

        cout << "Enter Height of Triangle: ";
        cin >> height;
        while (height < 0)
        {
            cout << "Kindly Enter Positive Height: ";
            cin >> height;
        }

        cout << "Enter Number of Sides of Triangle: ";
        cin >> sides;
        while (sides < 0)
        {
            cout << "Kindly Enter Positive Sides: ";
            cin >> sides;
        }

        setdata(base, height, sides, color);
    }
    double area() override
    {
        return (0.5 * tri_base * tri_height);
    }
    void display() override
    {
        cout << fixed << setprecision(2) << endl;
        cout << "\n --- Triangle Details --- " << endl;
        cout << "Base: " << tri_base << endl;
        cout << "Height: " << tri_height << endl;
        cout << "Number of Sides: " << tri_sides << endl;
        Shape::display();
        cout << "Area: " << area() << endl;
    }
};

class Circle : public Shape
{
private:
    double radius, center_X, center_Y;

public:
    void setdata(double r, double cX, double cY, string clr)
    {
        radius = r;
        center_X = cX;
        center_Y = cY;
        Shape::setdata(clr);
    }
    void getdata()
    {
        double rad, centerX, centerY;
        string color;

        cout << "\nEnter Color of Circle: ";
        cin.ignore();
        getline(cin, color);

        cout << "Enter Radius of Circle: ";
        cin >> rad;
        while (rad < 0)
        {
            cout << "Kindly Enter Positive Radius: ";
            cin >> rad;
        }

        cout << "Enter Center X Coordinate of Circle: ";
        cin >> centerX;

        cout << "Enter Center Y Coordinate of Circle: ";
        cin >> centerY;

        setdata(rad, centerX, centerY, color);
    }
    double area() override
    {
        return (3.14 * radius * radius);
    }
    double circumference()
    {
        return (2 * 3.14 * radius);
    }
    double diameter()
    {
        return (2 * radius);
    }
    void display() override
    {
        cout << fixed << setprecision(2) << endl;
        cout << "\n --- Circle Details --- " << endl;
        cout << "X Coordinate: " << center_X << endl;
        cout << "Y Coordinate: " << center_Y << endl;
        cout << "Radius: " << radius << endl;
        Shape::display();
        cout << "Area: " << area() << endl;
        cout << "Diameter: " << diameter() << endl;
        cout << "Circumference: " << circumference() << endl;
    }
};

int main()
{
    Triangle t;
    t.getdata();

    Circle c;
    c.getdata();

    t.display();
    c.display();

    cout << endl;
    return 0;
}
