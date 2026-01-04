#include "ProductsRepo.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
ProductsRepo::ProductsRepo(Database& db)
    : db_(db)
{
    // Nothing else to do here
}

// ------------------------------------------------------------
// Insert a single product
// ------------------------------------------------------------
void ProductsRepo::insert(const std::string& name, const std::string& description)
{
    // Create a prepared statement for inserting a product
    Statement stmt(db_.get(), R"sql(
        INSERT INTO Products (name, description) VALUES (?, ?);
    )sql");

    stmt.bind(1, name);         // Bind first parameter to product name
    stmt.bind(2, description);  // Bind second parameter to product description
    stmt.execute();             // Execute the insert statement
}

