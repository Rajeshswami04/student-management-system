#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

class Person
{
private:
    string name;
    int age;

public:
    Person(string s, int a) : name(s), age(a) {};  //common attributes for Professor and Student 
    void show() const
    {
        cout << " Name: " << name << ", Age: " << age << "\n";
    }
};

class Semester    
{
private:
    int semenum;
    map<string, pair<double, int>> subjectgrades; // subject -> {marks, credits}
public:
    Semester(int num) : semenum(num) {}
    void addsubject(const string &subject, double marks, int credits);         // adding subject , grades , marks
    bool clearbacklog(const string &subject, double newMarks, int newCredits); // after supplymentry exam new detail
    double calculateSGPA() const;                                              // calculating cg
    vector<string> getbacklog() const;                                         // all backlogged subjects
    void displaydetail() const;                                                // allinfo about student
    int getsemnumber() const { return semenum; }                               // return semester
};

void Semester::addsubject(const string &subject, double marks, int credits = 4)
{ // default credits are 4

    if (marks < 0 || marks > 100)
    {
        cout << "Invalid marks!\n";
        return;
    }
    if (subjectgrades.find(subject) != subjectgrades.end())
    {
        cout << "Subject already exists in semester " << semenum << ".\n";
        return;
    }
    subjectgrades[subject] = {marks, credits};
}

bool Semester::clearbacklog(const string &subject, double newMarks, int newCredits = 4)
{
    if (newMarks < 0 || newMarks > 100 || newCredits <= 0)
    {
        cout << "Invalid marks or credits!\n";
        return false;
    }
    auto it = subjectgrades.find(subject);
    if (it == subjectgrades.end())
    {
        cout << "Subject " << subject << " not found in semester " << semenum << ".\n";
        return false;
    }
    if (it->second.first >= 40)
    {
        cout << "Subject " << subject << " is not a backlog (marks >= 40).\n";
        return false;
    }
    it->second = {newMarks, newCredits};
    cout << "Backlog cleared for " << subject << " in semester " << semenum << ".\n";
    return true;
}
double Semester::calculateSGPA() const
{
    double totelnumber = 0.0;
    int totelcredit = 0;
    for (auto entry : subjectgrades)
    {
        double marks = entry.second.first;
        int credits = entry.second.second;
        double gp = (marks >= 40) ? floor(marks / 10) : 0.0;
        totelnumber += gp * credits;
        totelcredit += credits;
    }
    return totelcredit > 0 ? totelnumber / totelcredit : 0.0;
}

vector<string> Semester::getbacklog() const
{
    vector<string> backlogs;
    for (auto entry : subjectgrades)
    {
        if (entry.second.first < 40)
            backlogs.push_back(entry.first);
    }
    return backlogs;
}

void Semester::displaydetail() const
{
    cout << "Semester " << semenum << ":\n";
    for (auto entry : subjectgrades)
    {
        cout << "  " << entry.first << ": Marks=" << entry.second.first
             << ", Credits=" << entry.second.second << "\n";
    }
    cout << "  SGPA: " << fixed << setprecision(2) << calculateSGPA() << "\n";
    // fixed fixes totel number after decimal
    auto bl = getbacklog();
    if (!bl.empty())
    {
        cout << "  Backlogs: ";
        for (auto b : bl)
            cout << b << " ";
        cout << "\n";
    }
}

// Semester class ends

class Student : public Person
{
private:
    int roll_no;
    vector<Semester> semesters;

public:
    Student(int roll_no, string name, int age) : Person(name, age), roll_no(roll_no) {}
    bool addSemester();
    void addsubjectTosemester(int sem, const string &subject, double marks, int credits);
    bool clearbackloginsem(int sem, const string &subject, double newMarks, int newCredits);
    double calculateCGPA() const;
    void displayacademicdetails() const;
    int getroll() const { return roll_no; }
};

// emplace_back helps us to creating object and pushing into at instant time
// push only can push not create and push

bool Student::addSemester()
{
    if (semesters.size() >= 8)
    {
        cout << "Max 8 semesters reached!\n";
        return false;
    }
    int nextSem = semesters.empty() ? 1 : semesters.back().getsemnumber() + 1;
    // back() gives last element
    semesters.emplace_back(nextSem); // create semester obj and pushed into vector
    return true;
}

// [sem](const Semester& s) { return s.getsemnumber() == sem; }
// this is lambda function , takes sem as arg and compare and return boolean and helps to find find_if func to finding iterator

