#pragma once

#include "Database.h"
#include "Teacher.h"

#include <vector>

class TeacherRepository
{
public:
    explicit TeacherRepository(Database& db);

    void insert(const Teacher& teacher);
    std::vector<Teacher> getAll();

private:
    Database& db_;
};

