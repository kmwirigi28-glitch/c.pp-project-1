#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// -------- Student struct ----------
struct Student {
    int id;
    string name;
    int age;
    string course;
    double gpa;

    string serialize() const {
        return to_string(id) + "|" + name + "|" + to_string(age) + "|" + course + "|" + to_string(gpa);
    }

    static Student deserialize(const string &line) {
        stringstream ss(line);
        string token;
        Student s;

        auto getToken = [&](string &out) -> bool {
            if (!getline(ss, out, '|')) return false;
            return true;
        };

        if (!getToken(token)) throw runtime_error("Invalid record (id)");
        s.id = stoi(token);

        if (!getToken(s.name)) throw runtime_error("Invalid record (name)");
        if (!getToken(token)) throw runtime_error("Invalid record (age)");
        s.age = stoi(token);
        if (!getToken(s.course)) throw runtime_error("Invalid record (course)");
        if (!getToken(token)) throw runtime_error("Invalid record (gpa)");
        s.gpa = stod(token);

        return s;
    }
};

// -------- Input helpers ----------
string readLine(const string &prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

int readInt(const string &prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            if (s.empty()) { cout << "Please enter a number.\n"; continue; }
            int val = stoi(s);
            return val;
        } catch (...) {
            cout << "Invalid integer. Try again.\n";
        }
    }
}

double readDouble(const string &prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            if (s.empty()) { cout << "Please enter a number.\n"; continue; }
            double val = stod(s);
            return val;
        } catch (...) {
            cout << "Invalid number. Try again.\n";
        }
    }
}

// -------- File I/O ----------
vector<Student> loadFromFile(const string &filename) {
    vector<Student> students;
    ifstream in(filename);
    if (!in.is_open()) return students;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        try {
            students.push_back(Student::deserialize(line));
        } catch (const exception &e) {
            cerr << "Warning: skipping invalid record: " << e.what() << "\n";
        }
    }
    return students;
}

void saveToFile(const string &filename, const vector<Student> &students) {
    ofstream out(filename, ios::trunc);
    if (!out.is_open()) {
        cerr << "Error: could not open file for writing: " << filename << "\n";
        return;
    }
    for (const auto &s : students) out << s.serialize() << "\n";
}

// -------- Utilities ----------
int findIndexByID(const vector<Student> &students, int id) {
    for (size_t i = 0; i < students.size(); ++i) if (students[i].id == id) return (int)i;
    return -1;
}

void displayStudent(const Student &s) {
    cout << "ID: " << s.id
         << " | Name: " << s.name
         << " | Age: " << s.age
         << " | Course: " << s.course
         << " | GPA: " << s.gpa << "\n";
}

// -------- CRUD operations ----------
void addStudent(vector<Student> &students, const string &filename) {
    cout << "\n== Add Student ==\n";
    int id = readInt("Enter ID (integer): ");
    if (findIndexByID(students, id) != -1) {
        cout << "A student with this ID already exists. Aborting add.\n";
        return;
    }
    string name = readLine("Enter full name: ");
    int age = readInt("Enter age: ");
    string course = readLine("Enter course: ");
    double gpa = readDouble("Enter GPA: ");

    Student s { id, name, age, course, gpa };
    students.push_back(s);
    saveToFile(filename, students);
    cout << "Student added.\n";
}

void listStudents(const vector<Student> &students) {
    cout << "\n== All Students ==\n";
    if (students.empty()) { cout << "No students found.\n"; return; }
    for (const auto &s : students) displayStudent(s);
}

void searchStudent(const vector<Student> &students) {
    int id = readInt("Enter ID to search: ");
    int idx = findIndexByID(students, id);
    if (idx == -1) cout << "Student not found.\n";
    else displayStudent(students[idx]);
}

void updateStudent(vector<Student> &students, const string &filename) {
    int id = readInt("Enter ID to update: ");
    int idx = findIndexByID(students, id);
    if (idx == -1) { cout << "Student not found.\n"; return; }

    Student &s = students[idx];
    cout << "Leave a field blank and press Enter to keep existing value.\n";

    string input = readLine("New name (" + s.name + "): ");
    if (!input.empty()) s.name = input;

    input = readLine("New age (" + to_string(s.age) + "): ");
    if (!input.empty()) s.age = stoi(input);

    input = readLine("New course (" + s.course + "): ");
    if (!input.empty()) s.course = input;

    input = readLine("New GPA (" + to_string(s.gpa) + "): ");
    if (!input.empty()) s.gpa = stod(input);

    saveToFile(filename, students);
    cout << "Student updated.\n";
}

void deleteStudent(vector<Student> &students, const string &filename) {
    int id = readInt("Enter ID to delete: ");
    int idx = findIndexByID(students, id);
    if (idx == -1) { cout << "Student not found.\n"; return; }

    string conf = readLine("Type 'yes' to confirm deletion: ");
    if (conf == "yes") {
        students.erase(students.begin() + idx);
        saveToFile(filename, students);
        cout << "Student deleted.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// -------- Main menu ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string filename = "students.txt";
    vector<Student> students = loadFromFile(filename);

    while (true) {
        cout << "\n=== Student Management Menu ===\n";
        cout << "1. Add student\n";
        cout << "2. List all students\n";
        cout << "3. Search student by ID\n";
        cout << "4. Update student\n";
        cout << "5. Delete student\n";
        cout << "6. Exit\n";

        int choice = readInt("Enter your choice: ");
        switch (choice) {
            case 1: addStudent(students, filename); break;
            case 2: listStudents(students); break;
            case 3: searchStudent(students); break;
            case 4: updateStudent(students, filename); break;
            case 5: deleteStudent(students, filename); break;
            case 6: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}