void Student::addsubjectTosemester(int sem, const string &subject, double marks, int credits = 4)
{
    auto it = find_if(semesters.begin(), semesters.end(),
                      [sem](const Semester &s)
                      { return s.getsemnumber() == sem; });
    if (it != semesters.end())
    {
        it->addsubject(subject, marks, credits);
    }
    else
    {
        cout << "Semester " << sem << " not found!\n";
    }
}

bool Student::clearbackloginsem(int sem, const string &subject, double newMarks, int newCredits = 4)
{
    auto it = find_if(semesters.begin(), semesters.end(),
                      [sem](const Semester &s)
                      { return s.getsemnumber() == sem; });
    if (it != semesters.end())
    {
        return it->clearbacklog(subject, newMarks, newCredits);
    }
    else
    {
        cout << "Semester " << sem << " not found!\n";
        return false;
    }
}
// calculating whole cgpa till current sem not for 2 as year

double Student::calculateCGPA() const
{
    if (semesters.empty())
        return 0.0;
    double totalSGPA = 0.0;
    for (auto sem : semesters)
    {
        totalSGPA += sem.calculateSGPA();
    }
    return totalSGPA / semesters.size();
}

void Student::displayacademicdetails() const
{
    cout << "Roll_no:" << roll_no << " ";
    show();
    for (auto sem : semesters)
    {
        sem.displaydetail();
    }
    cout << "Overall CGPA: " << fixed << setprecision(2) << calculateCGPA() << "\n";
    int totalBl = 0;
    for (auto sem : semesters)
        totalBl += sem.getbacklog().size();
    cout << "Total Backlogs: " << totalBl << "\n";
}

class Faculty : public Person
{
private:
    string id;
    string qualification;
    int grade;
    int exp_year;
    string research_area;
    string department;

public:
    Faculty(string name, int age, string q, int g, int e, string r, string d)
        : Person(name, age), qualification(q), grade(g), exp_year(e), research_area(r), department(d)
    {
        id = name.substr(0, 3) + to_string(age) + department.substr(0, 3);
    }
    void showdetails() const
    {
        show();
        cout << "id:" << id << endl;
        cout << "qualifications:" << qualification << endl;
        cout << "grade:" << grade << endl;
        cout << "experience:" << exp_year << "yrs" << endl;
        cout << "research area :" << research_area << endl;
        cout << "department :" << department << endl;
    }
    void gradeupgrade(int g)
    {
        grade = g;
    }
    void addquali(string q)
    {
        qualification += " " + q;
    }
    void research(string r)
    {
        research_area = r;
    }
    string getid() const
    {
        return id;
    }
};

class Managementsystem
{
private:
    vector<Student> students;
    vector<Faculty> faculties;

public:
    // student stuff
    void addstudent(int roll_no, string name, int age)
    {
        for (auto s : students)
        {
            if (s.getroll() == roll_no)
            {
                cout << "student already exists!\n";
                return;
            }
        }
        students.emplace_back(roll_no, name, age);
    }
    Student *findstudent(int roll_no)
    { // helping finding to student for clear backlogs
        auto it = find_if(students.begin(), students.end(),
                          [roll_no](const Student &s)
                          { return s.getroll() == roll_no; }); // lambda
        return (it != students.end()) ? &(*it) : nullptr;
    }
    void clearbacklog(int studentID, int sem, const string &subject, double newMarks, int newCredits = 4)
    {
        if (auto student = findstudent(studentID))
        {
            student->clearbackloginsem(sem, subject, newMarks, newCredits);
        }
        else
        {
            cout << "Student not found!\n";
        }
    }
    void displayallstudent() const
    {
        if (students.empty())
            cout << "No students.\n";
        for (const auto &s : students)
        {
            s.displayacademicdetails();
            cout << "------------------------\n";
        }
    }
    // Faculty stuff

    void addfaculty(string name, int age, string q, int g, int e, string r, string d)
    {
        faculties.emplace_back(name, age, q, g, e, r, d);
    }
    void promotion(string id, int grade)
    {
        auto it = find_if(faculties.begin(), faculties.end(),
                          [id](const Faculty &s)
                          { return s.getid() == id; });
        if (it != faculties.end())
        {
            it->gradeupgrade(grade);
        }
        else
        {
            cout << "Faculty with ID " << id << " not found!\n";
        }
    }
    void addqualfications(string q, string id)
    {
        auto it = find_if(faculties.begin(), faculties.end(),
                          [id](const Faculty &s)
                          { return s.getid() == id; });
        if (it != faculties.end())
        {
            it->addquali(q);
        }
        else
        {
            cout << "Faculty with ID " << id << " not found!\n";
        }
    }
    void changeresearcharea(string id, string r)
    {
        auto it = find_if(faculties.begin(), faculties.end(),
                          [id](const Faculty &s)
                          { return s.getid() == id; });
        if (it != faculties.end())
        {
            it->research(r);
        }
        else
        {
            cout << "Faculty with ID " << id << " not found!\n";
        }
    }
    Faculty *findprof(string id)
    {
        auto it = find_if(faculties.begin(), faculties.end(),
                          [id](const Faculty &s)
                          { return s.getid() == id; });
        return (it != faculties.end()) ? &(*it) : nullptr;
    }

