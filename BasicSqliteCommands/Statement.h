#pragma once

#include <sqlite3.h>
#include <string>

class Statement
{
public:
    Statement(sqlite3* db, const std::string& sql);
    ~Statement();

    // No copiable
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    // Movimiento permitido (opcional pero pro)
    Statement(Statement&& other) noexcept;
    Statement& operator=(Statement&& other) noexcept;

    // Ejecución
    bool step();        // true si hay fila (SQLITE_ROW)
    void execute();     // para INSERT / UPDATE / DELETE
    void reset();

    // Bind
    void bind(int index, int value);
    void bind(int index, const std::string& value);

    // Column
    template<typename T>
    T column(int index) const;

private:
    sqlite3_stmt* stmt_{ nullptr };
};


