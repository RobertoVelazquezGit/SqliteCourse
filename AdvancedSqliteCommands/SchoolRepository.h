#pragma once

#include <vector>
#include <string>
#include <variant>
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
    std::vector<std::string> getTableNames() const;
    std::vector<std::string> getColumnNames(const std::string& tableName) const;    
    using Cell = std::variant<int, std::string>;
    std::vector<std::vector<Cell>> selectAllTyped(const std::string& tableName) const;

private:
    Database& db_;
};

