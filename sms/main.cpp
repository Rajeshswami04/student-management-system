#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;




class Semester {
private:
    int semenum;
    map<string, pair<double, int>> subjectgrades; // subject -> {marks, credits}
public:
    Semester(int num) : semenum(num) {}
    void addsubject(const string& subject, double marks, int credits); // adding subject , grades , marks
    bool clearbacklog(const string& subject, double newMarks, int newCredits); // after supplymentry exam new detail
    double calculateSGPA() const ; // calculating cg
    vector<string> getbacklog() const ; // all backlogged subjects
    void displaydetail() const;  // allinfo about student
    int getsemnumber() const { return semenum; } // return semester
};


void Semester::addsubject(const string& subject, double marks, int credits = 4){ //default credits are 4
    
        if (marks < 0 || marks > 100) {
            cout << "Invalid marks!\n";
            return;
        }
        if (subjectgrades.find(subject) != subjectgrades.end()) {
            cout << "Subject already exists in semester " << semenum << ".\n";
            return;
        }
        subjectgrades[subject] = {marks, credits};
    
}


bool Semester::clearbacklog(const string& subject, double newMarks, int newCredits = 4) {
        if (newMarks < 0 || newMarks > 100 || newCredits <= 0) {
            cout << "Invalid marks or credits!\n";
            return false;
        }
        auto it = subjectgrades.find(subject);
        if (it == subjectgrades.end()) {
            cout << "Subject " << subject << " not found in semester " << semenum << ".\n";
            return false;
        }
        if (it->second.first >= 40) {
            cout << "Subject " << subject << " is not a backlog (marks >= 40).\n";
            return false;
        }
        it->second = {newMarks, newCredits};
        cout << "Backlog cleared for " << subject << " in semester " << semenum << ".\n";
        return true;
    }
double Semester:: calculateSGPA() const {
        double totelnumber = 0.0;
        int totelcredit = 0;
        for ( auto entry : subjectgrades) {
            double marks = entry.second.first;
            int credits = entry.second.second;
            double gp = (marks >= 40) ? floor(marks / 10) : 0.0;
            totelnumber += gp * credits;
            totelcredit += credits;
        }
        return totelcredit > 0 ? totelnumber / totelcredit : 0.0;
    }

vector<string> Semester:: getbacklog() const {
        vector<string> backlogs;
        for ( auto entry : subjectgrades) {
            if (entry.second.first < 40) backlogs.push_back(entry.first);
        }
        return backlogs;
    }


void Semester:: displaydetail() const {
        cout << "Semester " << semenum << ":\n";
        for ( auto entry : subjectgrades) {
            cout << "  " << entry.first << ": Marks=" << entry.second.first 
                      << ", Credits=" << entry.second.second << "\n";
        }
        cout << "  SGPA: " << fixed << setprecision(2) << calculateSGPA() << "\n";
        // fixed fixes totel number after decimal
        auto bl = getbacklog();
        if (!bl.empty()) {
            cout << "  Backlogs: ";
            for ( auto b : bl) cout << b << " ";
            cout << "\n";
        }
    }

// Semester class ends













class Student {
private:
    int roll_no;
    string name;
    int age;
    vector<Semester> semesters;
public:
    Student(int roll_no, string name, int age) : roll_no(roll_no), name(name), age(age) {}
    bool addSemester();
    void addsubjectTosemester(int sem, const string& subject, double marks, int credits); 
    bool clearbackloginsem(int sem, const string& subject, double newMarks, int newCredits);
    double calculateCGPA() const ;
    void displayacademicdetails() const ;
    int getroll() const { return roll_no; }
};

// emplace_back helps us to creating object and pushing into at instant time
// push only can push not create and push

bool Student:: addSemester() {
        if (semesters.size() >= 8) {
            cout << "Max 8 semesters reached!\n";
            return false;
        }
        int nextSem = semesters.empty() ? 1 : semesters.back().getsemnumber() + 1;
        //back() gives last element
        semesters.emplace_back(nextSem); // create semester obj and pushed into vector
        return true;
    }


// [sem](const Semester& s) { return s.getsemnumber() == sem; }
// this is lambda function , takes sem as arg and compare and return boolean and helps to find find_if func to finding iterator


void Student:: addsubjectTosemester(int sem, const string& subject, double marks, int credits = 4) {
        auto it = find_if(semesters.begin(), semesters.end(), 
                              [sem](const Semester& s) { return s.getsemnumber() == sem; });
        if (it != semesters.end()) {
            it->addsubject(subject, marks, credits);
        } else {
            cout << "Semester " << sem << " not found!\n";
        }
    }

 bool Student:: clearbackloginsem(int sem, const string& subject, double newMarks, int newCredits = 4) {
        auto it = find_if(semesters.begin(), semesters.end(), 
                              [sem](const Semester& s) { return s.getsemnumber() == sem; });
        if (it != semesters.end()) {
            return it->clearbacklog(subject, newMarks, newCredits);
        } else {
            cout << "Semester " << sem << " not found!\n";
            return false;
        }
    }
