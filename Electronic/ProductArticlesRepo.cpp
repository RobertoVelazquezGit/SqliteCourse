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

std::vector<ProductArticlesRepo::Item>
ProductArticlesRepo::getByProductName(const std::string& product_name)
{
	std::vector<Item> result;

	Statement stmt(db_.get(), R"sql(
        SELECT
            p.name,           -- Column 0
            pa.designator,    -- Column 1
            a.description,    -- Column 2
            pa.quantity       -- Column 3
        FROM ProductArticles pa
        JOIN Products p ON pa.product_id = p.id
        JOIN Articles a ON pa.article_id = a.id
        WHERE p.name = ?      -- Parameter 1
        ORDER BY pa.designator;
    )sql");

	// Bind index starts at 1 (SQLite uses 1-based indexing for parameters)
	// This binds the first '?' in the SQL query to product_name
	stmt.bind(1, product_name);

	// Column index starts at 0 (SQLite uses 0-based indexing for result columns)
	while (stmt.step())
	{
		Item item;

		// Column 0 -> p.name (product name)
		item.product_name = stmt.column<std::string>(0);

		// Column 1 -> pa.designator (R10, C1, U1, ...)
		item.designator = stmt.column<std::string>(1);

		// Column 2 -> a.description (article description)
		item.description = stmt.column<std::string>(2);

		// Column 3 -> pa.quantity (quantity of the article in the product)
		item.quantity = stmt.column<double>(3);

		result.push_back(item);
	}

	return result;
}

// ------------------------------------------------------------
// Get total price of a product using product name
// ------------------------------------------------------------
double ProductArticlesRepo::getTotalPriceByProductName(
	const std::string& product_name)
{
	double totalPrice = 0.0;

	Statement stmt(db_.get(), R"sql(
        SELECT
            p.name,                                  -- Column 0: product name
            SUM(pa.quantity * a.price_unit)          -- Column 1: total price
        FROM ProductArticles pa
        JOIN Products p ON pa.product_id = p.id
        JOIN Articles a ON pa.article_id = a.id
        WHERE p.name = ?                             -- Parameter 1: product name
        GROUP BY p.id;
    )sql");

	// Bind index starts at 1 (binds the first '?' placeholder)
	stmt.bind(1, product_name);

	// Only one row is expected because we group by product
	if (stmt.step())
	{
		// Column 1 contains the calculated total price
		totalPrice = stmt.column<double>(1);
	}

	return totalPrice;
}

// ------------------------------------------------------------
// Get price breakdown per article using product name
// ------------------------------------------------------------
std::vector<ArticlePriceItem>
ProductArticlesRepo::getPriceBreakdownByProductName(
	const std::string& product_name)
{
	std::vector<ArticlePriceItem> items;

	Statement stmt(db_.get(), R"sql(
    SELECT
        a.description AS article_name,   -- Column 0
        pa.quantity AS quantity,          -- Column 1
        a.price_unit AS unit_price,       -- Column 2
        (pa.quantity * a.price_unit) AS total_price -- Column 3
    FROM ProductArticles pa
    JOIN Products p ON pa.product_id = p.id
    JOIN Articles a ON pa.article_id = a.id
    WHERE p.name = ?
    ORDER BY article_name;
    )sql");

	// Bind index starts at 1 and refers to the first '?' placeholder
	stmt.bind(1, product_name);

	// Iterate over all result rows
	while (stmt.step())
	{
		ArticlePriceItem item;

		// Column indices are zero-based
		item.article_name = stmt.column<std::string>(0); // a.description
		item.quantity = stmt.column<int>(1);         // pa.quantity
		item.unit_price = stmt.column<double>(2);      // a.price_unit
		item.total_price = stmt.column<double>(3);      // quantity * unit price

		items.push_back(item);
	}

#ifdef APP_DEBUG
	stmt.debugPrintColumns(4);
#endif

	return items;
}