    void displayallprof() const
    {
        if (faculties.empty())
            cout << "No faculties.\n";
        for (const auto &s : faculties)
        {
            s.showdetails();
            cout << "------------------------\n";
        }
    }
};

int main()
{
    Managementsystem sms;
    int choice;
    do
    {
        cout << "\n1. Add Student\n2. Add Semester for Student\n3. Add Subject Grade\n"
             << "4. View Academic Summary\n5. Clear Backlog\n6.Add Faculty\n7.promote Faculty\n"
             << "8.add more qualifications\n9.change research area of Prof.\n10. Show detail of prof.\n11. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1)
        {
            int roll_no, age;
            string name;
            cout << "Enter ID, Name, Age: ";
            cin >> roll_no >> ws;
            getline(cin, name);
            cin >> age;
            sms.addstudent(roll_no, name, age);
        }
        else if (choice == 2)
        {
            int roll_no;
            cout << "Enter Student ID: ";
            cin >> roll_no;
            if (auto student = sms.findstudent(roll_no))
            {
                student->addSemester();
            }
            else
            {
                cout << "Student not found!\n";
            }
        }
        else if (choice == 3)
        {
            int roll_no, sem, credits;
            double marks;
            string subject;
            cout << "Enter Student ID, Semester, Subject, Marks, Credits: ";
            cin >> roll_no >> sem >> ws;
            getline(cin, subject);
            cin >> marks >> credits;
            if (auto student = sms.findstudent(roll_no))
            {
                student->addsubjectTosemester(sem, subject, marks, credits);
            }
            else
            {
                cout << "Student not found!\n";
            }
        }
        else if (choice == 4)
        {
            int roll_no;
            cout << "Enter Student ID (or 0 for all): ";
            cin >> roll_no;
            if (roll_no == 0)
            {
                sms.displayallstudent();
            }
            else
            {
                if (auto student = sms.findstudent(roll_no))
                {
                    student->displayacademicdetails();
                }
                else
                {
                    cout << "Student not found!\n";
                }
            }
        }
        else if (choice == 5)
        {
            int roll_no, sem, credits;
            double newMarks;
            string subject;
            cout << "Enter Student ID, Semester, Subject, New Marks, New Credits: ";
            cin >> roll_no >> sem;
            cin.ignore();
            getline(cin, subject);
            cin >> newMarks >> credits;
            sms.clearbacklog(roll_no, sem, subject, newMarks, credits);
        }
        else if (choice == 6)
        {
            string name;
            int age;
            string q;
            int g;
            int e;
            string r;
            string d;
            cout << "Enter: name, age, qualification, grade, experience, research_area, department\n";
            cin.ignore();
            getline(cin, name);
            cin >> age;
            cin.ignore();
            getline(cin, q);
            cin >> g >> e;
            cin.ignore();
            getline(cin, r);
            getline(cin, d);
            sms.addfaculty(name, age, q, g, e, r, d);
        }
        else if (choice == 7)
        {
            string id;
            int grade;
            cout << "enter id of professor:";
            cin >> id;
            cout << "\nenter grade:";
            cin >> grade;
            sms.promotion(id, grade);
        }
        else if (choice == 8)
        {
            string id;
            string q;
            cout << "enter id of professor:";
            cin >> id;
            cout << "\nenter new qualification:";
            cin >> q;
            sms.addqualfications(id, q);
        }
        else if (choice == 9)
        {
            string id;
            string reas;
            cout << "enter id:";
            cin >> id;
            cout << "\nenter research area";
            cin.ignore();
            getline(cin, reas);
            sms.changeresearcharea(id, reas);
        }
        else if (choice == 10)
        {
            string id;
            cout << "enter id or (all for all) of prof.";
            cin >> id;
            if (id == "all")
            {
                sms.displayallprof();
            }
            else
            {
                if (auto facutly = sms.findprof(id))
                {
                    facutly->showdetails();
                }
                else
                {
                    cout << "prof not found!\n";
                }
            }
        }
    } while (choice != 11);
    return 0;
}
