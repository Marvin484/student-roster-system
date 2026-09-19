#pragma once
#include <string>
#include "degree.h"
using namespace std;

class Student {
public:
	Student(); //Parameterless constructor
	//Constructor
	Student(string studentID, string firstName, string lastName, string emailAddress, int age, int* daysInCourse, DegreeProgram degreeProgram);

	//Accessors
	string getStudentID();
	string getFirstName();
	string getLastName();
	string getEmailAddress();
	int getAge();
	int* getDaysInCourse();
	DegreeProgram getDegreeProgram();

	//Mutators
	void setStudentID(string studentID);
	void setFirstName(string firstName);
	void setLastName(string lastName);
	void setEmailAddress(string emailAddress);
	void setAge(int age);
	void setDaysInCourse(int daysInCourse[]);
	void setDegreeProgram(DegreeProgram degreeProgram);

	//Print method
	void print();


private:
	string studentID;
	string firstName;
	string lastName;
	string emailAddress;
	int age;
	int daysInCourse[3];
	DegreeProgram degreeProgram;
};
