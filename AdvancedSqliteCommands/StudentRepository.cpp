#include "StudentRepository.h"
#include "Statement.h"

StudentRepository::StudentRepository(Database& db)
	: db_(db)
{
}

void StudentRepository::insert(const Student& student)
{
	static const std::string sql =
		"INSERT INTO students (name, rollno, dept, course, age) "
		"VALUES (?, ?, ?, ?, ?);";

	Statement stmt(db_.get(), sql);

	stmt.bind(1, student.name);
	stmt.bind(2, student.rollno);
	stmt.bind(3, student.dept);
	stmt.bind(4, student.course);
	stmt.bind(5, student.age);

	stmt.execute();
}

std::vector<Student> StudentRepository::getAll()
{
	static const std::string sql =
		"SELECT name, rollno, dept, course, age FROM students;";

	Statement stmt(db_.get(), sql);
	std::vector<Student> students;

	while (stmt.step())
	{
		Student s;
		s.name = stmt.column<std::string>(0);
		s.rollno = stmt.column<int>(1);
		s.dept = stmt.column<std::string>(2);
		s.course = stmt.column<std::string>(3);
		s.age = stmt.column<int>(4);

		students.push_back(s);
	}

	return students;
}

std::vector<std::string> StudentRepository::getNames()
{
	static const std::string sql =
		"SELECT name FROM students;";

	Statement stmt(db_.get(), sql);
	std::vector<std::string> names;

	while (stmt.step())
	{
		names.push_back(
			stmt.column<std::string>(0)
		);
	}

	return names;
}

std::vector<std::string>
StudentRepository::getNamesOlderThan(int age)
{
	static const std::string sql =
		"SELECT name FROM students WHERE age > ?;";

	Statement stmt(db_.get(), sql);

	// Bind del parámetro
	stmt.bind(1, age);  // 1 referring to first (and only) ? in the SELECT

	std::vector<std::string> names;

	while (stmt.step())
	{
		names.push_back(
			stmt.column<std::string>(0)  // 0 referring to first (and only) column in the result set, name in this case
		);
	}

	return names;
}

std::vector<Student>
StudentRepository::getOlderThanInDepartment(int age,
	const std::string& dept)
{
	static const std::string sql =
		"SELECT * FROM students "
		"WHERE age > ? AND dept = ?;";

	Statement stmt(db_.get(), sql);

	// El orden importa: primer ? → age, segundo ? → dept
	stmt.bind(1, age);
	stmt.bind(2, dept);

	std::vector<Student> students;

	while (stmt.step())
	{
		Student s;

		// SELECT * → orden de columnas según CREATE TABLE
		s.name = stmt.column<std::string>(0);
		s.rollno = stmt.column<int>(1);
		s.dept = stmt.column<std::string>(2);
		s.course = stmt.column<std::string>(3);
		s.age = stmt.column<int>(4);

		students.push_back(std::move(s));
	}

	return students;
}

std::vector<Student>
StudentRepository::getNotInDepartment(const std::string& dept)
{
	static const std::string sql =
		"SELECT * FROM students "
		"WHERE NOT dept = ?;";

	Statement stmt(db_.get(), sql);

	// 1 → dept
	stmt.bind(1, dept);

	std::vector<Student> students;

	while (stmt.step())
	{
		Student s;

		// SELECT * → orden del CREATE TABLE
		s.name = stmt.column<std::string>(0);
		s.rollno = stmt.column<int>(1);
		s.dept = stmt.column<std::string>(2);
		s.course = stmt.column<std::string>(3);
		s.age = stmt.column<int>(4);

		students.push_back(std::move(s));
	}

	return students;
}

std::optional<std::string>
StudentRepository::getAnyNameOlderThan(int age)
{
	static const std::string sql =
		"SELECT name FROM students "
		"WHERE age > ? "
		"LIMIT 1;";

	Statement stmt(db_.get(), sql);

	// ─────────────────────────────────────────────
	// BIND DEL PARÁMETRO
	// ─────────────────────────────────────────────
	stmt.bind(1, age);
	/*
		El '1' se refiere al PRIMER parámetro '?' del SQL.

		Regla importante:
		- Los parámetros SQL ('?') se numeran desde 1
		- NO desde 0

		En esta consulta solo hay un '?':
			WHERE age > ?

		Por tanto:
			1 → ese único '?'
	*/

	// Ejecutamos la sentencia
	if (stmt.step())
	{
		// ─────────────────────────────────────────
		// LECTURA DE LA COLUMNA
		// ─────────────────────────────────────────
		return stmt.column<std::string>(0);
		/*
			El '0' se refiere a la PRIMERA columna del RESULT SET.

			Regla importante:
			- Las columnas del resultado se numeran desde 0
			- Dependen SOLO del orden del SELECT

			En esta consulta:
				SELECT name FROM students ...

			El result set tiene una sola columna:
				índice 0 → name
		*/
	}

	// Si no hubo ninguna fila
	return std::nullopt;
}

