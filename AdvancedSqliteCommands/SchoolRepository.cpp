#include "SchoolRepository.h"

SchoolRepository::SchoolRepository(Database& db)
    : db_(db)
{
}

std::vector<std::string> SchoolRepository::getAllNames()
{
    static const std::string sql =
        "SELECT name FROM students "
        "UNION "
        "SELECT name FROM teacher;";

    Statement stmt(db_.get(), sql);
    std::vector<std::string> names;

    while (stmt.step())
    {
        /*
            Result set explanation:

            - This UNION query returns a single column:
                index 0 → name
            - Values come from students or teachers
            - Duplicates removed automatically by UNION
        */
        names.push_back(stmt.column<std::string>(0));
    }

    return names;
}

std::vector<StudentTeacherRow>
SchoolRepository::getStudentsWithTeachersByDept()
{
    static const std::string sql =
        "SELECT students.name, students.rollno, students.dept, "
        "teacher.name, teacher.id "
        "FROM students "
        "INNER JOIN teacher "
        "ON students.dept = teacher.dept;";

    Statement stmt(db_.get(), sql);
    std::vector<StudentTeacherRow> result;

    while (stmt.step())
    {
        /*
            Result set column indices:

            0 → students.name
            1 → students.rollno
            2 → students.dept
            3 → teacher.name
            4 → teacher.id
        */

        StudentTeacherRow row;
        row.studentName = stmt.column<std::string>(0);
        row.rollno = stmt.column<int>(1);
        row.dept = stmt.column<std::string>(2);
        row.teacherName = stmt.column<std::string>(3);
        row.teacherId = stmt.column<int>(4);

        result.push_back(std::move(row));
    }

    return result;
}
