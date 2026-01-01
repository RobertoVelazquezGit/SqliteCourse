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

std::vector<StudentTeacherRow>
SchoolRepository::getStudentsWithTeachersCrossJoin()
{
    static const std::string sql =
        "SELECT DISTINCT "
        "students.name, students.rollno, students.dept, "
        "teacher.name, teacher.id "
        "FROM students "
        "CROSS JOIN teacher;";

    Statement stmt(db_.get(), sql);
    std::vector<StudentTeacherRow> result;

    while (stmt.step())
    {
        /*
            Column indices in the result set:

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

std::vector<std::string> SchoolRepository::getTableNames() const
{
    static const std::string sql =
        "SELECT name "
        "FROM sqlite_master "
        "WHERE type = 'table';";
    /*
        sqlite_master is an internal SQLite system table.

        It stores the schema of the database:
        - tables
        - indexes
        - views
        - triggers

        Each row in sqlite_master represents a database's object.

        The column 'type' tells what kind of object it is:
            'table'   → a table
            'index'   → an index
            'view'    → a view
            'trigger' → a trigger

        By filtering with:
            WHERE type = 'table'

        we ensure that only user tables are returned.
    */

    Statement stmt(db_.get(), sql);

    std::vector<std::string> tables;

    while (stmt.step())
    {
        // Column index 0 corresponds to the first column
        // in the SELECT clause: SELECT name
        tables.push_back(stmt.column<std::string>(0));
    }

    return tables;
}

std::vector<std::string>
SchoolRepository::getColumnNames(const std::string& tableName) const
{
    // PRAGMA statements cannot use bind parameters ('?'),
    // so the table name must be injected into the SQL string.
    const std::string sql =
        "PRAGMA table_info(" + tableName + ");";
    /*
        PRAGMA table_info(tableName) returns one row per column.

        The result columns are:
        0 → cid        (column index)
        1 → name       (column name)
        2 → type       (declared type)
        3 → notnull
        4 → dflt_value
        5 → pk

        We only care about column index 1 → column name.
    */

    Statement stmt(db_.get(), sql);
    std::vector<std::string> columns;

    while (stmt.step())
    {
        // Column 1 → column name
        columns.push_back(stmt.column<std::string>(1));
    }

    return columns;
}

std::vector<std::vector<SchoolRepository::Cell>>
SchoolRepository::selectAllTyped(const std::string& tableName) const
{
    const std::string sql =
        "SELECT * FROM " + tableName + ";";

    Statement stmt(db_.get(), sql);

    std::vector<std::vector<Cell>> rows;

    const int columnCount = sqlite3_column_count(stmt.get());

    while (stmt.step())
    {
        std::vector<Cell> row;
        row.reserve(columnCount);

        for (int i = 0; i < columnCount; ++i)
        {
            switch (sqlite3_column_type(stmt.get(), i))
            {
            case SQLITE_INTEGER:
                row.emplace_back(
                    sqlite3_column_int(stmt.get(), i)
                );
                break;

            case SQLITE_TEXT:
                row.emplace_back(
                    reinterpret_cast<const char*>(
                        sqlite3_column_text(stmt.get(), i)
                        )
                );
                break;

            default:
                row.emplace_back("NULL");
            }
        }

        rows.push_back(std::move(row));
    }

    return rows;
}
