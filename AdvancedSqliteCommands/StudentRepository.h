#pragma once

#include <vector>
#include <string>
#include <optional>
#include <utility>
#include <tuple>

#include "Database.h"
#include "Student.h"

class StudentRepository
{
public:
    explicit StudentRepository(Database& db);

    void insert(const Student& student);
    std::vector<Student> getAll();
    std::vector<std::string> getNames();
    std::vector<std::string> getNamesOlderThan(int age);
    std::vector<Student> getOlderThanInDepartment(int age,
        const std::string& dept);
    std::vector<Student> getNotInDepartment(const std::string& dept);
    std::optional<std::string> getAnyNameOlderThan(int age);
    std::vector<std::pair<std::string, int>> getNameAndAgeBetween(int minAge, int maxAge);
    std::vector<std::pair<std::string, int>> getNameAndAgeNotBetween(int minAge, int maxAge);
    std::vector<std::pair<std::string, std::string>> getNameAndDeptIn(const std::vector<std::string>& depts);
    std::vector<std::pair<std::string, std::string>> getNameAndDeptNotIn(const std::vector<std::string>& depts);
    std::optional<int> getMinAge();
    std::optional<int> getMaxAge();
    std::optional<int> getNameCount();
    std::optional<double> getAverageRollno();
    std::optional<double> getAgeSum();
    std::optional<std::pair<std::string, int>> getMaxAgeWithColumnName();
    std::vector<std::pair<std::string, int>> getAllOrderedByNameAsc();
    std::vector<std::pair<std::string, int>> getAllOrderedByAgeDesc();
    std::vector<std::pair<int, int>> getMaxAgeGroupByRollno();
    std::vector<std::tuple<int, std::string, std::string>>
        getMaxAgeGroupedByNameInDept(const std::string& dept);
    std::vector<std::pair<std::string, std::string>>
        getNamesAndDeptsEndingWith(const std::string& suffix);

private:
    Database& db_;
};


