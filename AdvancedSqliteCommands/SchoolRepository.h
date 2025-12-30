#pragma once

#include "Database.h"
#include "Statement.h"
#include <vector>
#include <string>

class SchoolRepository
{
public:
    explicit SchoolRepository(Database& db);

    // Return all unique names from students and teachers
    std::vector<std::string> getAllNames();

    // Optional: other multi-table queries can go here

private:
    Database& db_;
};

