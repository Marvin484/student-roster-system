#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "roster.h"

Roster::Roster(const std::string& dbPath) : db(nullptr) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }
    createTableIfNotExists();
}

Roster::~Roster() {
    if (db) {
        sqlite3_close(db);
    }
}

void Roster::createTableIfNotExists() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "student_id TEXT PRIMARY KEY,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "email TEXT NOT NULL,"
        "age INTEGER NOT NULL,"
        "days1 INTEGER NOT NULL,"
        "days2 INTEGER NOT NULL,"
        "days3 INTEGER NOT NULL,"
        "degree_program TEXT NOT NULL"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

bool Roster::isEmpty() const {
    const char* sql = "SELECT COUNT(*) FROM students;";
    sqlite3_stmt* stmt = nullptr;
    bool empty = true;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            empty = sqlite3_column_int(stmt, 0) == 0;
        }
    }
    sqlite3_finalize(stmt);
    return empty;
}

std::string Roster::degreeProgramToString(DegreeProgram dp) {
    return DEGREE_PROGRAM_NAMES[static_cast<int>(dp)];
}

DegreeProgram Roster::stringToDegreeProgram(const std::string& s) {
    if (s == "SECURITY") return DegreeProgram::SECURITY;
    if (s == "NETWORK") return DegreeProgram::NETWORK;
    if (s == "SOFTWARE") return DegreeProgram::SOFTWARE;
    throw std::invalid_argument("Unknown degree program: " + s);
}

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
        DegreeProgram degreeProgram = stringToDegreeProgram(degreeProgramStr);

        add(studentID, firstName, lastName, emailAddress, age,
            daysInCourse1, daysInCourse2, daysInCourse3, degreeProgram);
    } catch (const std::exception& e) {
        std::cerr << "Skipped malformed row: \"" << row << "\" -- " << e.what() << std::endl;
    }
}

void Roster::add(const std::string& studentID, const std::string& firstName,
                  const std::string& lastName, const std::string& emailAddress, int age,
                  int daysInCourse1, int daysInCourse2, int daysInCourse3,
                  DegreeProgram degreeProgram) {
    const char* sql =
        "INSERT OR REPLACE INTO students "
        "(student_id, first_name, last_name, email, age, days1, days2, days3, degree_program) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, emailAddress.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, age);
    sqlite3_bind_int(stmt, 6, daysInCourse1);
    sqlite3_bind_int(stmt, 7, daysInCourse2);
    sqlite3_bind_int(stmt, 8, daysInCourse3);
    sqlite3_bind_text(stmt, 9, degreeProgramToString(degreeProgram).c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to insert student: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}

void Roster::remove(const std::string& studentID) {
    const char* sql = "DELETE FROM students WHERE student_id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare delete: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    int changed = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    if (changed > 0) {
        std::cout << "Removing " << studentID << std::endl;
    } else {
        std::cout << "Student with ID " << studentID << " was not found." << std::endl;
    }
}

Student Roster::rowToStudent(sqlite3_stmt* stmt) const {
    std::string studentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    int age = sqlite3_column_int(stmt, 4);
    int days[3] = {
        sqlite3_column_int(stmt, 5),
        sqlite3_column_int(stmt, 6),
        sqlite3_column_int(stmt, 7)
    };
    std::string degreeStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    DegreeProgram degree = stringToDegreeProgram(degreeStr);

    return Student(studentID, firstName, lastName, email, age, days, degree);
}

std::vector<Student> Roster::getAllStudents() const {
    std::vector<Student> students;
    const char* sql = "SELECT student_id, first_name, last_name, email, age, days1, days2, days3, degree_program FROM students;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            students.push_back(rowToStudent(stmt));
        }
    }
    sqlite3_finalize(stmt);
    return students;
}

void Roster::printAll() const {
    for (const auto& student : getAllStudents()) {
        student.print();
    }
}

void Roster::printAverageDaysInCourse(const std::string& studentID) const {
    const char* sql = "SELECT days1, days2, days3 FROM students WHERE student_id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int d1 = sqlite3_column_int(stmt, 0);
            int d2 = sqlite3_column_int(stmt, 1);
            int d3 = sqlite3_column_int(stmt, 2);
            std::cout << "Average days in course for student ID: " << studentID << " is ";
            std::cout << std::fixed << std::setprecision(4) << (d1 + d2 + d3) / 3.0 << std::endl;
        } else {
            std::cout << "Student with ID " << studentID << " was not found." << std::endl;
        }
    }
    sqlite3_finalize(stmt);
}

void Roster::printInvalidEmails() const {
    for (const auto& student : getAllStudents()) {
        const std::string& email = student.getEmailAddress();
        if (email.find(' ') != std::string::npos ||
            email.find('@') == std::string::npos ||
            email.find('.') == std::string::npos) {
            std::cout << "Invalid email: " << email << std::endl;
        }
    }
}

void Roster::printByDegreeProgram(DegreeProgram degreeProgram) const {
    for (const auto& student : getAllStudents()) {
        if (student.getDegreeProgram() == degreeProgram) {
            student.print();
        }
    }
}