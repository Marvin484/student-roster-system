#pragma once
#include <string>
#include "degree.h"

class Student {
public:
    Student();
    Student(std::string studentID, std::string firstName, std::string lastName,
             std::string emailAddress, int age, const int daysInCourse[3],
             DegreeProgram degreeProgram);

    // Accessors
    std::string getStudentID() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmailAddress() const;
    int getAge() const;
    const int* getDaysInCourse() const;
    DegreeProgram getDegreeProgram() const;

    // Mutators
    void setStudentID(const std::string& studentID);
    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setEmailAddress(const std::string& emailAddress);
    void setAge(int age);
    void setDaysInCourse(const int daysInCourse[3]);
    void setDegreeProgram(DegreeProgram degreeProgram);

    void print() const;

private:
    std::string studentID;
    std::string firstName;
    std::string lastName;
    std::string emailAddress;
    int age;
    int daysInCourse[3];
    DegreeProgram degreeProgram;
};
