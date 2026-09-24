#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "roster.h"
#include <cstdio>

namespace {
    const std::string TEST_DB = "test_students.db";
    void resetTestDb() {
        std::remove(TEST_DB.c_str());
    }
}

// --- Email validation ---

TEST_CASE("Valid email passes validation", "[email]") {
    REQUIRE(Roster::isValidEmail("jane.doe@example.com"));
}

TEST_CASE("Email missing @ fails validation", "[email]") {
    REQUIRE_FALSE(Roster::isValidEmail("jane.doeexample.com"));
}

TEST_CASE("Email missing period fails validation", "[email]") {
    REQUIRE_FALSE(Roster::isValidEmail("jane@doeexamplecom"));
}

TEST_CASE("Email containing a space fails validation", "[email]") {
    REQUIRE_FALSE(Roster::isValidEmail("jane doe@example.com"));
}

// --- CSV parsing ---

TEST_CASE("Parsing a well-formed CSV row adds the student", "[parse]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T1,Test,User,test.user@example.com,21,10,20,30,SECURITY");

    auto student = roster.getStudentById("T1");
    REQUIRE(student.has_value());
    REQUIRE(student->getFirstName() == "Test");
    REQUIRE(student->getDegreeProgram() == DegreeProgram::SECURITY);
}

TEST_CASE("Parsing a malformed CSV row does not crash and is skipped", "[parse]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T2,Bad,Row,bad@example.com,notanumber,10,20,30,SECURITY");

    auto student = roster.getStudentById("T2");
    REQUIRE_FALSE(student.has_value());
}

// --- Average days in course ---

TEST_CASE("Average days in course is calculated correctly", "[average]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T3,Avg,Test,avg.test@example.com,20,10,20,30,NETWORK");

    auto avg = roster.getAverageDaysInCourse("T3");
    REQUIRE(avg.has_value());
    REQUIRE(*avg == Approx(20.0));
}

TEST_CASE("Average days returns nullopt for an unknown student", "[average]") {
    resetTestDb();
    Roster roster(TEST_DB);
    auto avg = roster.getAverageDaysInCourse("NOPE");
    REQUIRE_FALSE(avg.has_value());
}

// --- Remove ---

TEST_CASE("removeById returns false for a student that does not exist", "[remove]") {
    resetTestDb();
    Roster roster(TEST_DB);
    REQUIRE_FALSE(roster.removeById("GHOST"));
}

TEST_CASE("removeById removes an existing student and returns true", "[remove]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T4,Remove,Me,remove.me@example.com,22,10,20,30,SOFTWARE");

    REQUIRE(roster.removeById("T4"));
    REQUIRE_FALSE(roster.getStudentById("T4").has_value());
}

// --- Regression test for the ORDER BY bug found during Phase 4 ---

TEST_CASE("getAllStudents returns rows ordered by student ID regardless of insertion order", "[order]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T9,Z,Last,z.last@example.com,20,10,20,30,SOFTWARE");
    roster.parse("T1,A,First,a.first@example.com,20,10,20,30,SOFTWARE");
    roster.parse("T5,M,Mid,m.mid@example.com,20,10,20,30,SOFTWARE");

    auto students = roster.getAllStudents();
    REQUIRE(students.size() == 3);
    REQUIRE(students[0].getStudentID() == "T1");
    REQUIRE(students[1].getStudentID() == "T5");
    REQUIRE(students[2].getStudentID() == "T9");
}

// --- Invalid email detection ---

TEST_CASE("Invalid emails are detected among seeded students", "[invalid-emails]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T6,Bad,Email,bademailexample.com,20,10,20,30,SOFTWARE");
    roster.parse("T7,Good,Email,good.email@example.com,20,10,20,30,SOFTWARE");

    auto invalid = roster.getInvalidEmails();
    REQUIRE(invalid.size() == 1);
    REQUIRE(invalid[0] == "bademailexample.com");
}

// --- Degree program filtering ---

TEST_CASE("getStudentsByDegree only returns matching students", "[degree]") {
    resetTestDb();
    Roster roster(TEST_DB);
    roster.parse("T8,Sec,Student,sec.student@example.com,20,10,20,30,SECURITY");
    roster.parse("T10,Net,Student,net.student@example.com,20,10,20,30,NETWORK");

    auto securityStudents = roster.getStudentsByDegree(DegreeProgram::SECURITY);
    REQUIRE(securityStudents.size() == 1);
    REQUIRE(securityStudents[0].getStudentID() == "T8");
}