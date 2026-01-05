#include "ProductArticlesRepo.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
ProductArticlesRepo::ProductArticlesRepo(Database& db)
    : db_(db)
{
    // Nothing else to do here
}

// ------------------------------------------------------------
// Insert a single article into a product
// ------------------------------------------------------------
void ProductArticlesRepo::insert(int product_id, int article_id,
    const std::string& designator, double quantity)
{
    // Create a prepared statement for inserting a ProductArticles row
    Statement stmt(db_.get(), R"sql(
        INSERT INTO ProductArticles (product_id, article_id, designator, quantity)
        VALUES (?, ?, ?, ?);
    )sql");

    stmt.bind(1, product_id);   // Bind product ID
    stmt.bind(2, article_id);   // Bind article ID
    stmt.bind(3, designator);   // Bind designator (e.g., R10, C1)
    stmt.bind(4, quantity);     // Bind quantity
    stmt.execute();             // Execute the insert
}

