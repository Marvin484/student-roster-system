#pragma once
#include <string>
#include <vector>
#include <optional>
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

    // Data-returning methods used by the REST API (no console output)
    std::vector<Student> getAllStudents() const;
    std::optional<Student> getStudentById(const std::string& studentID) const;
    std::vector<Student> getStudentsByDegree(DegreeProgram degreeProgram) const;
    std::optional<double> getAverageDaysInCourse(const std::string& studentID) const;
    std::vector<std::string> getInvalidEmails() const;
    bool removeById(const std::string& studentID); // same as remove() but returns success, no cout

    static bool isValidEmail(const std::string& email);
    static std::string degreeProgramToString(DegreeProgram dp);
    static DegreeProgram stringToDegreeProgram(const std::string& s);

private:
    sqlite3* db;

    void createTableIfNotExists();
    bool isEmpty() const;
    Student rowToStudent(sqlite3_stmt* stmt) const;
};
