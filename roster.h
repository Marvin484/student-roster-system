#pragma once
#include <iostream>
#include <string>
#include "student.h"
using namespace std;

class Roster {
public:

	Student* classRosterArray[5]; // Array of pointers to hold student data

	Roster(); // Constructor
	~Roster(); // Destructor

	// Parse student data and add to the roster
	void parse(string row); 

	// Add to student and corresponding information to the roster
	void add(string studentID, string firstName, string lastName, string emailAddress, int age,
		int daysInCourse1, int daysInCourse2, int daysInCourse3, DegreeProgram degreeProgram);

	// Remove from the roster
	void remove(string studentID);

	// Print all students in the roster with their corresponding information
	void printAll();

	// Print average number of days in course for a student
	void printAverageDaysInCourse(string studentID);

	// Print invalid email addresses
	void printInvalidEmails();

	// Print students by degree program
	void printByDegreeProgram(DegreeProgram degreeProgram);

private:
	
	int lastIndex; // To track the last index filled into the array
};

