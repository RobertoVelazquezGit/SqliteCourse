#pragma once

#include <string>
#include <sqlite3.h>

class Database
{
public:
    enum class Reset { No, Yes };
    explicit Database(const std::string& filename, Reset reset = Reset::No);   
    ~Database();

    sqlite3* get() const;
    void execute(const std::string& sql);

private:
    sqlite3* db_{ nullptr };
};


