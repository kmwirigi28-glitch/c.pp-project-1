#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// ================= CUSTOMER =================
class Customer {
public:
    string name;
    double amountPaid;
    double amountDue;

    Customer() {
        name = "";
        amountPaid = 0;
        amountDue = 0;
    }

    Customer(string n, double rent) {
        name = n;
        amountPaid = 0;
        amountDue = rent;
    }

    double balance() {
        return amountDue - amountPaid;
    }

    void pay(double amount) {
        amountPaid += amount;
    }
};

// ================= CAR =================
class Car {
public:
    int id;
    string model;
    bool isRented;
    Customer customer;

    Car() {}

    Car(int i, string m) {
        id = i;
        model = m;
        isRented = false;
    }

    void hire(Customer c) {
        customer = c;
        isRented = true;
    }

    void returnCar() {
        isRented = false;
        customer = Customer();
    }
};

// ================= SYSTEM =================
class CarSystem {
private:
    vector<Car> cars;

public:
    void addCar(int id, string model) {
        cars.push_back(Car(id, model));
    }

    void hireCar(int index, string name, double rent) {
        Customer c(name, rent);
        cars[index].hire(c);
    }

    void returnCar(int index) {
        cars[index].returnCar();
    }

    void pay(int index, double amount) {
        cars[index].customer.pay(amount);
    }

    void showCars() {
        for (int i = 0; i < cars.size(); i++) {
            cout << "\nCar ID: " << cars[i].id;
            cout << " Model: " << cars[i].model;

            if (cars[i].isRented) {
                cout << " | Rented by: " << cars[i].customer.name;
                cout << " | Paid: " << cars[i].customer.amountPaid;
                cout << " | Balance: " << cars[i].customer.balance();
            } else {
                cout << " | Available";
            }
        }
    }

    void showAvailable() {
        cout << "\nAvailable Cars:\n";
        for (int i = 0; i < cars.size(); i++) {
            if (!cars[i].isRented) {
                cout << "Car ID: " << cars[i].id
                     << " | Model: " << cars[i].model << endl;
            }
        }
    }

    void searchCustomer(string name) {
        for (int i = 0; i < cars.size(); i++) {
            if (cars[i].isRented && cars[i].customer.name == name) {
                cout << "\nFound:\n";
                cout << "Car: " << cars[i].model << endl;
                cout << "Paid: " << cars[i].customer.amountPaid << endl;
                cout << "Balance: " << cars[i].customer.balance() << endl;
                return;
            }
        }
        cout << "Customer not found!\n";
    }

    void save() {
        ofstream file("car_data.txt");

        for (int i = 0; i < cars.size(); i++) {
            file << cars[i].id << " "
                 << cars[i].model << " "
                 << cars[i].isRented << " ";

            if (cars[i].isRented) {
                file << cars[i].customer.name << " "
                     << cars[i].customer.amountDue << " "
                     << cars[i].customer.amountPaid;
            }

            file << endl;
        }

        file.close();
        cout << "Data saved!\n";
    }
};

// ================= LOGIN =================
bool login() {
    string user, pass;

    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    return (user == "admin" && pass == "1234");
}

// ================= MAIN =================
int main() {
    if (!login()) {
        cout << "Access denied!\n";
        return 0;
    }

    CarSystem system;
    int choice;

    do {
        cout << "\n===== CAR HIRE SYSTEM =====\n";
        cout << "1. Add Car\n";
        cout << "2. Hire Car\n";
        cout << "3. Return Car\n";
        cout << "4. Make Payment\n";
        cout << "5. Show All Cars\n";
        cout << "6. Show Available Cars\n";
        cout << "7. Search Customer\n";
        cout << "8. Save Data\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string model;
            cout << "Car ID: ";
            cin >> id;
            cout << "Model: ";
            cin >> model;

            system.addCar(id, model);
        }

        else if (choice == 2) {
            int index;
            string name;
            double rent;

            cout << "Car index: ";
            cin >> index;
            cout << "Customer name: ";
            cin >> name;
            cout << "Rent: ";
            cin >> rent;

            system.hireCar(index, name, rent);
        }

        else if (choice == 3) {
            int index;
            cout << "Car index: ";
            cin >> index;

            system.returnCar(index);
        }

        else if (choice == 4) {
            int index;
            double amount;

            cout << "Car index: ";
            cin >> index;
            cout << "Amount: ";
            cin >> amount;

            system.pay(index, amount);
        }

        else if (choice == 5) {
            system.showCars();
        }

        else if (choice == 6) {
            system.showAvailable();
        }

        else if (choice == 7) {
            string name;
            cout << "Customer name: ";
            cin >> name;
            system.searchCustomer(name);
        }

        else if (choice == 8) {
            system.save();
        }

    } while (choice != 0);

    return 0;
}