std::vector<std::pair<std::string, int>>
StudentRepository::getNameAndAgeBetween(int minAge, int maxAge)
{
	static const std::string sql =
		"SELECT name, age FROM students "
		"WHERE age BETWEEN ? AND ?;";

	Statement stmt(db_.get(), sql);

	// ───────────── BIND DE PARÁMETROS ─────────────
	stmt.bind(1, minAge); // primer '?' → mínimo age
	stmt.bind(2, maxAge); // segundo '?' → máximo age

	std::vector<std::pair<std::string, int>> result;

	while (stmt.step()) {
		std::string name = stmt.column<std::string>(0); // columna 0 = name
		int age = stmt.column<int>(1);         // columna 1 = age

		result.emplace_back(name, age);
	}

	return result;
}

std::vector<std::pair<std::string, int>>
StudentRepository::getNameAndAgeNotBetween(int minAge, int maxAge)
{
	static const std::string sql =
		"SELECT name, age FROM students "
		"WHERE age NOT BETWEEN ? AND ?;";

	Statement stmt(db_.get(), sql);

	// ───────────── BIND DE PARÁMETROS ─────────────
	stmt.bind(1, minAge); // primer '?' → mínimo age
	stmt.bind(2, maxAge); // segundo '?' → máximo age

	std::vector<std::pair<std::string, int>> result;

	while (stmt.step()) {
		std::string name = stmt.column<std::string>(0); // columna 0 = name
		int age = stmt.column<int>(1);         // columna 1 = age

		result.emplace_back(name, age);
	}

	return result;
}

std::vector<std::pair<std::string, std::string>>
StudentRepository::getNameAndDeptIn(const std::vector<std::string>& depts)
{
	if (depts.empty()) return {}; // nada que buscar

	// Construimos los placeholders '?' dinámicamente según el tamaño del vector
	std::string placeholders;
	for (size_t i = 0; i < depts.size(); ++i) {
		placeholders += (i == 0 ? "?" : ", ?");
	}

	std::string sql = "SELECT name, dept FROM students WHERE dept IN (" + placeholders + ");";

	Statement stmt(db_.get(), sql);

	// Bind dinámico de cada elemento del vector
	for (size_t i = 0; i < depts.size(); ++i) {
		stmt.bind(static_cast<int>(i + 1), depts[i]);
		// Recordatorio: bind empieza en 1
	}

	std::vector<std::pair<std::string, std::string>> result;

	while (stmt.step()) {
		std::string name = stmt.column<std::string>(0); // columna 0 = name
		std::string dept = stmt.column<std::string>(1); // columna 1 = dept
		result.emplace_back(name, dept);
	}

	return result;
}

std::vector<std::pair<std::string, std::string>>
StudentRepository::getNameAndDeptNotIn(const std::vector<std::string>& depts)
{
	// Si no hay departamentos a excluir, no filtramos nada
	if (depts.empty())
		return {};

	// Construimos dinámicamente: ?, ?, ?, ...
	std::string placeholders;
	for (size_t i = 0; i < depts.size(); ++i) {
		placeholders += (i == 0 ? "?" : ", ?");
	}

	std::string sql =
		"SELECT name, dept FROM students "
		"WHERE dept NOT IN (" + placeholders + ");";

	Statement stmt(db_.get(), sql);

	// Bind de cada valor del IN
	for (size_t i = 0; i < depts.size(); ++i) {
		stmt.bind(static_cast<int>(i + 1), depts[i]);
		// bind empieza en 1
	}

	std::vector<std::pair<std::string, std::string>> result;

	while (stmt.step()) {
		std::string name = stmt.column<std::string>(0); // 0 → name
		std::string dept = stmt.column<std::string>(1); // 1 → dept
		result.emplace_back(name, dept);
	}

	return result;
}

std::optional<int>
StudentRepository::getMinAge()
{
	static const std::string sql =
		"SELECT MIN(age) FROM students;";

	Statement stmt(db_.get(), sql);

	if (stmt.step()) {
		// Columna 0 → resultado de MIN(age)
		return stmt.column<int>(0);
	}

	return std::nullopt;
}

std::optional<int>
StudentRepository::getMaxAge()
{
	static const std::string sql =
		"SELECT MAX(age) FROM students;";

	Statement stmt(db_.get(), sql);

	if (stmt.step()) {
		// Columna 0 → resultado de MAX(age)
		return stmt.column<int>(0);
	}

	return std::nullopt;
}

