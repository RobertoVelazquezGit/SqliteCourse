#pragma once

#include "Database.h"
#include "Statement.h"
#include <string>

// ============================================================
// Repository for managing Articles table
// ============================================================
class ArticlesRepo
{
public:
    // Constructor: initialize repository with database reference
    explicit ArticlesRepo(Database& db);

    // Insert a single article into the Articles table
    void insert(const std::string& description,
        const std::string& part_number,
        const std::string& manufacturer,
        double price_unit);

private:
    Database& db_;  // Reference to the database connection
};

