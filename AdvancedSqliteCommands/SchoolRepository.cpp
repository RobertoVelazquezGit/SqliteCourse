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