std::optional<int>
StudentRepository::getNameCount()
{
	static const std::string sql =
		"SELECT COUNT(name) FROM students;";

	Statement stmt(db_.get(), sql);

	if (stmt.step()) {
		// Columna 0 → resultado de COUNT(name)
		return stmt.column<int>(0);
	}

	return std::nullopt;
}


std::optional<double>
StudentRepository::getAverageRollno()
{
	static const std::string sql =
		"SELECT AVG(rollno) FROM students;";

	Statement stmt(db_.get(), sql);

	if (stmt.step()) {
		// columna 0 → resultado de AVG(rollno)
		return stmt.column<double>(0);
	}

	return std::nullopt;
}

std::optional<double>
StudentRepository::getAgeSum()
{
	static const std::string sql =
		"SELECT SUM(age) FROM students;";

	Statement stmt(db_.get(), sql);

	if (stmt.step()) {
		// Columna 0 → resultado de SUM(age)
		return stmt.column<double>(0);
	}

	return std::nullopt;
}

std::optional<std::pair<std::string, int>>
StudentRepository::getMaxAgeWithColumnName()
{
	static const std::string sql =
		"SELECT MAX(age) AS \"maximum age\" FROM students;";

	Statement stmt(db_.get(), sql);

	// Execute the statement.
	// This query returns exactly ONE row if the table is not empty.
	if (stmt.step())
	{
		/*
			Column index explanation:

			- Column indices refer to the RESULT SET,
			  NOT to the table structure.

			- Indices start at 0.

			In this query:

				SELECT MAX(age) AS "maximum age"
				FROM students;

			The result set contains exactly ONE column:

				index 0 → MAX(age)   (aliased as "maximum age")

			Therefore:
				column 0 = the result of MAX(age)
		*/

		const char* columnName = stmt.columnName(0);
		int maxAge = stmt.column<int>(0);

		return std::make_pair(
			columnName ? columnName : "",
			maxAge
		);
	}

	// No row returned (e.g. empty table)
	return std::nullopt;
}

std::vector<std::pair<std::string, int>>
StudentRepository::getAllOrderedByNameAsc()
{
	static const std::string sql =
		"SELECT name, age FROM students ORDER BY name ASC;";

	Statement stmt(db_.get(), sql);
	std::vector<std::pair<std::string, int>> result;

	while (stmt.step())
	{
		std::string name = stmt.column<std::string>(0); // 0 → name
		int age = stmt.column<int>(1);                  // 1 → age
		result.emplace_back(name, age);
	}

	return result;
}

std::vector<std::pair<std::string, int>>
StudentRepository::getAllOrderedByAgeDesc()
{
	static const std::string sql =
		"SELECT name, age FROM students ORDER BY age DESC;";

	Statement stmt(db_.get(), sql);
	std::vector<std::pair<std::string, int>> result;

	while (stmt.step())
	{
		std::string name = stmt.column<std::string>(0); // 0 → name
		int age = stmt.column<int>(1);                  // 1 → age
		result.emplace_back(name, age);
	}

	return result;
}

std::vector<std::pair<int, int>>
StudentRepository::getMaxAgeGroupByRollno()
{
	static const std::string sql =
		"SELECT MAX(age), rollno FROM students GROUP BY rollno;";
	// GROUP BY generates a row per rollno

	Statement stmt(db_.get(), sql);
	std::vector<std::pair<int, int>> result;

	while (stmt.step())
	{
		int maxAge = stmt.column<int>(0);   // 0 → MAX(age)
		int rollno = stmt.column<int>(1);   // 1 → rollno
		result.emplace_back(maxAge, rollno);
	}

	return result;
}

std::vector<std::tuple<int, std::string, std::string>>
StudentRepository::getMaxAgeGroupedByNameInDept(const std::string& dept)
{
	static const std::string sql =
		"SELECT MAX(age), name, dept "
		"FROM students "
		"WHERE dept = ? "
		"GROUP BY name;";

	Statement stmt(db_.get(), sql);

	// Bind the department parameter
	stmt.bind(1, dept);
	/*
		Parameter index explanation:

		- SQL parameters ('?') are 1-based, not 0-based
		- This query has only one '?':
			  WHERE dept = ?
		- Therefore:
			  bind(1, dept) → binds that parameter
	*/

	std::vector<std::tuple<int, std::string, std::string>> result;

	while (stmt.step())
	{
		/*
			Result set column indices:

			0 → MAX(age)
			1 → name
			2 → dept
		*/

		int maxAge = stmt.column<int>(0);
		std::string name = stmt.column<std::string>(1);
		std::string deptValue = stmt.column<std::string>(2);

		result.emplace_back(maxAge, name, deptValue);
	}

	return result;
}


