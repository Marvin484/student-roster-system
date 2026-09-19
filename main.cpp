#include <iostream>
#include <string>
#include "roster.h"
using namespace std;

int main() {

    // Student data table
    const string studentData[] = {
    "A1,John,Smith,John1989@gm ail.com,20,30,35,40,SECURITY",
    "A2,Suzan,Erickson,Erickson_1990@gmailcom,19,50,30,40,NETWORK",
    "A3,Jack,Napoli,The_lawyer99yahoo.com,19,20,40,33,SOFTWARE",
    "A4,Erin,Black,Erin.black@comcast.net,22,50,58,40,SECURITY",
    "A5,Jack,Joel,jack.joel@gmail.com,40,30,35,40,SOFTWARE"
    };

    
    Roster classRoster; // Creating an instance of the Roster class

    for (int i = 0; i < 5; ++i) {
        classRoster.parse(studentData[i]); // Add student to roster
    }

    classRoster.printAll(); // Print all students with their corresponding information
    classRoster.printInvalidEmails(); // Print invalid email addresses

    // Print average days in course for each student
    for (int i = 0; i < 5; ++i) {
        classRoster.printAverageDaysInCourse(classRoster.classRosterArray[i]->getStudentID());
    }
    
    classRoster.printByDegreeProgram(SOFTWARE); // Print students by degree program (Students in the software program)
    classRoster.remove("A3"); // Removing student by ID (A3)  
    classRoster.printAll(); // Print the updated roster
    classRoster.remove("A3"); // Try removing the same student by ID (A3) and get error

    return 0;
}