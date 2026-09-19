#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include "roster.h"

Roster::Roster() {}

void Roster::parse(const std::string& row) {
    try {
        size_t column = row.find(",");
        std::string studentID = row.substr(0, column);

        size_t nextColumn = column + 1;
        column = row.find(",", nextColumn);
        std::string firstName = row.substr(nextColumn, column - nextColumn);

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        std::string lastName = row.substr(nextColumn, column - nextColumn);

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        std::string emailAddress = row.substr(nextColumn, column - nextColumn);

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        int age = std::stoi(row.substr(nextColumn, column - nextColumn));

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        int daysInCourse1 = std::stoi(row.substr(nextColumn, column - nextColumn));

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        int daysInCourse2 = std::stoi(row.substr(nextColumn, column - nextColumn));

        nextColumn = column + 1;
        column = row.find(",", nextColumn);
        int daysInCourse3 = std::stoi(row.substr(nextColumn, column - nextColumn));

        nextColumn = column + 1;
        std::string degreeProgramStr = row.substr(nextColumn);
        DegreeProgram degreeProgram;
        if (degreeProgramStr == "SECURITY") {
            degreeProgram = DegreeProgram::SECURITY;
        } else if (degreeProgramStr == "NETWORK") {
            degreeProgram = DegreeProgram::NETWORK;
        } else if (degreeProgramStr == "SOFTWARE") {
            degreeProgram = DegreeProgram::SOFTWARE;
        } else {
            throw std::invalid_argument("Unknown degree program: " + degreeProgramStr);
        }

        add(studentID, firstName, lastName, emailAddress, age,
            daysInCourse1, daysInCourse2, daysInCourse3, degreeProgram);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Skipped malformed row (invalid number format): \"" << row
                  << "\" -- " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Skipped malformed row (number out of range): \"" << row
                  << "\" -- " << e.what() << std::endl;
    }
}

void Roster::add(const std::string& studentID, const std::string& firstName,
                  const std::string& lastName, const std::string& emailAddress, int age,
                  int daysInCourse1, int daysInCourse2, int daysInCourse3,
                  DegreeProgram degreeProgram) {
    int daysInCourse[] = { daysInCourse1, daysInCourse2, daysInCourse3 };
    classRosterArray.emplace_back(studentID, firstName, lastName, emailAddress,
                                   age, daysInCourse, degreeProgram);
}

void Roster::remove(const std::string& studentID) {
    auto it = std::find_if(classRosterArray.begin(), classRosterArray.end(),
        [&studentID](const Student& s) { return s.getStudentID() == studentID; });

    if (it != classRosterArray.end()) {
        std::cout << "Removing " << studentID << std::endl;
        classRosterArray.erase(it);
    } else {
        std::cout << "Student with ID " << studentID << " was not found." << std::endl;
    }
}

void Roster::printAll() const {
    for (const auto& student : classRosterArray) {
        student.print();
    }
}

void Roster::printAverageDaysInCourse(const std::string& studentID) const {
    for (const auto& student : classRosterArray) {
        if (student.getStudentID() == studentID) {
            const int* days = student.getDaysInCourse();
            std::cout << "Average days in course for student ID: " << studentID << " is ";
            std::cout << std::fixed << std::setprecision(4)
                      << (days[0] + days[1] + days[2]) / 3.0 << std::endl;
            return;
        }
    }
    std::cout << "Student with ID " << studentID << " was not found." << std::endl;
}

void Roster::printInvalidEmails() const {
    for (const auto& student : classRosterArray) {
        const std::string& email = student.getEmailAddress();
        if (email.find(' ') != std::string::npos ||
            email.find('@') == std::string::npos ||
            email.find('.') == std::string::npos) {
            std::cout << "Invalid email: " << email << std::endl;
        }
    }
}

void Roster::printByDegreeProgram(DegreeProgram degreeProgram) const {
    for (const auto& student : classRosterArray) {
        if (student.getDegreeProgram() == degreeProgram) {
            student.print();
        }
    }
}

const std::vector<Student>& Roster::getClassRosterArray() const {
    return classRosterArray;
}