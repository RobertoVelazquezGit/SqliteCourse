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
    std::vector<std::string> getDistinctDepartments();
    std::vector<int> getIdsByDepartment(const std::string& dept);
    std::vector<Teacher> getOlderThanOrInDepartment(
        int age,
        const std::string& dept);

private:
    Database& db_;
};

