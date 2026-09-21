#pragma once
#include <string>
#include <vector>
#include "student.h"
#include "sqlite3.h"

class Roster {
public:
    explicit Roster(const std::string& dbPath = "students.db");
    ~Roster();

    // Parse a CSV row and insert it (used for seeding initial data)
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

    // Builds a fresh in-memory snapshot from the database (used by main.cpp)
    std::vector<Student> getAllStudents() const;

private:
    sqlite3* db;

    void createTableIfNotExists();
    bool isEmpty() const;
    Student rowToStudent(sqlite3_stmt* stmt) const;

    static std::string degreeProgramToString(DegreeProgram dp);
    static DegreeProgram stringToDegreeProgram(const std::string& s);
};