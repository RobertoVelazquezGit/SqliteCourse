#pragma once

#include <vector>
#include <string>
#include "Database.h"
#include "Statement.h"

struct StudentTeacherRow
{
    std::string studentName;
    int rollno;
    std::string dept;
    std::string teacherName;
    int teacherId;
};

class SchoolRepository
{
public:
    explicit SchoolRepository(Database& db);
    std::vector<std::string> getAllNames();
    std::vector<StudentTeacherRow> getStudentsWithTeachersByDept();
    std::vector<StudentTeacherRow> getStudentsWithTeachersCrossJoin();

private:
    Database& db_;
};

