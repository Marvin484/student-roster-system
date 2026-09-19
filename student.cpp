#include <iostream>
#include "student.h"

Student::Student()
    : studentID(""), firstName(""), lastName(""), emailAddress(""),
      age(0), daysInCourse{0, 0, 0}, degreeProgram(DegreeProgram::SOFTWARE) {
}

Student::Student(std::string studentID, std::string firstName, std::string lastName,
                  std::string emailAddress, int age, const int daysInCourse[3],
                  DegreeProgram degreeProgram)
    : studentID(std::move(studentID)), firstName(std::move(firstName)),
      lastName(std::move(lastName)), emailAddress(std::move(emailAddress)),
      age(age), degreeProgram(degreeProgram) {
    for (int i = 0; i < 3; ++i) {
        this->daysInCourse[i] = daysInCourse[i];
    }
}

std::string Student::getStudentID() const { return studentID; }
std::string Student::getFirstName() const { return firstName; }
std::string Student::getLastName() const { return lastName; }
std::string Student::getEmailAddress() const { return emailAddress; }
int Student::getAge() const { return age; }
const int* Student::getDaysInCourse() const { return daysInCourse; }
DegreeProgram Student::getDegreeProgram() const { return degreeProgram; }

void Student::setStudentID(const std::string& studentID) { this->studentID = studentID; }
void Student::setFirstName(const std::string& firstName) { this->firstName = firstName; }
void Student::setLastName(const std::string& lastName) { this->lastName = lastName; }
void Student::setEmailAddress(const std::string& emailAddress) { this->emailAddress = emailAddress; }
void Student::setAge(int age) { this->age = age; }
void Student::setDaysInCourse(const int daysInCourse[3]) {
    for (int i = 0; i < 3; ++i) {
        this->daysInCourse[i] = daysInCourse[i];
    }
}
void Student::setDegreeProgram(DegreeProgram degreeProgram) { this->degreeProgram = degreeProgram; }

void Student::print() const {
    std::cout << getStudentID() << "\tFirst Name: " << getFirstName()
              << "\tLastName: " << getLastName()
              << "\tAge: " << getAge()
              << "\tDays in Course: {" << getDaysInCourse()[0] << ", "
              << getDaysInCourse()[1] << ", " << getDaysInCourse()[2] << "}"
              << "\tDegree Program: "
              << DEGREE_PROGRAM_NAMES[static_cast<int>(getDegreeProgram())] << std::endl;
}
