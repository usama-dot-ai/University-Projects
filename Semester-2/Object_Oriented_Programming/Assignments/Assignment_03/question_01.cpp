#include <iostream>
#include <cstring>
using namespace std;

class Publication
{
private:
    string title;
    float price;

public:
    void setdata(string t, float p)
    {
        title = t;
        price = p;
    }
    void getdata()
    {
        cout << "\nTitle: " << title << endl;
        cout << "Price: " << price << "$" << endl;
    }
};

class Book : public Publication
{
private:
    int page_count;

public:
    void setdata(int p_c, string t, float p)
    {
        Publication ::setdata(t, p);
        page_count = p_c;
    }
    void getdata()
    {
        string title;
        float price;
        cout << "\n--- Enter Book Details ---" << endl;

        cout << "Enter title of the book: ";
        getline(cin, title);

        cout << "Enter Price of the book: ";
        cin >> price;
        while (price < 0)
        {
            cout << "Kindly Enter Positive Price: ";
            cin >> price;
        }

        cout << "Enter Page Count of the book: ";
        cin >> page_count;
        while (page_count < 0)
        {
            cout << "Kindly Enter Positive Page Count: ";
            cin >> page_count;
        }

        setdata(page_count, title, price);
    }

    void putdata()
    {
        Publication ::getdata();
        cout << "Total Pages: " << page_count << endl;
    }
};

class Tape : public Publication
{
private:
    float play_time;

public:
    void setdata(float p_t, string t, float p)
    {
        Publication ::setdata(t, p);
        play_time = p_t;
    }
    void getdata()
    {
        string title;
        float price;
        cout << "\n--- Enter Tape Details ---" << endl;

        cout << "Enter title of the tape: ";
        cin.ignore();
        getline(cin, title);

        cout << "Enter Price of the tape: ";
        cin >> price;
        while (price < 0)
        {
            cout << "Kindly Enter Positive Price: ";
            cin >> price;
        }

        cout << "Enter Playtime for the tape: ";
        cin >> play_time;
        while (play_time < 0)
        {
            cout << "Kindly Enter Positive Playtime: ";
            cin >> play_time;
        }

        setdata(play_time, title, price);
    }

    void putdata()
    {
        Publication::getdata();
        cout << "Total Play Time: " << play_time << " minutes" << endl;
    }
};

int main()
{
    Book b;
    b.getdata();

    Tape t;
    t.getdata();

    b.putdata();
    t.putdata();

    cout << endl;
    return 0;
}
