#include <iostream>
using namespace std;

template <typename T>
class Box
{
    public:
    T value;
    Box (T v)
    {
        value = v;
    }
    void show()
    {
        cout << "Value: " << value << endl;
    }
};
int main ()
{
    Box <int> intB(10);
    Box <string> strbox("Hello");
    int B.show();

    cout << endl;
    return 0;
}
