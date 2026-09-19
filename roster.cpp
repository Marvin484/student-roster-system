#include <iostream>
#include <string>
#include <iomanip>
#include "roster.h"
using namespace std;

// Constructor
Roster::Roster() {
	lastIndex = -1; // Initializing lastIndex to -1 to indicate empty roster
}

// Destructor
Roster::~Roster() {
	for (int i = 0; i <= lastIndex; ++i) {
		delete classRosterArray[i];
	}
}

// Parse method
void Roster::parse(string row) {
	size_t column = row.find(",");
	string studentID = row.substr(0, column);

	size_t nextColumn = column + 1;
	column = row.find(",", nextColumn);
	string firstName = row.substr(nextColumn, column - nextColumn);

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	string lastName = row.substr(nextColumn, column - nextColumn);

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	string emailAddress = row.substr(nextColumn, column - nextColumn);

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	int age = stoi(row.substr(nextColumn, column - nextColumn));

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	int daysInCourse1 = stoi(row.substr(nextColumn, column - nextColumn));

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	int daysInCourse2 = stoi(row.substr(nextColumn, column - nextColumn));

	nextColumn = column + 1;
	column = row.find(",", nextColumn);
	int daysInCourse3 = stoi(row.substr(nextColumn, column - nextColumn));

	nextColumn = column + 1;
	string degreeProgramStr = row.substr(nextColumn);
	DegreeProgram degreeProgram;
	if (degreeProgramStr == "SECURITY") {
		degreeProgram = SECURITY;
	}
	else if (degreeProgramStr == "NETWORK") {
		degreeProgram = NETWORK;
	}
	else if (degreeProgramStr == "SOFTWARE") {
		degreeProgram = SOFTWARE;
	}

	add(studentID, firstName, lastName, emailAddress, age, daysInCourse1, daysInCourse2, daysInCourse3,
		degreeProgram);
}

// Add method
void Roster::add(string studentID, string firstName, string lastName, string emailAddress, int age,
	int daysInCourse1, int daysInCourse2, int daysInCourse3, DegreeProgram degreeProgram) {

	int daysInCourse[] = { daysInCourse1, daysInCourse2, daysInCourse3 };
	classRosterArray[++lastIndex] = new Student(studentID, firstName, lastName, emailAddress, age, daysInCourse, degreeProgram);

}

// Remove method
void Roster::remove(string studentID) {
	bool found = false;
	for (int i = 0; i <= lastIndex; ++i) {
		if (classRosterArray[i]->getStudentID() == studentID) {
			found = true;
			delete classRosterArray[i];
			cout << "Removing " << studentID << endl;
			for (int j = i; j < lastIndex; ++j) {
				classRosterArray[j] = classRosterArray[j + 1];
			}
			--lastIndex;
			break;
		}
	}
	if (!found) {
		cout << "Student with ID " << studentID << " was not found." << endl;
	}
}

// Print all method
void Roster::printAll() {
	for (int i = 0; i <= lastIndex; ++i) {
		classRosterArray[i]->print();
	}
}

// Print average days  in course method
void Roster::printAverageDaysInCourse(string studentID) {
	for (int i = 0; i <= lastIndex; ++i) {
		if (classRosterArray[i]->getStudentID() == studentID) {
			int* days = classRosterArray[i]->getDaysInCourse();
			cout << "Average days in course for student ID: " << studentID << " is ";
			cout << fixed << setprecision(4) << (days[0] + days[1] + days[2]) / 3.0 << endl;
			break;
		}
	}
}

// Print invalid emails method
void Roster::printInvalidEmails() {
	for (int i = 0; i <= lastIndex; ++i) {
		string email = classRosterArray[i]->getEmailAddress();
		if (email.find(' ') != string::npos || email.find('@') == string::npos || email.find('.') == string::npos) {
			cout << "Invalid email: " << email << endl;
		}
	}
}

// Print by degree program method
void Roster::printByDegreeProgram(DegreeProgram degreeProgram) {
	for (int i = 0; i <= lastIndex; ++i) {
		if (classRosterArray[i]->getDegreeProgram() == degreeProgram) {
			classRosterArray[i]->print();
		}
	}
}