// calculating whole cgpa till current sem not for 2 as year

double Student:: calculateCGPA() const {
        if (semesters.empty()) return 0.0;
        double totalSGPA = 0.0;
        for (auto sem : semesters) {
            totalSGPA += sem.calculateSGPA();
        }
        return totalSGPA / semesters.size();
    }



void Student:: displayacademicdetails() const {
        cout << "Student ID: " << roll_no << ", Name: " << name << ", Age: " << age << "\n";
        for (auto sem : semesters) {
            sem.displaydetail();
        }
        cout << "Overall CGPA: " << fixed << setprecision(2) << calculateCGPA() << "\n";
        int totalBl = 0;
        for ( auto sem : semesters) totalBl += sem.getbacklog().size();
        cout << "Total Backlogs: " << totalBl << "\n";
    }


// class Undergraduate : public Student {
// private:
//     string degreeProgram; 
// public:
//     Undergraduate(int roll_no, string name, int age, string degreeProgram)
//         : Student(roll_no, name, age), degreeProgram(degreeProgram) {}
//     void displayacademicdetailsug() const  {
//         cout << "Undergraduate Student Details:\n";
//         displayacademicdetails();
//     }
//     string getDegreeType() const  { return "Undergraduate"; }
// };

// class Postgraduate : public Student {
// private:
//     string thesisTopic; 
// public:
//     Postgraduate(int roll_no, string name, int age, string thesisTopic)
//         : Student(roll_no, name, age), thesisTopic(thesisTopic) {}
//     void displayacademicdetailspg() const {
//         cout << "Postgraduate Student Details:\n";
//         displayacademicdetails();
//     }
//     string getDegreeType() const  { return "Postgraduate"; }
// };












class Managementsystem {
private:
    vector<Student> students;
public:
    void addstudentug(int roll_no, string name, int age,string program){
        for (auto s : students) {
            if (s.getroll() == roll_no) {
                cout << "student already exists!\n";
                return;
            }
        }
        students.emplace_back(roll_no, name, age);
    }
    Student* findstudent(int roll_no) { // helping finding to student for clear backlogs
        auto it = find_if(students.begin(), students.end(), 
                              [roll_no](const Student& s) { return s.getroll() == roll_no; }); // lambda
        return (it != students.end()) ? &(*it) : nullptr;
    }
    void clearbacklog(int studentID, int sem, const string& subject, double newMarks, int newCredits = 4) {
        if (auto student = findstudent(studentID)) { 
            student->clearbackloginsem(sem, subject, newMarks, newCredits);
        } else {
            cout << "Student not found!\n";
        }
    }
    void displayallstudent() const {
        if (students.empty()) cout << "No students.\n";
        for (const auto& s : students) {
            s.displayacademicdetails();
            cout << "------------------------\n";
        }
    }
};

int main() {
    Managementsystem sms;
    cout<<"--------------------------------------------------------*-FOR UNDERGRADUATES ONLY-*-------------------------------------------------------\n";
    int choice;
    do {
        cout << "\n1. Add Student\n2. Add Semester for Student\n3. Add Subject Grade\n"
                  << "4. View Academic Summary\n5. Clear Backlog\n6. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            int roll_no, age;
            string name;
            cout << "Enter ID, Name, Age: ";
            cin >> roll_no>>ws; 
            getline(cin, name);
            cin >> age;
            sms.addstudent(roll_no, name, age);
        } else if (choice == 2) {
            int roll_no;
            cout << "Enter Student ID: ";
            cin >> roll_no;
            if (auto student = sms.findstudent(roll_no)) {
                student->addSemester();
            } else {
                cout << "Student not found!\n";
            }
        } else if (choice == 3) {
            int roll_no, sem, credits;
            double marks;
            string subject;
            cout << "Enter Student ID, Semester, Subject, Marks, Credits: ";
            cin >> roll_no >> sem >> ws;
            getline(cin, subject);
            cin >> marks >> credits;
            if (auto student = sms.findstudent(roll_no)) {
                student->addsubjectTosemester(sem, subject, marks, credits);
            } else {
                cout << "Student not found!\n";
            }
        } else if (choice == 4) {
            int roll_no;
            cout << "Enter Student ID (or 0 for all): ";
            cin >> roll_no;
            if (roll_no == 0) {
                sms.displayallstudent();
            } else {
                if (auto student = sms.findstudent(roll_no)) {
                    student->displayacademicdetails();
                } else {
                    cout << "Student not found!\n";
                }
            }
        } else if (choice == 5) {
            int roll_no, sem, credits;
            double newMarks;
            string subject;
            cout << "Enter Student ID, Semester, Subject, New Marks, New Credits: ";
            cin >> roll_no >> sem >> ws;
            getline(cin, subject);
            cin >> newMarks >> credits;
            sms.clearbacklog(roll_no, sem, subject, newMarks, credits);
        }
    } while (choice != 6);
    return 0;
}
