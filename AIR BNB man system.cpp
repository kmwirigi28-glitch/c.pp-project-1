#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// ================= PROPERTY =================
class Property {
public:
    string name;
    string location;
    double price;
    bool available;

    Property() {}

    Property(string n, string l, double p) {
        name = n;
        location = l;
        price = p;
        available = true;
    }
};

// ================= BOOKING =================
class Booking {
public:
    string customer;
    string property;
    int days;
    double total;
};

// ================= SYSTEM =================
class Airbnb {
private:
    vector<Property> properties;
    vector<Booking> bookings;

public:

    // ADD PROPERTY
    void addProperty() {
        string name, location;
        double price;

        cin.ignore();

        cout << "Enter name: ";
        getline(cin, name);

        cout << "Enter location: ";
        getline(cin, location);

        cout << "Enter price: ";
        cin >> price;

        properties.push_back(Property(name, location, price));
        cout << "Property added!\n";
    }

    // DISPLAY
    void display() {
        if (properties.empty()) {
            cout << "No properties available!\n";
            return;
        }

        for (int i = 0; i < properties.size(); i++) {
            cout << "\nID: " << i;
            cout << "\nName: " << properties[i].name;
            cout << "\nLocation: " << properties[i].location;
            cout << "\nPrice: " << properties[i].price;
            cout << "\nStatus: "
                 << (properties[i].available ? "Available" : "Booked")
                 << endl;
        }
    }

    // BOOK PROPERTY
    void book(string customerName) {
        int id, days;

        cout << "Enter property ID: ";
        cin >> id;

        if (id < 0 || id >= properties.size()) {
            cout << "Invalid ID!\n";
            return;
        }

        if (!properties[id].available) {
            cout << "Already booked!\n";
            return;
        }

        cout << "Enter days: ";
        cin >> days;

        double total = properties[id].price * days;
        properties[id].available = false;

        Booking b;
        b.customer = customerName;
        b.property = properties[id].name;
        b.days = days;
        b.total = total;

        bookings.push_back(b);

        cout << "Booked successfully!\n";
        cout << "Total cost: " << total << endl;
    }

    // CANCEL
    void cancel() {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        if (id < 0 || id >= properties.size()) {
            cout << "Invalid ID!\n";
            return;
        }

        properties[id].available = true;
        cout << "Booking cancelled!\n";
    }

    // SEARCH
    void search() {
        string loc;
        cin.ignore();

        cout << "Enter location: ";
        getline(cin, loc);

        bool found = false;

        for (int i = 0; i < properties.size(); i++) {
            if (properties[i].location == loc && properties[i].available) {
                cout << "\nID: " << i;
                cout << "\nName: " << properties[i].name;
                cout << "\nPrice: " << properties[i].price << endl;
                found = true;
            }
        }

        if (!found)
            cout << "No matching properties!\n";
    }

    // EDIT
    void edit() {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        if (id < 0 || id >= properties.size()) {
            cout << "Invalid ID!\n";
            return;
        }

        cin.ignore();

        cout << "New name: ";
        getline(cin, properties[id].name);

        cout << "New location: ";
        getline(cin, properties[id].location);

        cout << "New price: ";
        cin >> properties[id].price;

        cout << "Updated!\n";
    }

    // DELETE
    void remove() {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        if (id < 0 || id >= properties.size()) {
            cout << "Invalid ID!\n";
            return;
        }

        properties.erase(properties.begin() + id);
        cout << "Deleted!\n";
    }

    // SHOW BOOKINGS
    void showBookings() {
        if (bookings.empty()) {
            cout << "No bookings yet!\n";
            return;
        }

        for (auto &b : bookings) {
            cout << "\nCustomer: " << b.customer;
            cout << "\nProperty: " << b.property;
            cout << "\nDays: " << b.days;
            cout << "\nTotal: " << b.total << endl;
        }
    }

    // SAVE
    void save() {
        ofstream file("airbnb.txt");

        for (auto &p : properties) {
            file << p.name << "|"
                 << p.location << "|"
                 << p.price << "|"
                 << p.available << endl;
        }

        file.close();
    }

    // LOAD
    void load() {
        ifstream file("airbnb.txt");

        if (!file) return;

        properties.clear();

        string line;

        while (getline(file, line)) {
            Property p;

            int pos1 = line.find('|');
            int pos2 = line.find('|', pos1 + 1);
            int pos3 = line.find('|', pos2 + 1);

            p.name = line.substr(0, pos1);
            p.location = line.substr(pos1 + 1, pos2 - pos1 - 1);
            p.price = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
            p.available = stoi(line.substr(pos3 + 1));

            properties.push_back(p);
        }

        file.close();
    }
};

// ================= LOGIN =================
bool login(string &role, string &username) {
    string user, pass;

    cout << "Username: ";
    cin >> user;

    cout << "Password: ";
    cin >> pass;

    if (user == "admin" && pass == "1234") {
        role = "ADMIN";
        username = user;
        return true;
    }

    if (user == "user" && pass == "1111") {
        role = "CUSTOMER";
        username = user;
        return true;
    }

    cout << "Login failed!\n";
    return false;
}

// ================= MAIN =================
int main() {
    Airbnb system;
    system.load();

    string role, username;

    if (!login(role, username)) return 0;

    int choice;

    do {
        cout << "\n===== AIRBNB SYSTEM =====\n";

        if (role == "ADMIN") {
            cout << "1. Add Property\n";
            cout << "2. View Properties\n";
            cout << "3. Edit\n";
            cout << "4. Delete\n";
            cout << "5. View Bookings\n";
            cout << "6. Save\n";
        }

        if (role == "CUSTOMER") {
            cout << "1. View Properties\n";
            cout << "2. Book Property\n";
            cout << "3. Search\n";
            cout << "4. Cancel Booking\n";
        }

        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (role == "ADMIN") {
            switch (choice) {
                case 1: system.addProperty(); break;
                case 2: system.display(); break;
                case 3: system.edit(); break;
                case 4: system.remove(); break;
                case 5: system.showBookings(); break;
                case 6: system.save(); break;
            }
        }

        if (role == "CUSTOMER") {
            switch (choice) {
                case 1: system.display(); break;
                case 2: system.book(username); break;
                case 3: system.search(); break;
                case 4: system.cancel(); break;
            }
        }

    } while (choice != 0);

    system.save();
    return 0;
}