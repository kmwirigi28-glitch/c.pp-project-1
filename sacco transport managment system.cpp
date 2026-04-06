#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// ================= VEHICLE =================
class Vehicle {
public:
    string plate;
    string type;
    int seats;
    int trips;
    double earnings;
    string status;

    Vehicle() {}

    Vehicle(string p, string t, int s) {
        plate = p;
        type = t;
        seats = s;
        trips = 0;
        earnings = 0;
        status = "Active";
    }
};

// ================= SYSTEM =================
class SACCO {
private:
    vector<Vehicle> vehicles;

public:
    // ADD
    void add(string plate, string type, int seats) {
        vehicles.push_back(Vehicle(plate, type, seats));
    }

    // FIND
    int find(string plate) {
        for (int i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].plate == plate)
                return i;
        }
        return -1;
    }

    // DISPLAY
    void display() {
        for (auto &v : vehicles) {
            cout << "\nPlate: " << v.plate;
            cout << "\nType: " << v.type;
            cout << "\nSeats: " << v.seats;
            cout << "\nTrips: " << v.trips;
            cout << "\nEarnings: " << v.earnings;
            cout << "\nStatus: " << v.status << endl;
        }
    }

    // EDIT
    void edit(string plate) {
        int i = find(plate);
        if (i == -1) {
            cout << "Not found!\n";
            return;
        }

        cout << "New type: ";
        cin >> vehicles[i].type;

        cout << "New seats: ";
        cin >> vehicles[i].seats;
    }

    // DELETE
    void remove(string plate) {
        int i = find(plate);
        if (i != -1)
            vehicles.erase(vehicles.begin() + i);
    }

    // TRIP + EARNINGS
    void addTrip(string plate, double fare) {
        int i = find(plate);
        if (i != -1) {
            vehicles[i].trips++;
            vehicles[i].earnings += fare;
        }
    }

    // SAVE FILE
    void save() {
        ofstream file("sacco.txt");

        for (auto &v : vehicles) {
            file << v.plate << " "
                 << v.type << " "
                 << v.seats << " "
                 << v.trips << " "
                 << v.earnings << " "
                 << v.status << endl;
        }

        file.close();
        cout << "Saved successfully!\n";
    }

    // LOAD FILE
    void load() {
        ifstream file("sacco.txt");

        if (!file) {
            cout << "No previous data found.\n";
            return;
        }

        vehicles.clear();

        string plate, type, status;
        int seats, trips;
        double earnings;

        while (file >> plate >> type >> seats >> trips >> earnings >> status) {
            Vehicle v;
            v.plate = plate;
            v.type = type;
            v.seats = seats;
            v.trips = trips;
            v.earnings = earnings;
            v.status = status;

            vehicles.push_back(v);
        }

        file.close();
    }
};

// ================= LOGIN =================
bool login(string &role) {
    string user, pass;

    cout << "Username: ";
    cin >> user;

    cout << "Password: ";
    cin >> pass;

    if (user == "admin" && pass == "1234") {
        role = "ADMIN";
        return true;
    }
    else if (user == "staff" && pass == "1111") {
        role = "STAFF";
        return true;
    }

    return false;
}

// ================= MAIN =================
int main() {
    SACCO system;
    system.load();

    string role;

    if (!login(role)) {
        cout << "Invalid login!\n";
        return 0;
    }

    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Edit Vehicle\n";
        cout << "3. Delete Vehicle\n";
        cout << "4. Add Trip\n";
        cout << "5. Display\n";
        cout << "6. Save\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        string plate;

        if (choice == 1 && role == "ADMIN") {
            string type;
            int seats;

            cout << "Plate: ";
            cin >> plate;

            cout << "Type: ";
            cin >> type;

            cout << "Seats: ";
            cin >> seats;

            system.add(plate, type, seats);
        }

        else if (choice == 2 && role == "ADMIN") {
            cout << "Plate: ";
            cin >> plate;
            system.edit(plate);
        }

        else if (choice == 3 && role == "ADMIN") {
            cout << "Plate: ";
            cin >> plate;
            system.remove(plate);
        }

        else if (choice == 4) {
            double fare;

            cout << "Plate: ";
            cin >> plate;

            cout << "Fare: ";
            cin >> fare;

            system.addTrip(plate, fare);
        }

        else if (choice == 5) {
            system.display();
        }

        else if (choice == 6) {
            system.save();
        }

    } while (choice != 0);

    system.save();
    return 0;
}
