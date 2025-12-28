#include "StudentRepository.h"
#include "Statement.h"

StudentRepository::StudentRepository(Database& db)
    : db_(db)
{
}

void StudentRepository::insert(const Student& student)
{
    static const std::string sql =
        "INSERT INTO students (name, rollno, dept, course, age) "
        "VALUES (?, ?, ?, ?, ?);";

    Statement stmt(db_.get(), sql);

    stmt.bind(1, student.name);
    stmt.bind(2, student.rollno);
    stmt.bind(3, student.dept);
    stmt.bind(4, student.course);
    stmt.bind(5, student.age);

    stmt.execute();
}

std::vector<Student> StudentRepository::getAll()
{
    static const std::string sql =
        "SELECT name, rollno, dept, course, age FROM students;";

    Statement stmt(db_.get(), sql);
    std::vector<Student> students;

    while (stmt.step())
    {
        Student s;
        s.name = stmt.column<std::string>(0);
        s.rollno = stmt.column<int>(1);
        s.dept = stmt.column<std::string>(2);
        s.course = stmt.column<std::string>(3);
        s.age = stmt.column<int>(4);

        students.push_back(s);
    }

    return students;
}

