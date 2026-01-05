PRAGMA foreign_keys = ON;

CREATE TABLE Products (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    description TEXT
);

CREATE TABLE Articles (
    id INTEGER PRIMARY KEY,
    description TEXT NOT NULL COLLATE NOCASE,
    part_number TEXT NOT NULL COLLATE NOCASE,
    manufacturer TEXT NOT NULL COLLATE NOCASE,
    price_unit REAL NOT NULL DEFAULT 0.0,
    UNIQUE (part_number, manufacturer)
);

CREATE TABLE ProductArticles (
    product_id INTEGER NOT NULL,
    article_id INTEGER NOT NULL,
    designator TEXT NOT NULL,      -- R10, C3, U1
    quantity REAL NOT NULL DEFAULT 1,
    PRIMARY KEY (product_id, designator),
    FOREIGN KEY (product_id) REFERENCES Products(id) ON DELETE CASCADE,
    FOREIGN KEY (article_id) REFERENCES Articles(id) ON DELETE RESTRICT
);

-- Create 2 products
INSERT INTO Products (name, description)
VALUES
('Product A', 'PCB Product A'),
('Product B', 'PCB Product B');

-- Create 5 articles
INSERT INTO Articles (description, part_number, manufacturer, price_unit)
VALUES
('Resistor 10k 1%', 'R10K-0603', 'Yageo', 0.02),
('Resistor 1k 1%', 'R1K-0603', 'Yageo', 0.02),
('Capacitor 100nF', 'C100NF-0603', 'Kemet', 0.03),
('NPN Transistor', 'BC817', 'Nexperia', 0.10),
('Microcontroller', 'ATMEGA328P', 'Microchip', 2.50);

-- Insert articles into product a
INSERT INTO ProductArticles (product_id, article_id, designator, quantity)
VALUES
(1, 1, 'R10', 2),   -- Resistor 10k (quantity 2)
(1, 2, 'R5',  1),   -- Resistor 1k
(1, 3, 'C1',  1),   -- Capacitor
(1, 4, 'Q1',  1);   -- Transistor

-- Insert articles into product B
INSERT INTO ProductArticles (product_id, article_id, designator, quantity)
VALUES
(2, 1, 'R3',  1),   -- Resistor 10k (repeated from Product A)
(2, 3, 'C2',  3),   -- Capacitor (quantity 3)
(2, 4, 'Q2',  1),   -- Transistor
(2, 5, 'U1',  1);   -- Microcontroller

-- Articles in product a
SELECT
    p.name,
    pa.designator,
    a.description,
    pa.quantity
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 1
ORDER BY pa.designator;

-- Articles in product b
SELECT
    p.name,
    pa.designator,
    a.description,
    pa.quantity
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 2
ORDER BY pa.designator;

-- Price product a
SELECT
    p.name,                                  -- Product name
    SUM(pa.quantity * a.price_unit) AS total_price  -- Total cost of the product
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 1                               -- Product A
GROUP BY p.id;                               -- Group by product to sum all components

-- Price product b
SELECT
    p.name,                                  -- Product name
    SUM(pa.quantity * a.price_unit) AS total_price  -- Total cost of the product
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 2                               -- Product B
GROUP BY p.id;                               -- Group by product to sum all components

-- Price per component product a
SELECT
    a.description AS article_name,            -- Name / description of the article
    pa.quantity,                               -- Quantity of this article in the product
    a.price_unit,                              -- Unit price of the article
    (pa.quantity * a.price_unit) AS total_price -- Total price for this article
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 1                                -- Product A
ORDER BY a.description;                       -- Order by article name

-- Price per component product b
SELECT
    a.description AS article_name,            -- Name / description of the article
    pa.quantity,                               -- Quantity of this article in the product
    a.price_unit,                              -- Unit price of the article
    (pa.quantity * a.price_unit) AS total_price -- Total price for this article
FROM ProductArticles pa
JOIN Products p ON pa.product_id = p.id
JOIN Articles a ON pa.article_id = a.id
WHERE p.id = 2                                -- Product B
ORDER BY a.description;                       -- Order by article name

-- Delete a product by its name
DELETE FROM Products
WHERE name = 'Product B';  
-- Check executing Articles in product b

-- Insert Product B again into products
INSERT INTO Products (name, description)
VALUES ('Product B', 'PCB Product B');
-- Execute "Insert articles into product B"
-- Check executing "Articles in product b"




