/* 
    Event Management System

    Zaeem Furqan | 247169
    Usama Arshad | 247141

*/

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <regex>
#include <ctime>

using namespace std;

// Forward declaration
class Event;

// Global container for events
vector<unique_ptr<Event>> events;
// Global container for user-selected venues
vector<unique_ptr<class Venue>> selectedVenues;

// Fixed venues list
const vector<pair<string, int>> fixedVenues = {
    {"Auditorium", 200},
    {"Sports Complex", 1000},
    {"Grand Hall", 800},
    {"Ballroom", 500},
    {"Hotel Banquet Hall", 1000},
    {"Grand Dining Hall", 400},
    {"Lecture Theatre", 300},
    {"Conference Room", 500},
    {"Seminar Room", 100},
    {"Departmental Auditorium", 150},
    {"Classroom", 35},
    {"Computer Lab", 40},
    {"Exhibition Space", 300},
    {"Marquee/Outdoor Event Space", 500}};

// ------------------- Venue -------------------
class Venue
{
private:
    const int id;
    static int total_venues;
    string name;
    int capacity;

public:
    Venue(string n, int c) : id(total_venues++), name(n), capacity(c) {}

    string getName() const { return name; }
    int getCapacity() const { return capacity; }

    void displaydetails() const
    {
        cout << "Venue Name: " << name << endl;
        cout << "Capacity: " << capacity << endl;
    }
};
int Venue::total_venues = 0;

// ------------------- Organizer -------------------
class Organizer
{
private:
    string name;

public:
    Organizer(string n) : name(n) {}

    string getName() const { return name; }

    static unique_ptr<Organizer> create_organizer()
    {
        string n;
        cout << "Enter Organizer Name: ";
        getline(cin, n);
        return make_unique<Organizer>(n);
    }

    virtual void displaydetails() const
    {
        cout << "Organizer Name: " << name << endl;
    }

    virtual ~Organizer() = default;
};

// ------------------- Attendee -------------------
class Attendee
{
private:
    static int total_attendees;

public:
    Attendee()
    {
        total_attendees++;
    }

    static unique_ptr<Attendee> create_attendee()
    {
        return make_unique<Attendee>();
    }

    static int get_total_attendees()
    {
        return total_attendees;
    }
};
int Attendee::total_attendees = 0;

// ------------------- Event Base -------------------
class Event
{
protected:
    static int total_events;
    string date;
    unique_ptr<Organizer> organizer;

    // Store venues and corresponding attendees counts
    struct VenueAttendees
    {
        unique_ptr<Venue> venue;
        int attendeeCount;
    };
    vector<VenueAttendees> venuesWithAttendees;

public:
    Event() {}

    virtual void inputdetails() = 0;
    virtual void displaydetails() const = 0;

    static void create_event(int event_number); // defined later

    virtual ~Event() = default;

    // Utility functions for validation
    static bool isValidName(const string &name)
    {
        return !name.empty();
    }

    static bool isValidDate(const string &date)
    {
        regex datePattern(R"(\d{2}-\d{2}-\d{4})");
        if (!regex_match(date, datePattern))
            return false;

        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        // Check month range
        if (month < 1 || month > 12)
            return false;

        // Check day range based on month
        if (day < 1 || day > 31)
            return false;

        // Days in each month
        const int daysInMonth[] = {0, 31, 28 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)),
                                   31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (day > daysInMonth[month])
            return false;

        return true;
    }

    static bool isAtLeastOneWeekAhead(const string &date)
    {
        // Get current date
        time_t t = time(0);
        tm *now = localtime(&t);
        now->tm_mday += 7; // Add 7 days
        mktime(now);       // Normalize the date

        // Convert to YYYY-MM-DD format for comparison
        char currentDate[11];
        strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", now);

        // Convert input date to YYYY-MM-DD format
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        char inputDate[11];
        snprintf(inputDate, sizeof(inputDate), "%04d-%02d-%02d", year, month, day);

        return strcmp(inputDate, currentDate) > 0; // Compare dates
    }
};
int Event::total_events = 0;

// --------------- Utility function to select venues ---------------
void select_venues()
{
    cout << "\n============== Venue Selection ==============\n";
    cout << "Available Venues:\n";
    for (size_t i = 0; i < fixedVenues.size(); ++i)
    {
        cout << i + 1 << ". " << fixedVenues[i].first << " (Capacity: " << fixedVenues[i].second << ")\n";
    }

    int n;
    cout << "How many venues do you want to select (1-" << fixedVenues.size() << ")? ";
    cin >> n;
    while (n < 1 || n > (int)fixedVenues.size())
    {
        cout << "Invalid number. Please enter between 1 and " << fixedVenues.size() << ": ";
        cin >> n;
    }

    vector<bool> chosen(fixedVenues.size(), false);
    selectedVenues.clear();

    for (int i = 0; i < n; ++i)
    {
        int choice;
        cout << "Select Venue #" << (i + 1) << ": ";
        cin >> choice;
        while (choice < 1 || choice > (int)fixedVenues.size() || chosen[choice - 1])
        {
            if (choice >= 1 && choice <= (int)fixedVenues.size() && chosen[choice - 1])
                cout << "Venue already selected. Choose a different one: ";
            else
                cout << "Invalid choice. Enter a number between 1 and " << fixedVenues.size() << ": ";
            cin >> choice;
        }
        chosen[choice - 1] = true;
        selectedVenues.push_back(make_unique<Venue>(fixedVenues[choice - 1].first, fixedVenues[choice - 1].second));
    }
    cin.ignore(); // clear input buffer
}

