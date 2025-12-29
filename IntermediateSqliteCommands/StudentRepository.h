#pragma once

#include "Database.h"
#include "Student.h"

#include <vector>
#include <string>
#include <optional>

class StudentRepository
{
public:
    explicit StudentRepository(Database& db);

    void insert(const Student& student);
    std::vector<Student> getAll();
    std::vector<std::string> getNames();
    std::vector<std::string> getNamesOlderThan(int age);
    std::vector<Student> getOlderThanInDepartment(int age,
        const std::string& dept);
    std::vector<Student> getNotInDepartment(const std::string& dept);
    std::optional<std::string> getAnyNameOlderThan(int age);

private:
    Database& db_;
};


