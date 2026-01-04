#pragma once

#include "Database.h"
#include "Statement.h"
#include <string>

// ============================================================
// Repository for managing Products table
// ============================================================
class ProductsRepo
{
public:
    // Constructor: initialize repository with database reference
    explicit ProductsRepo(Database& db);

    // Insert a single product with name and description
    void insert(const std::string& name, const std::string& description);

private:
    Database& db_;  // Reference to the database connection
};

