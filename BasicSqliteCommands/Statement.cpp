#include "Statement.h"
#include <stdexcept>

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
Statement::Statement(sqlite3* db, const std::string& sql)
{
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
Statement::~Statement()
{
    if (stmt_)
        sqlite3_finalize(stmt_);
}

// ------------------------------------------------------------
// Move semantics
// ------------------------------------------------------------
Statement::Statement(Statement&& other) noexcept
    : stmt_(other.stmt_)
{
    other.stmt_ = nullptr;
}

Statement& Statement::operator=(Statement&& other) noexcept
{
    if (this != &other)
    {
        sqlite3_finalize(stmt_);
        stmt_ = other.stmt_;
        other.stmt_ = nullptr;
    }
    return *this;
}

// ------------------------------------------------------------
// Ejecución
// ------------------------------------------------------------
bool Statement::step()
{
    return sqlite3_step(stmt_) == SQLITE_ROW;
}

void Statement::execute()
{
    if (sqlite3_step(stmt_) != SQLITE_DONE)
    {
        throw std::runtime_error("Statement execution failed");
    }
}

void Statement::reset()
{
    sqlite3_reset(stmt_);
    sqlite3_clear_bindings(stmt_);
}

// ------------------------------------------------------------
// Bind
// ------------------------------------------------------------
void Statement::bind(int index, int value)
{
    if (sqlite3_bind_int(stmt_, index, value) != SQLITE_OK)
    {
        throw std::runtime_error("Bind int failed");
    }
}

void Statement::bind(int index, const std::string& value)
{
    if (sqlite3_bind_text(
        stmt_,
        index,
        value.c_str(),
        -1,
        SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw std::runtime_error("Bind text failed");
    }
}
