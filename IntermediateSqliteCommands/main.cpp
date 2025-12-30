#include "Database.h"
#include "StudentRepository.h"
#include "TeacherRepository.h"

#include <iostream>

int main()
{
	try
	{
		// ----------------------------------------------------
		// 1. Abrir / crear base de datos
		// ----------------------------------------------------
		Database db("school.db", Database::Reset::Yes);

		// ----------------------------------------------------
		// 2. Crear tablas (solo una vez)
		// ----------------------------------------------------
		db.execute(
			"CREATE TABLE IF NOT EXISTS students ("
			"name TEXT, "
			"rollno INT, "
			"dept TEXT, "
			"course TEXT, "
			"age INT);"
		);

		db.execute(
			"CREATE TABLE IF NOT EXISTS teacher ("
			"name TEXT, "
			"id INT, "
			"dept TEXT, "
			"course TEXT, "
			"age INT);"
		);

		// ----------------------------------------------------
		// 3. Crear repositorios
		// ----------------------------------------------------
		StudentRepository studentRepo(db);
		TeacherRepository teacherRepo(db);

		// ----------------------------------------------------
		// 4. Insertar estudiantes
		// ----------------------------------------------------
		studentRepo.insert({ "arun", 100, "cse", "sql", 22 });
		studentRepo.insert({ "ajay kumar", 101, "cse", "java", 24 });
		studentRepo.insert({ "thor", 102, "ece", "python", 22 });
		studentRepo.insert({ "ironman", 103, "eee", "c", 27 });
		studentRepo.insert({ "spider man", 104, "ece", "java", 29 });

		// ----------------------------------------------------
		// 5. Insertar profesores
		// ----------------------------------------------------
		teacherRepo.insert({ "super man", 200, "cse", "maths", 30 });
		teacherRepo.insert({ "flash", 201, "ece", "physics", 40 });
		teacherRepo.insert({ "batman", 202, "it", "maths", 30 });
		teacherRepo.insert({ "ben", 203, "eee", "chemistry", 42 });
		teacherRepo.insert({ "joker", 204, "ece", "english", 50 });

		// ----------------------------------------------------
		// 6. Leer estudiantes
		// ----------------------------------------------------
		std::cout << "\nSTUDENTS\n";
		auto students = studentRepo.getAll();

		for (const auto& s : students)
		{
			std::cout
				<< s.name << " | "
				<< s.rollno << " | "
				<< s.dept << " | "
				<< s.course << " | "
				<< s.age << "\n";
		}

		// ----------------------------------------------------
		// 7. Leer profesores
		// ----------------------------------------------------
		std::cout << "\nTEACHERS\n";
		auto teachers = teacherRepo.getAll();

		for (const auto& t : teachers)
		{
			std::cout
				<< t.name << " | "
				<< t.id << " | "
				<< t.dept << " | "
				<< t.course << " | "
				<< t.age << "\n";
		}

		// Read students names
		auto studentNames = studentRepo.getNames();
		std::cout << "\nSTUDENT NAMES\n";
		for (const auto& name : studentNames)
		{
			std::cout << name << "\n";
		}

		// Read teachers names
		auto teacherNames = teacherRepo.getNames();
		std::cout << "\nTEACHER NAMES\n";
		for (const auto& name : teacherNames)
		{
			std::cout << name << "\n";
		}

		// Read distinct departments from teachers
		auto depts = teacherRepo.getDistinctDepartments();
		std::cout << "\nDISTINCT DEPT FROM TEACHER\n";
		for (const auto& d : depts)
		{
			std::cout << d << '\n';
		}

		// Read students older than 22
		auto names = studentRepo.getNamesOlderThan(22);
		std::cout << "\nSTUDENTS OLDER THAN 22\n";
		for (const auto& n : names)
		{
			std::cout << n << '\n';
		}

		// Teachers IDs from CSE department
		auto ids = teacherRepo.getIdsByDepartment("cse");
		std::cout << "\nTEACHERS ID FROM CSE DEPT\n";
		for (int id : ids)
		{
			std::cout << id << '\n';
		}

		// Students older than 22 in ECE department
		students.clear();
		students = studentRepo.getOlderThanInDepartment(20, "cse");
		std::cout << "\nSTUDENTS OLDER THAN n IN xxx DEPT\n";
		for (const auto& s : students)
		{
			std::cout
				<< s.name << " "
				<< s.rollno << " "
				<< s.dept << " "
				<< s.course << " "
				<< s.age << '\n';
		}


		// Students older than 22 or in cse department
		teachers.clear();
		teachers = teacherRepo.getOlderThanOrInDepartment(30, "cse");
		std::cout << "\nTEACHERS OLDER THAN n OR IN xxx DEPT\n";
		for (const auto& t : teachers)
		{
			std::cout
				<< t.name << " "
				<< t.id << " "
				<< t.dept << " "
				<< t.course << " "
				<< t.age << '\n';
		}
		
		// Students not in cse department
		students.clear();
		students = studentRepo.getNotInDepartment("cse");
		std::cout << "\nSTUDENTS NOT IN XXX DEPT\n";
		for (const auto& s : students)
		{
			std::cout
				<< s.name << " "
				<< s.rollno << " "
				<< s.dept << " "
				<< s.course << " "
				<< s.age << '\n';
		}

		// Any student name older than 20
		auto nameOpt = studentRepo.getAnyNameOlderThan(20);
		std::cout << "\nANY STUDENT NAME OLDER THAN n\n";
		if (nameOpt)
		{
			std::cout << "Found: " << *nameOpt << '\n';
		}
		else
		{
			std::cout << "No student found\n";
		}

		// Student names and ages between 20 and 25
		auto vec = studentRepo.getNameAndAgeBetween(20, 25);
		std::cout << "\nSTUDENTS BETWEEN n AND m\n";
		for (const auto& [name, age] : vec) {
			std::cout << name << " " << age << "\n";
		}

		// Student names and ages not between 20 and 25
		vec.clear();
		vec = studentRepo.getNameAndAgeNotBetween(20, 25);
		std::cout << "\nSTUDENTS NOT BETWEEN n AND m\n";
		for (const auto& [name, age] : vec) {
			std::cout << name << " " << age << "\n";
		}

		// Student names and departments in given departments
		std::vector<std::string> depts_ingiven = { "cse", "ece" };
		auto vec_ingiven = studentRepo.getNameAndDeptIn(depts_ingiven);
		std::cout << "\nSTUDENTS IN GIVEN DEPTS\n";
		for (const auto& [name, dept] : vec_ingiven) {  // structured binding c++17
			std::cout << name << " " << dept << "\n";
		}

		// Student names and departments not in given departments
		std::vector<std::string> excluded = { "cse", "ece" };
		auto vec_excluded = studentRepo.getNameAndDeptNotIn(excluded);
		std::cout << "\nSTUDENTS EXCLUDED FROM GIVEN DEPTS\n";
		for (const auto& [name, dept] : vec_excluded) {
			std::cout << name << " " << dept << "\n";
		}

		// Minimum and maximum age of students
		if (auto minAge = studentRepo.getMinAge()) {
			std::cout << "\nMIN AGE FROM STUDENTS\n";
			std::cout << "Min age: " << *minAge << "\n";
		}
		// Maximum age of students
		if (auto maxAge = studentRepo.getMaxAge()) {
			std::cout << "\nMAX AGE FROM STUDENTS\n";
			std::cout << "Max age: " << *maxAge << "\n";
		}


	}
	catch (const std::exception& ex)
	{
		std::cerr << "ERROR: " << ex.what() << "\n";
		return 1;
	}

	return 0;
}
