#include <iostream>
#include "roster.h"
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

json studentToJson(const Student& s) {
    const int* days = s.getDaysInCourse();
    return json{
        {"studentId", s.getStudentID()},
        {"firstName", s.getFirstName()},
        {"lastName", s.getLastName()},
        {"email", s.getEmailAddress()},
        {"age", s.getAge()},
        {"daysInCourse", {days[0], days[1], days[2]}},
        {"degreeProgram", Roster::degreeProgramToString(s.getDegreeProgram())}
    };
}

int main() {
    Roster roster("students.db");
    httplib::Server svr;

    // Allow the frontend (served from a different origin during local dev) to call this API
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });

    svr.Get("/students", [&](const httplib::Request&, httplib::Response& res) {
        json arr = json::array();
        for (const auto& s : roster.getAllStudents()) {
            arr.push_back(studentToJson(s));
        }
        res.set_content(arr.dump(), "application/json");
    });

    svr.Get(R"(/students/([^/]+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        auto student = roster.getStudentById(id);
        if (!student) {
            res.status = 404;
            res.set_content(json{{"error", "Student not found"}}.dump(), "application/json");
            return;
        }
        res.set_content(studentToJson(*student).dump(), "application/json");
    });

    svr.Get(R"(/students/([^/]+)/average)", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        auto avg = roster.getAverageDaysInCourse(id);
        if (!avg) {
            res.status = 404;
            res.set_content(json{{"error", "Student not found"}}.dump(), "application/json");
            return;
        }
        res.set_content(json{{"studentId", id}, {"averageDays", *avg}}.dump(), "application/json");
    });

    svr.Get("/students/invalid-emails", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(json(roster.getInvalidEmails()).dump(), "application/json");
    });

    svr.Get(R"(/students/degree/([^/]+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string programStr = req.matches[1];
        try {
            DegreeProgram program = Roster::stringToDegreeProgram(programStr);
            json arr = json::array();
            for (const auto& s : roster.getStudentsByDegree(program)) {
                arr.push_back(studentToJson(s));
            }
            res.set_content(arr.dump(), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(json{{"error", "Unknown degree program"}}.dump(), "application/json");
        }
    });

    svr.Post("/students", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            std::string id = body.at("studentId");

            if (roster.getStudentById(id)) {
                res.status = 409;
                res.set_content(json{{"error", "Student ID already exists"}}.dump(), "application/json");
                return;
            }

            std::string email = body.at("email");
            if (!Roster::isValidEmail(email)) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid email format"}}.dump(), "application/json");
                return;
            }

            DegreeProgram program = Roster::stringToDegreeProgram(body.at("degreeProgram").get<std::string>());
            auto days = body.at("daysInCourse");

            roster.add(id, body.at("firstName"), body.at("lastName"), email,
                       body.at("age").get<int>(),
                       days.at(0).get<int>(), days.at(1).get<int>(), days.at(2).get<int>(),
                       program);

            res.status = 201;
            res.set_content(studentToJson(*roster.getStudentById(id)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", std::string("Invalid request: ") + e.what()}}.dump(), "application/json");
        }
    });

    svr.Delete(R"(/students/([^/]+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        if (roster.removeById(id)) {
            res.status = 204;
        } else {
            res.status = 404;
            res.set_content(json{{"error", "Student not found"}}.dump(), "application/json");
        }
    });

    std::cout << "Student Roster API server running at http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
