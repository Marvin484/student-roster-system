#pragma once
#include <string>
#include <vector>
#include "student.h"

class Roster {
public:
    Roster();
    // No custom destructor needed: std::vector<Student> manages its own
    // memory automatically, eliminating the manual new/delete bookkeeping
    // the original array-of-pointers design required.

    void parse(const std::string& row);

    void add(const std::string& studentID, const std::string& firstName,
              const std::string& lastName, const std::string& emailAddress, int age,
              int daysInCourse1, int daysInCourse2, int daysInCourse3,
              DegreeProgram degreeProgram);

    void remove(const std::string& studentID);
    void printAll() const;
    void printAverageDaysInCourse(const std::string& studentID) const;
    void printInvalidEmails() const;
    void printByDegreeProgram(DegreeProgram degreeProgram) const;

    // Read-only access so main.cpp can iterate without exposing internals directly
    const std::vector<Student>& getClassRosterArray() const;

private:
    std::vector<Student> classRosterArray;
};
