#include <iostream>
#include <unordered_map>
#include <string>
#include <stack>

using namespace std;

class courseName {
private:
    string subName;
    int creditsHr;
    char grade;

public:
    courseName(string sN, int cHr, char g) : subName(sN), creditsHr(cHr), grade(g) {}

    int getCreditsHr() const {
        return creditsHr;
    }

    char getGrade() const {
        return grade;
    }
};

class Node {
public:
    courseName coursename;
    Node* next;

    Node(const courseName& c) : coursename(c), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    void addcourseName(const courseName& coursename) {
        Node* newNode = new Node(coursename);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    Node* getHead() const {
        return head;
    }

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

class GPACalculator {
private:
    LinkedList subjects;

    double gradeToPoints(char grade) const {
        unordered_map<char, double> gradePoints = {
            {'A', 4.0},
            {'B', 3.0},
            {'C', 2.0},
            {'D', 1.0},
            {'F', 0.0}
        };
        auto it = gradePoints.find(toupper(grade));
        if (it != gradePoints.end()) {
            return it->second;
        }
        return 0.0;
    }

public:
    void addcourseName(const courseName& coursename) {
        subjects.addcourseName(coursename);
    }

    double calculateGPA() const {
        double totalCreditsHr = 0;
        double weightedSum = 0;
        Node* temp = subjects.getHead();

        while (temp) {
            totalCreditsHr += temp->coursename.getCreditsHr();
            weightedSum += temp->coursename.getCreditsHr() * gradeToPoints(temp->coursename.getGrade());
            temp = temp->next;
        }
        if (totalCreditsHr == 0) return 0;
        return weightedSum / totalCreditsHr;
    }

    LinkedList getSubjects() const {
        return subjects;
    }

    void clearSubjects() {
        subjects.~LinkedList();  // Reset the linked list
        new(&subjects) LinkedList();  // Reinitialize the linked list
    }

    double getGradePoints(char grade) const {
        return gradeToPoints(grade);
    }
};

bool isValidGrade(char grade) {
    return grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F';
}

bool isValidInteger(int& num) {
    cin >> num;
    if (cin.fail()) {
        cin.clear(); // Clear the error flag
        cin.ignore(10000, '\n'); // Discard invalid input up to 10000 characters or newline
        return false;
    }
    cin.ignore(10000, '\n'); // Clear the rest of the line
    return true;
}

bool isValidChar(char& ch) {
    cin >> ch;
    if (cin.fail() || !isalpha(ch)) {
        cin.clear(); // Clear the error flag
        cin.ignore(10000, '\n'); // Discard invalid input up to 10000 characters or newline
        return false;
    }
    cin.ignore(10000, '\n'); // Clear the rest of the line
    return true;
}

int main() {
    stack<double> semesterGpaStack;  // Stack to store GPA of each semester
    char continueChoice;

    do {
        int numSemesters;
        cout << "Welcome to the GPA Calculator.\n";
        cout << "Enter the number of semesters: ";
        while (!isValidInteger(numSemesters) || numSemesters <= 0) {
            cout << "Please enter a valid number for the number of semesters: ";
        }

        for (int s = 1; s <= numSemesters; ++s) {
            cout << "\nSemester " << s << ":\n";
            int numStages;
            cout << "Enter the number of stages: ";
            while (!isValidInteger(numStages) || numStages <= 0) {
                cout << "Please enter a valid number for the number of stages: ";
            }

            LinkedList semesterSubjects; // To aggregate courses for overall GPA

            for (int stage = 1; stage <= numStages; ++stage) {
                int numSubjects;
                cout << "\nStage " << stage << ":\n";
                cout << "How many subjects do you have for this stage? ";
                while (!isValidInteger(numSubjects) || numSubjects <= 0) {
                    cout << "Please enter a valid number for the number of subjects: ";
                }

                GPACalculator stageCalculator; // For calculating GPA of the current stage

                for (int i = 0; i < numSubjects; ++i) {
                    string subName;
                    int creditsHr;
                    char grade;

                    cout << "\nSubject " << (i + 1) << ":\n";

                    cout << "Enter subject name: ";
                    getline(cin, subName);

                    cout << "Enter number of credits for " << subName << ": ";
                    while (!isValidInteger(creditsHr) || creditsHr <= 0) {
                        cout << "Please enter a valid number for the number of credits: ";
                    }

                    cout << "Enter grade for " << subName << " (A, B, C, D, F): ";
                    while (!isValidChar(grade) || !isValidGrade(grade)) {
                        cout << "Please enter a valid grade (A, B, C, D, F): ";
                    }

                    courseName coursename(subName, creditsHr, grade);
                    stageCalculator.addcourseName(coursename);
                    semesterSubjects.addcourseName(coursename); // Add to overall semester courses
                }

                double stageGPA = stageCalculator.calculateGPA();
                cout << "\nGPA for Stage " << stage << " is: " << stageGPA << endl;
                semesterGpaStack.push(stageGPA);  // Store the stage GPA in the stack
                stageCalculator.clearSubjects();  // Clear courses for the next stage
            }

            // Calculate overall GPA for the semester
            double semesterTotalCreditsHr = 0;
            double semesterWeightedSum = 0;

            Node* temp = semesterSubjects.getHead();

            while (temp) {
                semesterTotalCreditsHr += temp->coursename.getCreditsHr();
                semesterWeightedSum += temp->coursename.getCreditsHr() * GPACalculator().getGradePoints(temp->coursename.getGrade());
                temp = temp->next;
            }

            double semesterGPA = (semesterTotalCreditsHr == 0) ? 0 : semesterWeightedSum / semesterTotalCreditsHr;
            cout << "\nOverall GPA for Semester " << s << " is: " << semesterGPA << endl;
            semesterGpaStack.push(semesterGPA);  // Store the semester GPA in the stack
        }

        // Calculate overall GPA across all semesters
        double totalCreditsHr = 0;
        double weightedSum = 0;

        while (!semesterGpaStack.empty()) {
            double semesterGPA = semesterGpaStack.top();
            semesterGpaStack.pop();
            totalCreditsHr += 1; // Each semester is treated as 1 unit for averaging
            weightedSum += semesterGPA; // Sum of all semester GPAs
        }

        double overallGPA = (totalCreditsHr == 0) ? 0 : weightedSum / totalCreditsHr;
        cout << "\nOverall GPA for all semesters is: " << overallGPA << endl;

        cout << "\nWould you like to calculate another set of GPAs? (y/n): ";
        cin >> continueChoice;
        cin.ignore(10000, '\n'); // Clear the rest of the line

    } while (tolower(continueChoice) == 'y');

    cout << "\nThank you for using the GPA Calculator. Goodbye!" << endl;
    return 0;
}
