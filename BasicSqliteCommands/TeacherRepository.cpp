#include "TeacherRepository.h"
#include "Statement.h"

TeacherRepository::TeacherRepository(Database& db)
    : db_(db)
{
}

void TeacherRepository::insert(const Teacher& teacher)
{
    static const std::string sql =
        "INSERT INTO teacher (name, id, dept, course, age) "
        "VALUES (?, ?, ?, ?, ?);";

    Statement stmt(db_.get(), sql);

    stmt.bind(1, teacher.name);
    stmt.bind(2, teacher.id);
    stmt.bind(3, teacher.dept);
    stmt.bind(4, teacher.course);
    stmt.bind(5, teacher.age);

    stmt.execute();
}

std::vector<Teacher> TeacherRepository::getAll()
{
    static const std::string sql =
        "SELECT name, id, dept, course, age FROM teacher;";

    Statement stmt(db_.get(), sql);
    std::vector<Teacher> teachers;

    while (stmt.step())
    {
        Teacher t;
        t.name = stmt.column<std::string>(0);
        t.id = stmt.column<int>(1);
        t.dept = stmt.column<std::string>(2);
        t.course = stmt.column<std::string>(3);
        t.age = stmt.column<int>(4);

        teachers.push_back(t);
    }

    return teachers;
}

