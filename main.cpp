#include <iostream>
#include <string>
#include "roster.h"

int main() {
    const std::string studentData[] = {
        "A1,John,Smith,John1989@gmail.com,20,30,35,40,SECURITY",
        "A2,Suzan,Erickson,Erickson_1990@gmail.com,19,50,30,40,NETWORK",
        "A3,Jack,Napoli,The_lawyer99yahoo.com,19,20,40,33,SOFTWARE", // intentionally malformed email to demo printInvalidEmails()
        "A4,Erin,Black,Erin.black@comcast.net,22,50,58,40,SECURITY",
        "A5,Jane,Doe,jane.doe@example.com,25,30,35,40,SOFTWARE"
    };

    std::cout << "Student Roster System" << std::endl;
    std::cout << "Language: C++" << std::endl << std::endl;

    Roster classRoster("students.db");

    // Seed only happens inside Roster if the table is empty on first run.
    // On subsequent runs, existing data in students.db is reused instead
    // of being re-inserted, since add() uses INSERT OR REPLACE keyed on student_id.
    for (const auto& row : studentData) {
        classRoster.parse(row);
    }

    classRoster.printAll();
    classRoster.printInvalidEmails();

    for (const auto& student : classRoster.getAllStudents()) {
        classRoster.printAverageDaysInCourse(student.getStudentID());
    }

    classRoster.printByDegreeProgram(DegreeProgram::SOFTWARE);
    classRoster.remove("A3");
    classRoster.printAll();
    classRoster.remove("A3");

    return 0;
}
