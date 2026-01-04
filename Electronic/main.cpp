
#include <iostream>
#include "Database.h"
#include "ProductsRepo.h"

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
    }
    catch (const std::exception& ex)
    {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
