#pragma once

#include <sqlite3.h>
#include <string>
#include "Config.h"

class Statement
{
public:
    Statement(sqlite3* db, const std::string& sql);
    ~Statement();

    sqlite3_stmt* get() const { return stmt_; }

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
    void bind(int index, double value);

    // Column
    template<typename T>
    T column(int index) const;
	// Column name
    const char* columnName(int index) const;

#ifdef APP_DEBUG
    // Debug helper: prints column names of the prepared statement
    void debugPrintColumns(int columnCount) const;
#endif

private:
    sqlite3_stmt* stmt_{ nullptr };
};

// ============================================================
// Template specializations MUST be in the header
// ============================================================

// ---- int ----
template<>
inline int Statement::column<int>(int index) const
{
    return sqlite3_column_int(stmt_, index);
}

// ---- std::string ----
template<>
inline std::string Statement::column<std::string>(int index) const
{
    const unsigned char* text = sqlite3_column_text(stmt_, index);
    return text ? reinterpret_cast<const char*>(text) : "";
}

// ---- double ----
template<>
inline double Statement::column<double>(int index) const
{
    return sqlite3_column_double(stmt_, index);
}


