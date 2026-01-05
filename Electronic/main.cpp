
#include <iostream>
#include "Database.h"
#include "ProductsRepo.h"
#include "ArticlesRepo.h"
#include "ProductArticlesRepo.h"

static void printProductBOM(ProductArticlesRepo& paRepo,
    const std::string& productName);
static void printProductTotalPrice(ProductArticlesRepo& paRepo,
    const std::string& productName);
static void printProductPriceBreakdown(ProductArticlesRepo& paRepo,
    const std::string& productName);

int main()
{
    try
    {
        // Configure SQLite to be fully thread-safe
        // Serialized mode enables internal mutexes
        if (sqlite3_config(SQLITE_CONFIG_SERIALIZED) != SQLITE_OK)
        {
            // sqlite3_config must be called before sqlite3_initialize / sqlite3_open
            throw std::runtime_error(
                "SQLite configuration failed: SQLITE_CONFIG_SERIALIZED"
            );
        }

        // Open / create the database "electronic.db"
        Database db("electronic.db", Database::Reset::Yes);

        // Enable foreign key constraints
        db.execute("PRAGMA foreign_keys = ON;");

        // --- Create Products table ---
		// Raw string c++ 11 R"sql( ... )sql", to avoid escaping quotes, write comments
        db.execute(R"sql(
            CREATE TABLE Products (
                id INTEGER PRIMARY KEY,              -- Unique product ID
                name TEXT NOT NULL,                  -- Product name
                description TEXT                     -- Product description
            );
        )sql");

        // --- Create Articles table ---
        db.execute(R"sql(
            CREATE TABLE Articles (
                id INTEGER PRIMARY KEY,              -- Unique article ID
                description TEXT NOT NULL COLLATE NOCASE,  -- Article description
                part_number TEXT NOT NULL COLLATE NOCASE,  -- Manufacturer part number
                manufacturer TEXT NOT NULL COLLATE NOCASE, -- Manufacturer name
                price_unit REAL NOT NULL DEFAULT 0.0,      -- Unit price
                UNIQUE (part_number, manufacturer)         -- Prevent duplicate part+manufacturer
            );
        )sql");

        // --- Create ProductArticles table ---
        db.execute(R"sql(
            CREATE TABLE ProductArticles (
                product_id INTEGER NOT NULL,         -- References Products(id)
                article_id INTEGER NOT NULL,         -- References Articles(id)
                designator TEXT NOT NULL,            -- PCB reference, e.g., R10, C3
                quantity REAL NOT NULL DEFAULT 1,    -- Quantity of this article
                PRIMARY KEY (product_id, designator),-- Unique designator per product
                FOREIGN KEY (product_id) REFERENCES Products(id) ON DELETE CASCADE,
                FOREIGN KEY (article_id) REFERENCES Articles(id) ON DELETE RESTRICT
            );
        )sql");

        std::cout << "\nDatabase and tables created successfully.\n";

        // Create products repository
        ProductsRepo productsRepo(db);

        // Insert products
        productsRepo.insert("Product A", "PCB Product A");
        productsRepo.insert("Product B", "PCB Product B");

        std::cout << "\nProducts inserted successfully.\n";

        // Create repository
        ArticlesRepo articlesRepo(db);

        // Insert articles
        articlesRepo.insert("Resistor 10k 1%", "R10K-0603", "Yageo", 0.02);
        articlesRepo.insert("Resistor 1k 1%", "R1K-0603", "Yageo", 0.02);
        articlesRepo.insert("Capacitor 100nF", "C100NF-0603", "Kemet", 0.03);
        articlesRepo.insert("NPN Transistor", "BC817", "Nexperia", 0.10);
        articlesRepo.insert("Microcontroller", "ATMEGA328P", "Microchip", 2.50);

        std::cout << "\nArticles inserted successfully.\n";

        // --- Assume tables already created ---
        ProductArticlesRepo paRepo(db);

        // --- Insert articles into Product A ---
        paRepo.insert(1, 1, "R10", 2);  // Resistor 10k (quantity 2)
        paRepo.insert(1, 2, "R5", 1);  // Resistor 1k
        paRepo.insert(1, 3, "C1", 1);  // Capacitor
        paRepo.insert(1, 4, "Q1", 1);  // Transistor

        // --- Insert articles into Product B ---
        paRepo.insert(2, 1, "R3", 1);  // Resistor 10k (repeated from Product A)
        paRepo.insert(2, 3, "C2", 3);  // Capacitor (quantity 3)
        paRepo.insert(2, 4, "Q2", 1);  // Transistor
        paRepo.insert(2, 5, "U1", 1);  // Microcontroller

        std::cout << "\nProduct articles inserted successfully.\n";

        // Print BOM for Product A
        printProductBOM(paRepo, "Product A");

        // Print BOM for Product B
        printProductBOM(paRepo, "Product B");

        // Print total prices
        printProductTotalPrice(paRepo, "Product A");
        printProductTotalPrice(paRepo, "Product B");

        // Print price breakdowns
        printProductPriceBreakdown(paRepo, "Product A");
        printProductPriceBreakdown(paRepo, "Product B");
    }
    catch (const std::exception& ex)
    {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}

// ------------------------------------------------------------
// Print the Bill Of Materials (BOM) for a given product
// ------------------------------------------------------------
static void printProductBOM(ProductArticlesRepo& paRepo,
    const std::string& productName)
{
    // Retrieve all articles for the given product name
    auto items = paRepo.getByProductName(productName);

    // Print BOM header
    std::cout << "\nBOM for product: " << productName << "\n";
    std::cout << "----------------------------------------\n";

    // Iterate through all BOM items and print them
    for (const auto& item : items)
    {
        std::cout
            << item.product_name << " | "
            << item.designator << " | "
            << item.description << " | "
            << item.quantity << "\n";
    }

    std::cout << std::endl;
}

// ------------------------------------------------------------
// Print the total price of a product
// ------------------------------------------------------------
static void printProductTotalPrice(ProductArticlesRepo& paRepo,
    const std::string& productName)
{
    // Get total price from the repository
    double totalPrice = paRepo.getTotalPriceByProductName(productName);

    // Print result
    std::cout
        << "\nTotal price for product \"" << productName << "\": "
        << totalPrice << " EUR\n\n";
}

// ------------------------------------------------------------
// Print price breakdown per article for a product
// ------------------------------------------------------------
static void printProductPriceBreakdown(ProductArticlesRepo& paRepo,
    const std::string& productName)
{
    auto items = paRepo.getPriceBreakdownByProductName(productName);

    std::cout << "\nPrice breakdown for product \"" << productName << "\":\n";

    for (const auto& item : items)
    {
        std::cout
            << item.article_name << " | "
            << "Qty: " << item.quantity << " | "
            << "Unit: " << item.unit_price << " EUR | "
            << "Total: " << item.total_price << " EUR\n";
    }

    std::cout << "\n";
}
