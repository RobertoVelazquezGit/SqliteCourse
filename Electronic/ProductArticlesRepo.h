#pragma once

#include "Database.h"
#include "Statement.h"
#include <string>

// ============================================================
// Repository for managing ProductArticles table (BOM)
// ============================================================
class ProductArticlesRepo
{
public:
    // Constructor: initialize repository with database reference
    explicit ProductArticlesRepo(Database& db);

    // Insert a single article into a product
    void insert(int product_id, int article_id,
        const std::string& designator, double quantity = 1.0);

private:
    Database& db_;  // Reference to the database connection
};

