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
