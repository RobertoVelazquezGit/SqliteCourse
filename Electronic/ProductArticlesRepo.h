#pragma once

#include "Database.h"
#include "Statement.h"
#include <string>
#include <vector>

// ------------------------------------------------------------
// DTO representing the price breakdown of an article
// Used as a return type for price-related queries
// ------------------------------------------------------------
struct ArticlePriceItem
{
	std::string article_name;
	int quantity;
	double unit_price;
	double total_price;
};

// ============================================================
// Repository for managing ProductArticles table (BOM)
// ============================================================
class ProductArticlesRepo
{
public:
	struct Item
	{
		std::string product_name;  // Product name
		std::string designator;    // R10, C1, U1...
		std::string description;   // Article description
		double quantity;           // Quantity in the product
	};

	// Constructor: initialize repository with database reference
	explicit ProductArticlesRepo(Database& db);

	// Insert a single article into a product
	void insert(int product_id, int article_id,
		const std::string& designator, double quantity = 1.0);
	// Get all articles for a product by product name
	std::vector<Item> getByProductName(const std::string& product_name);
	// Get total price of a product by product name
	double getTotalPriceByProductName(const std::string& product_name);
	// Get price breakdown per article for a product
	std::vector<ArticlePriceItem>
		getPriceBreakdownByProductName(const std::string& product_name);

private:
	Database& db_;  // Reference to the database connection
};

