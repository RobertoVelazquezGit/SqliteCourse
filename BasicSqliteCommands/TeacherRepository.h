#pragma once

#include "Database.h"
#include "Teacher.h"

#include <vector>
#include <string>

class TeacherRepository
{
public:
    explicit TeacherRepository(Database& db);

    void insert(const Teacher& teacher);
    std::vector<Teacher> getAll();
    std::vector<std::string> getNames();

private:
    Database& db_;
};

