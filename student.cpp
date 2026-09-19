#include <iostream>
#include <string>
#include "student.h"
using namespace std;

//Parameterless constructor definition
Student::Student() {
	this->studentID = "";
	this->firstName = "";
	this->lastName = "";
	this->emailAddress = "";
	this->age = 0;
	for (int i = 0; i < 3; ++i) {
		this->daysInCourse[i] = 0;
	}
	this->degreeProgram = DegreeProgram::SOFTWARE;
}

//Constructor definition
Student::Student(string studentID, string firstName, string lastName, string emailAddress, int age, int* daysInCourse, DegreeProgram degreeProgram) {
	this->studentID = studentID;
	this->firstName = firstName;
	this->lastName = lastName;
	this->emailAddress = emailAddress;
	this->age = age;
	for (int i = 0; i < 3; ++i) {
		this->daysInCourse[i] = daysInCourse[i];
	}
	this->degreeProgram = degreeProgram;
}

//Accessors
string Student::getStudentID() {
	return studentID;
}
string Student::getFirstName() {
	return firstName;
}
string Student::getLastName() {
	return lastName;
}
string Student::getEmailAddress() {
	return emailAddress;
}
int Student::getAge() {
	return age;
}
int* Student::getDaysInCourse() {
	return daysInCourse;
}
DegreeProgram Student::getDegreeProgram() {
	return degreeProgram;
}

// Mutators
void Student::setStudentID(string studentID) {
	this->studentID = studentID;
}
void Student::setFirstName(string firstName) {
	this->firstName = firstName;
}
void Student::setLastName(string lastName) {
	this->lastName = lastName;
}
void Student::setEmailAddress(string emailAddress) {
	this->emailAddress = emailAddress;
}
void Student::setAge(int age) {
	this->age = age;
}
void Student::setDaysInCourse(int daysInCourse[]) {
	for (int i = 0; i < 3; ++i) {
		this->daysInCourse[i] = daysInCourse[i];
	}
}
void Student::setDegreeProgram(DegreeProgram degreeProgram) {
	this->degreeProgram = degreeProgram;
}

//Print method
void Student::print() {
	cout << getStudentID() << "\tFirst Name: " << getFirstName() << "\tLastName: " << getLastName();
	cout << "\tAge: " << getAge() << "\tDays in Course: " << " {" << getDaysInCourse()[0] << ", ";
	cout << getDaysInCourse()[1] << ", " << getDaysInCourse()[2] << "}" << "\tDegree Program: " << DEGREEPROGRAM[static_cast<int>(getDegreeProgram())] << endl;
}