// Drive class Convocation Ceremony
class ConvocationCeremony : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Convocation Ceremony Details ------\n";

        while (true)
        {
            cout << "Enter Event Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }

        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << " (max " << venueCopy->getCapacity() << "): ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Convocation Ceremony Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Gala Dinner
class GalaDinner : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Gala Dinner Details ------\n";
        while (true)
        {
            cout << "Enter Event Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Gala Dinner Summary ------\n";
        cout << "\nDate: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Distinguished Lecture
class DistinguishedLecture : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Distinguished Lecture Details ------\n";
        while (true)
        {
            cout << "Enter Lecture Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Distinguished Lecture Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Research Colloquium
class ResearchColloquium : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Research Colloquium Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Research Colloquium Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Academic Workshop
class AcademicWorkshop : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Academic Workshop Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Academic Workshop Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Guest Speaker Session
class GuestSpeakerSession : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Guest Speaker Session Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Guest Speaker Session Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Career Fair
class CareerFair : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Career Fair Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Career Fair Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Debate Competition
class DebateCompetition : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Debate Competition Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Debate Competition Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Graduation Ball
class GraduationBall : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Graduation Ball Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Graduation Ball Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// Drive class Poster Presentation Session
class PosterPresentationSession : public Event
{
public:
    void inputdetails() override
    {
        cout << "\n------ Poster Presentation Session Details ------\n";
        while (true)
        {
            cout << "Enter Date (DD-MM-YYYY): ";
            getline(cin, date);
            if (isValidDate(date) && isAtLeastOneWeekAhead(date))
                break;
            cout << "Invalid format or date must be at least one week ahead.\n";
        }
        organizer = Organizer::create_organizer();
        venuesWithAttendees.clear();
        for (const auto &venue : selectedVenues)
        {
            auto venueCopy = make_unique<Venue>(venue->getName(), venue->getCapacity());
            int attendees;
            while (true)
            {
                cout << "Attendees at " << venueCopy->getName() << ": ";
                cin >> attendees;
                cin.ignore();
                if (attendees >= 0 && attendees <= venueCopy->getCapacity())
                    break;
                cout << "Invalid count.\n";
            }
            venuesWithAttendees.push_back({std::move(venueCopy), attendees});
        }
    }

    void displaydetails() const override
    {
        cout << "\n------ Poster Presentation Session Summary ------\n";
        cout << "Date: " << date << "\nOrganizer: " << organizer->getName() << endl;
        for (const auto &va : venuesWithAttendees)
            cout << "Venue: " << va.venue->getName() << ", Attendees: " << va.attendeeCount << endl;
    }
};

// ------------------- Event::create_event Implementation -------------------
void Event::create_event(int event_number)
{
    int choice;
    cout << "\n====================== Select Event Type For Event #" << event_number << " ======================\n";
    cout << "1. Convocation Ceremony\n"
         << "2. Gala Dinner\n"
         << "3. Distinguished Lecture\n"
         << "4. Research Colloquium\n"
         << "5. Academic Workshop\n"
         << "6. Guest Speaker Session\n"
         << "7. Career Fair\n"
         << "8. Debate Competition\n"
         << "9. Graduation Ball\n"
         << "10. Poster Presentation Session\n";
    cout << "Enter The Event Type Number That You Want To Create: ";
    cin >> choice;
    cin.ignore();

    while (choice < 1 || choice > 10)
    {
        cout << "Invalid choice. Please select a number between 1 and 10: ";
        cin >> choice;
        cin.ignore();
    }

    // Let user select venues first
    select_venues();

    unique_ptr<Event> event;

    switch (choice)
    {
    case 1:
    {
        event = make_unique<ConvocationCeremony>();
        break;
    }
    case 2:
    {
        event = make_unique<GalaDinner>();
        break;
    }
    case 3:
    {
        event = make_unique<DistinguishedLecture>();
        break;
    }
    case 4:
    {
        event = make_unique<ResearchColloquium>();
        break;
    }
    case 5:
    {
        event = make_unique<AcademicWorkshop>();
        break;
    }
    case 6:
    {
        event = make_unique<GuestSpeakerSession>();
        break;
    }
    case 7:
    {
        event = make_unique<CareerFair>();
        break;
    }
    case 8:
    {
        event = make_unique<DebateCompetition>();
        break;
    }
    case 9:
    {
        event = make_unique<GraduationBall>();
        break;
    }
    case 10:
    {
        event = make_unique<PosterPresentationSession>();
        break;
    }
    default:
    {
        cout << "Invalid choice.\n";
        return;
    }
    }

    event->inputdetails();
    events.push_back(move(event));
}

// ------------------- Main -------------------
int main()
{
    cout << "\n===================Event Management System===================\n";

    cout << "How many events do you want to create? ";
    int count;
    cin >> count;
    cin.ignore();

    for (int i = 0; i < count; i++)
    {
        Event::create_event(i + 1);
    }

    cout << "\n=============== Event Summary ===============\n";
    for (const auto &e : events)
    {
        e->displaydetails();
        cout << "----------------------------------------\n";
    }

    return 0;
}