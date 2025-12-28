#pragma once

#include "Database.h"
#include "Student.h"

#include <vector>

class StudentRepository
{
public:
    explicit StudentRepository(Database& db);

    void insert(const Student& student);
    std::vector<Student> getAll();

private:
    Database& db_;
};


