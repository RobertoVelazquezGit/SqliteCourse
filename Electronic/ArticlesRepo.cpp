#include "ArticlesRepo.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
ArticlesRepo::ArticlesRepo(Database& db)
    : db_(db)
{
    // Nothing else to do here
}

// ------------------------------------------------------------
// Insert a single article
// ------------------------------------------------------------
void ArticlesRepo::insert(const std::string& description,
    const std::string& part_number,
    const std::string& manufacturer,
    double price_unit)
{
    // Create a prepared statement for inserting an article
    Statement stmt(db_.get(), R"sql(
        INSERT INTO Articles (description, part_number, manufacturer, price_unit)
        VALUES (?, ?, ?, ?);
    )sql");

    stmt.bind(1, description);     // Bind article description
    stmt.bind(2, part_number);     // Bind part number
    stmt.bind(3, manufacturer);    // Bind manufacturer
    stmt.bind(4, price_unit);      // Bind unit price
    stmt.execute();                // Execute the insert statement
}

