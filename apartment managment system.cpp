#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// ================= CLIENT =================
class Client {
public:
    string name;
    double rentPaid;
    double rentDue;

    Client() {
        name = "";
        rentPaid = 0;
        rentDue = 0;
    }

    Client(string n, double due) {
        name = n;
        rentPaid = 0;
        rentDue = due;
    }

    void makePayment(double amount) {
        rentPaid += amount;
    }

    double balance() {
        return rentDue - rentPaid;
    }
};

// ================= ROOM =================
class Room {
public:
    int roomNumber;
    bool isOccupied;
    Client client;

    Room() {
        roomNumber = 0;
        isOccupied = false;
    }

    Room(int num) {
        roomNumber = num;
        isOccupied = false;
    }

    void assignClient(Client c) {
        client = c;
        isOccupied = true;
    }
};

// ================= HOUSE =================
class House {
public:
    string name;
    vector<Room> rooms;

    House() {}

    House(string n) {
        name = n;
    }

    void addRoom(int num) {
        rooms.push_back(Room(num));
    }
};

// ================= SYSTEM =================
class SystemManager {
private:
    vector<House> houses;

public:
    // Add House
    void addHouse(string name) {
        houses.push_back(House(name));
    }

    // Add Room
    void addRoom(int houseIndex, int roomNumber) {
        if (houseIndex < houses.size())
            houses[houseIndex].addRoom(roomNumber);
    }

    // Add Tenant
    void addTenant(int houseIndex, int roomIndex, string name, double rent) {
        Client c(name, rent);
        houses[houseIndex].rooms[roomIndex].assignClient(c);
    }

    // Payment
    void makePayment(int houseIndex, int roomIndex, double amount) {
        houses[houseIndex].rooms[roomIndex].client.makePayment(amount);
    }

    // Search Tenant
    void searchTenant(string name) {
        for (int i = 0; i < houses.size(); i++) {
            for (int j = 0; j < houses[i].rooms.size(); j++) {
                if (houses[i].rooms[j].isOccupied &&
                    houses[i].rooms[j].client.name == name) {

                    cout << "\nFOUND:\n";
                    cout << "House: " << houses[i].name << endl;
                    cout << "Room: " << houses[i].rooms[j].roomNumber << endl;
                    cout << "Rent Paid: " << houses[i].rooms[j].client.rentPaid << endl;
                    cout << "Balance: " << houses[i].rooms[j].client.balance() << endl;
                    return;
                }
            }
        }
        cout << "Tenant not found!\n";
    }

    // Show Due Rent
    void showDue() {
        cout << "\n=== TENANTS WITH DUE RENT ===\n";
        for (int i = 0; i < houses.size(); i++) {
            for (int j = 0; j < houses[i].rooms.size(); j++) {
                if (houses[i].rooms[j].isOccupied &&
                    houses[i].rooms[j].client.balance() > 0) {

                    cout << houses[i].rooms[j].client.name
                         << " owes: "
                         << houses[i].rooms[j].client.balance()
                         << endl;
                }
            }
        }
    }

    // Display All
    void display() {
        for (int i = 0; i < houses.size(); i++) {
            cout << "\nHouse: " << houses[i].name << endl;

            for (int j = 0; j < houses[i].rooms.size(); j++) {
                cout << " Room " << houses[i].rooms[j].roomNumber;

                if (houses[i].rooms[j].isOccupied) {
                    cout << " - " << houses[i].rooms[j].client.name;
                    cout << " | Paid: " << houses[i].rooms[j].client.rentPaid;
                    cout << " | Due: " << houses[i].rooms[j].client.balance();
                } else {
                    cout << " - Vacant";
                }
                cout << endl;
            }
        }
    }

    // Save Data
    void save() {
        ofstream file("data.txt");

        for (int i = 0; i < houses.size(); i++) {
            file << houses[i].name << endl;

            for (int j = 0; j < houses[i].rooms.size(); j++) {
                file << houses[i].rooms[j].roomNumber << " "
                     << houses[i].rooms[j].isOccupied << " ";

                if (houses[i].rooms[j].isOccupied) {
                    file << houses[i].rooms[j].client.name << " "
                         << houses[i].rooms[j].client.rentDue << " "
                         << houses[i].rooms[j].client.rentPaid;
                }

                file << endl;
            }
        }

        file.close();
        cout << "Data saved!\n";
    }
};

// ================= LOGIN =================
bool login() {
    string user, pass;

    cout << "Enter username: ";
    cin >> user;

    cout << "Enter password: ";
    cin >> pass;

    if (user == "admin" && pass == "1234") {
        return true;
    }

    cout << "Invalid login!\n";
    return false;
}

// ================= MAIN MENU =================
int main() {
    if (!login()) return 0;

    SystemManager sys;
    int choice;

    do {
        cout << "\n===== ADVANCED RENT SYSTEM =====\n";
        cout << "1. Add House\n";
        cout << "2. Add Room\n";
        cout << "3. Add Tenant\n";
        cout << "4. Make Payment\n";
        cout << "5. Search Tenant\n";
        cout << "6. Show Due Rent\n";
        cout << "7. Display All\n";
        cout << "8. Save Data\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "House name: ";
            cin >> name;
            sys.addHouse(name);
        }

        else if (choice == 2) {
            int h, r;
            cout << "House index: ";
            cin >> h;
            cout << "Room number: ";
            cin >> r;
            sys.addRoom(h, r);
        }

        else if (choice == 3) {
            int h, r;
            string name;
            double rent;

            cout << "House index: ";
            cin >> h;
            cout << "Room index: ";
            cin >> r;
            cout << "Tenant name: ";
            cin >> name;
            cout << "Rent amount: ";
            cin >> rent;

            sys.addTenant(h, r, name, rent);
        }

        else if (choice == 4) {
            int h, r;
            double amt;

            cout << "House index: ";
            cin >> h;
            cout << "Room index: ";
            cin >> r;
            cout << "Payment: ";
            cin >> amt;

            sys.makePayment(h, r, amt);
        }

        else if (choice == 5) {
            string name;
            cout << "Enter tenant name: ";
            cin >> name;
            sys.searchTenant(name);
        }

        else if (choice == 6) {
            sys.showDue();
        }

        else if (choice == 7) {
            sys.display();
        }

        else if (choice == 8) {
            sys.save();
        }

    } while (choice != 0);

    return 0;
}