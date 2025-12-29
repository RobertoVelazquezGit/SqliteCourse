#pragma once

#include "Database.h"
#include "Student.h"

#include <vector>
#include <string>

class StudentRepository
{
public:
    explicit StudentRepository(Database& db);

    void insert(const Student& student);
    std::vector<Student> getAll();
    std::vector<std::string> getNames();
    std::vector<std::string> getNamesOlderThan(int age);

private:
    Database& db_;
};


