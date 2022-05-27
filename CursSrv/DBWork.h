#pragma once
#ifndef DBWORK_H
#define DBWORK_H

#include <cstdlib>
#include <iostream>
#include <utility>
#include "../Utils/stdafx.h"
#include <Windows.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "User.h"
#include "Company.h"
#include "Project.h"
#include "Mark.h"


using namespace std;

class DBWork {
private:
	std::string server;
	std::string username;
	std::string password;
	std::string database;
protected:
	sql::Driver* driver{};
	sql::Connection* con{};
	sql::PreparedStatement* pstmt{};
	sql::ResultSet* result{};
public:
	int _error = 0; 

	DBWork() {}

	DBWork(std::string _server, std::string _username, std::string _password, std::string _database) : 
		server(std::move(_server)), 
		username(std::move(_username)), 
		password(std::move(_password)), 
		database(std::move(_database)) 
	{
		try {
			driver = get_driver_instance();
			con = driver->connect(encryptChars(database) + server, encryptChars(username), encryptChars(password));
			std::cout << "----------------------------------------------------------" << std::endl;
			//std::cout << "Успешное соединение к mysql.services.clever-cloud.com." << std::endl;
			std::cout << "Успешное соединение к Базе данных." << std::endl;
		}
		catch (const sql::SQLException& e) {
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			system("pause");
			exit(1);
		}
		con->setSchema(encryptChars(database));

		std::cout << "Успешное открытие БД." << std::endl;
		std::cout << "----------------------------------------------------------" << std::endl;
		pstmt = con->prepareStatement("SET NAMES cp1251;");
		pstmt->execute();
	}

	void connect(std::string _server, std::string _username, std::string _password, std::string _database) {
		server = std::move(_server);
		username = std::move(_username);
		password = std::move(_password);
		database = std::move(_database);
		_error = 0;
		// Подключение к серверу
		try {
			driver = get_driver_instance();
			con = driver->connect(encryptChars(server), encryptChars(username), encryptChars(password));
			//con = driver->connect(server, username, password);
			std::cout << "----------------------------------------------------------" << std::endl;
			//std::cout << "Успешное соединение к mysql.services.clever-cloud.com." << std::endl;
			std::cout << "Успешное соединение к Базе Данных." << std::endl;
		}
		catch (const sql::SQLException& e) {
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			_error = -1;
			system("pause");
			return;
			//exit(1);
		}
		// Подключение к БД
		try {
			con->setSchema(encryptChars(database));
			//con->setSchema(database);

			std::cout << "Успешное открытие БД." << std::endl;
			std::cout << "----------------------------------------------------------" << std::endl;

			pstmt = con->prepareStatement("SET NAMES cp1251;");
			pstmt->execute();
		}
		catch (const sql::SQLException& e) {
			cout << "Could not connect to database. Error message: " << e.what() << endl;
			_error = -1;
			system("pause");
			return;
		}
	}

	~DBWork() {
		delete result;
		delete pstmt;
		delete con;
	}

	// Получение справочников
	// _table : таблица
	// _numField : номер поля
	// exp :   = 1 - для отбора экспертов (по умолчанию 0)
	//			 2 - для не Администратора
	std::vector<std::string> getGuide(const std::string& _table, size_t _numField, size_t exp = 0) {
		vector<string> tmp;
		std::string str; // = "SELECT * FROM " + _table;
		//std::string
		if (exp == 1) str = " WHERE name = 'Эксперт';";
		if (exp == 2) {
			str = " WHERE NOT name = 'Менеджер';";
		}

		try {
			pstmt = con->prepareStatement("SELECT * FROM " + _table + str + ";");
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {Select user_role} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
		while (result->next()) {
			//tmp.push_back(result->getString(_numField).c_str());
			tmp.emplace_back(result->getString(_numField).c_str());
		}
		delete result;
		return tmp;
	}

	std::map<string, size_t> getGuideMap(const std::string& _table, size_t _numField, size_t exp = 0) {
		std::map<std::string, size_t> tmp;
		std::string str;
		if (exp == 1) str = " WHERE role = 'Эксперт'";
		if (exp == 2) str = " WHERE NOT role = 'Администратор'";
		pstmt = con->prepareStatement("SELECT * FROM " + _table + str + ";");
		result = pstmt->executeQuery();
		while (result->next()) {
			//printf("%s\n", result->getString(_numField).c_str());
			//tmp.push_back(result->getString(_numField).c_str());
			tmp.insert(make_pair(result->getString(_numField).c_str(), result->getInt(1)));
		}
		delete result;
		return tmp;
	}

	//--------- User --------
	// Получение ролей
	vector<string> getRoles() {
		vector<string> tmp;
		pstmt = con->prepareStatement("SELECT * FROM user_role;");
		result = pstmt->executeQuery();
		while (result->next()) {
			printf("%s\n", result->getString(1).c_str());
			//tmp.push_back(result->getString(1).c_str());
			tmp.emplace_back(result->getString(1).c_str());
		}
		delete result;
		//setlocale(LC_ALL, ".1251");
		return tmp;
	}
	// Добавить пользователя
	void addUser(User& user) {
		try {
			pstmt = con->prepareStatement("INSERT INTO user (user_name, login, pass, role) VALUES(?, ?, ?, ?);");
			pstmt->setString(1, user.getName());
			pstmt->setString(2, user.getLogin());
			pstmt->setString(3, user.getPass());
			pstmt->setString(4, user.getRole());
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {INSERT user} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	// Редактировать пользователя
	void editUser(User& user, size_t u_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE user SET user_name = ?, login = ?, pass = ?, role = ? where user_id = ?;");
			pstmt->setString(1, user.getName());
			pstmt->setString(2, user.getLogin());
			pstmt->setString(3, user.getPass());
			pstmt->setString(4, user.getRole());
			pstmt->setInt(5, u_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {UPDATE user}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	// Получить пользователя
	// field_name - поле для отбора
	// u_name - значение поля
	User getUser(const std::string& field_name, const std::string& u_name) {
		User tmp;
		pstmt = con->prepareStatement("SELECT * FROM user WHERE " + field_name + " = ?;");
		pstmt->setString(1, u_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setUid(result->getInt(1));
			tmp.setUser(result->getString(2), result->getString(3), result->getString(4), result->getString(5));
		}
		delete result;
		return tmp;
	}

	// Получить всех пользователей
	std::vector<User> getUsers(const std::string& findUser = "") {
		User tmp;
		std::vector<User> tmpU;
		std::string strSearch;
		if (!findUser.empty()) {
			strSearch = " WHERE user_name LIKE '%" + findUser + "%'";
		}
		pstmt = con->prepareStatement("SELECT * FROM user" + strSearch + ";");
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setUid(result->getInt(1));
			tmp.setUser(result->getString(2), result->getString(3), result->getString(4), result->getString(5));
			tmpU.push_back(tmp);
		}
		delete result;
		return tmpU;
	}

	// Удалить пользователя по id
	void deleteUser(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM user WHERE user_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE user user_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Удалить пользователя по логину
	void deleteUser(const std::string& _login) {
		try {
			pstmt = con->prepareStatement("DELETE FROM user WHERE login = ?;");
			pstmt->setString(1, _login);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE user login} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	//------- Company -------
	// Добавить компанию
	void addCompany(const Company& company) {
		try {
			pstmt = con->prepareStatement("INSERT INTO company (company_name, activity, finance) VALUES(?, ?, ?);");
			pstmt->setString(1, company.getName());
			pstmt->setString(2, company.getActivity());
			pstmt->setInt(3, company.getFinance());
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {INSERT company} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	// Редактировать компанию
	void editCompany(const Company& company, size_t c_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE company SET company_name = ?, activity = ?, finance = ? where company_id = ?;");
			pstmt->setString(1, company.getName());
			pstmt->setString(2, company.getActivity());
			pstmt->setInt(3, company.getFinance());
			pstmt->setInt(4, c_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {UPDATE company}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Получить компанию
	Company getCompany(const std::string& field_name, const std::string& u_name) {
		Company tmp;
		pstmt = con->prepareStatement("SELECT * FROM company WHERE " + field_name + " = ?;");
		pstmt->setString(1, u_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setId(result->getInt(1));
			tmp.setCompany(result->getString(2), result->getString(3), result->getInt(4));
		}
		delete result;
		return tmp;
	}

	// Получить компании
	std::vector<Company> getCompanies(const std::string& findCompany = "") {
		std::vector<Company> tmpVc;
		Company tmp;
		std::string strSearch;
		if (!findCompany.empty()) {
			strSearch = " WHERE company_name LIKE '%" + findCompany + "%'";
		}
		pstmt = con->prepareStatement("SELECT * FROM company" + strSearch + ";");
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setId(result->getInt(1));
			tmp.setCompany(result->getString(2), result->getString(3), result->getInt(4));
			tmpVc.push_back(tmp);
		}
		delete result;
		return tmpVc;
	}

	// Удалить компанию по id
	void deleteCompany(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM company WHERE company_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE company company_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Удалить пользователя по названию
	void deleteCompany(const std::string& _name) {
		try {
			pstmt = con->prepareStatement("DELETE FROM company WHERE company_name = ?;");
			pstmt->setString(1, _name);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE company company_name} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	//------- Project -------
	// Добавить проект
	void addProject(const Project& project) {
		try {
			pstmt = con->prepareStatement(
				"INSERT INTO project (project_name, sum_credit, credit_time, sud_reestr, application_date, company_id) VALUES(?, ?, ?, ?, ?, ?);");
			pstmt->setString(1, project.getProjectName());
			pstmt->setInt(2, project.getSumCredit());
			pstmt->setInt(3, project.getCreditTime());
			pstmt->setString(4, project.getSudReestr());
			pstmt->setString(5, project.getApplicationDate().getDateStr());
			pstmt->setInt(6, project.getCompanyId());
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {INSERT project} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Редактировать проект
	void editProject(const Project& project, size_t p_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE project SET project_name = ?, sum_credit = ?, credit_time = ?, sud_reestr = ?, application_date = ?, company_id = ? where project_id = ?;");
			pstmt->setString(1, project.getProjectName());
			pstmt->setInt(2, project.getSumCredit());
			pstmt->setInt(3, project.getCreditTime());
			pstmt->setString(4, project.getSudReestr());
			pstmt->setString(5, project.getApplicationDate().getDateStr());
			pstmt->setInt(6, project.getCompanyId());
			pstmt->setInt(7, p_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {UPDATE project}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Получить проект
	Project getProject(const std::string& field_name, const std::string& p_name) {
		Project tmp;
		pstmt = con->prepareStatement("SELECT * FROM project WHERE " + field_name + " = ?;");
		pstmt->setString(1, p_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setProject(result->getInt(1), result->getString(2), result->getInt(3), result->getInt(4),
				result->getString(5), result->getString(6), result->getInt(7));
		}
		delete result;
		return tmp;
	}
	// Получение map проектов
	std::map<std::string, Project> getProjectMp(/*const std::string& field_name, const std::string& p_name*/) {
		Project tmp;
		std::map<std::string, Project> tmpMp;
		pstmt = con->prepareStatement("SELECT * FROM project;");
		//pstmt->setString(1, p_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setProject(result->getInt(1), result->getString(2), result->getInt(3), result->getInt(4),
				result->getString(5), result->getString(6), result->getInt(7));
			tmpMp.insert(make_pair(tmp.getProjectName(), tmp));
		}
		delete result;
		return tmpMp;
	}

	// Получение vector проектов
	std::vector<Project> getProjects(const std::string& findProject = "", const std::string& orderField = "" /*const std::string& field_name, const std::string& p_name*/) {
		Project tmp;
		std::vector<Project> tmpVc;
		std::string strSearch, strOrder;
		if (!findProject.empty()) {
			strSearch = " WHERE project_name LIKE '%" + findProject + "%'";
		}
		if (!orderField.empty()) {
			strOrder = " ORDER BY " + orderField;
		}
		pstmt = con->prepareStatement("SELECT * FROM project" + strSearch + strOrder + ";");
		//pstmt->setString(1, p_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setProject(result->getInt(1), result->getString(2), result->getInt(3), result->getInt(4),
				result->getString(5), result->getString(6), result->getInt(7));
			tmpVc.push_back(tmp);
		}
		delete result;
		return tmpVc;
	}

	// Удалить проект по id
	void deleteProject(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM project WHERE project_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE project project_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Удалить проект по названию
	void deleteProject(const std::string& _name) {
		try {
			pstmt = con->prepareStatement("DELETE FROM project WHERE project_name = ?;");
			pstmt->setString(1, _name);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE project project_name} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	//------- Mark -------
	// Добавить оценку
	void addMark(const Mark& mark) {
		try {
			pstmt = con->prepareStatement(
				"INSERT INTO mark (number, user_id, project1_id, project2_id, mark1, mark2) VALUES(?, ?, ?, ?, ?, ?);");
			pstmt->setInt(1, mark.getNumber());
			pstmt->setInt(2, mark.getUserId());
			pstmt->setInt(3, mark.getProject1Id());
			pstmt->setInt(4, mark.getProject2Id());
			pstmt->setDouble(5, mark.getValue1());
			pstmt->setDouble(6, mark.getValue2());
			//pstmt->setInt(6, m_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {INSERT mark} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	void addMarkVc(const std::vector<Mark>& _mark) {
		for (auto&& it : _mark) {
			addMark(it);
		}
	}

	// Редактировать оценку
	void editMark(const Mark& mark, size_t m_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE mark SET number = ?, user_id = ?, project1_id = ?, project2_id = ?, mark1 = ?, mark2 =? where mark_id = ?;");
			pstmt->setInt(1, mark.getNumber());
			pstmt->setInt(2, mark.getUserId());
			pstmt->setInt(3, mark.getProject1Id());
			pstmt->setInt(4, mark.getProject2Id());
			pstmt->setDouble(5, mark.getValue1());
			pstmt->setDouble(6, mark.getValue2());
			pstmt->setInt(7, m_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {UPDATE mark}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Получить оценку
	Mark getMark(const std::string& field_name, size_t m_id) {
		Mark tmp;
		pstmt = con->prepareStatement("SELECT mark_id, number, user_id, project1_id, project2_id, ROUND(mark1, 2), ROUND(mark2, 2) FROM mark WHERE " + field_name + " = ?;");
		pstmt->setInt(1, m_id);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setMark(result->getInt(1), result->getInt(2), result->getInt(3), result->getInt(4), result->getInt(5),
				result->getDouble(6)); //, result->getDouble(7)
		}
		delete result;
		return tmp;
	}

	// Получить map оценок для набора и пользователя
	std::map<size_t, std::vector<Mark>> getMpMarks(size_t _number, size_t _userId = 0) {
		Mark tmp;
		std::map<size_t, std::vector<Mark>> tmpRn;
		std::vector<Mark> tmpMrk;
		std::string strUserId;
		if (_userId != 0) strUserId = " AND user_id = " + std::to_string(_userId);
		pstmt = con->prepareStatement("SELECT mark_id, number, user_id, project1_id, project2_id, ROUND(mark1, 2), ROUND(mark2, 2) FROM mark WHERE number = ?" + strUserId + ";"); //  ORDER BY user_id, project1_id, project2_id // AND user_id = ? ; ");
		pstmt->setInt(1, _number);
		//pstmt->setInt(2, _userId);
		size_t lastUser = -1;
		result = pstmt->executeQuery();
		while (result->next()) {
			//setMark(size_t markId, size_t _number, size_t userId, size_t project1Id, size_t project2Id, float _value)
			tmp.setMark(result->getInt(1), result->getInt(2), result->getInt(3), result->getInt(4), result->getInt(5),
				result->getDouble(6)); //, result->getDouble(7)
			std::cout << tmp;
			if (lastUser == -1) lastUser = tmp.getUserId();
			if (lastUser != -1 && lastUser != tmp.getUserId()) {
				tmpRn.insert(make_pair(lastUser, tmpMrk));
				lastUser = tmp.getUserId();
				tmpMrk.clear();
			}
			tmpMrk.push_back(tmp);
		}
		delete result;
		if (lastUser != -1) tmpRn.insert(make_pair(lastUser, tmpMrk));
		return tmpRn;
	}

	// Получение экспертов из ранжа
	std::map<string, size_t> getExpertMapMark(size_t _number) {
		std::map<std::string, size_t> tmp;
		std::string strQuery;
		strQuery = "SELECT user.user_name, mark.user_id ";
		strQuery += "FROM mark Inner Join user ON mark.user_id = user.user_id ";
		strQuery += "WHERE mark.number = ? ";
		strQuery += "GROUP BY mark.user_id ORDER BY mark.user_id ASC;";

		pstmt = con->prepareStatement(strQuery);
		pstmt->setInt(1, _number);
		result = pstmt->executeQuery();
		while (result->next()) {
			//printf("%s\n", result->getString(_numField).c_str());
			//tmp.push_back(result->getString(_numField).c_str());
			tmp.insert(make_pair(result->getString(1).c_str(), result->getInt(2)));
		}
		delete result;
		return tmp;
	}

	// Получение проектов из ранжа
	std::vector<Project> getProjectVcMark(size_t _number) {
		std::vector<Project> tmpPr;
		Project tmp;
		std::string strQuery;

		// Запрос проектов в ранже

		strQuery = "SELECT A.*, project.project_name, ";
		strQuery += "project.sum_credit, project.credit_time, ";
		strQuery += "project.sud_reestr, project.application_date, ";
		strQuery += "project.company_id ";
		strQuery += "FROM (SELECT project1_id AS project_id ";
		strQuery += "FROM mark WHERE number = ? ";
		strQuery += "UNION ALL ";
		strQuery += "SELECT project2_id AS project_id ";
		strQuery += "FROM mark WHERE number = ?) A ";
		strQuery += "INNER JOIN project ON A.project_id = project.project_id ";
		strQuery += "GROUP BY A.project_id;";

		pstmt = con->prepareStatement(strQuery);
		pstmt->setInt(1, _number);
		pstmt->setInt(2, _number);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setProject(result->getInt(1), result->getString(2), result->getInt(3), result->getInt(4),
				result->getString(5), result->getString(6), result->getInt(7));
			tmpPr.push_back(tmp);
		}
		delete result;
		return tmpPr;
	}

	// Получение проектов из ранжа (итоги)
	std::vector<Project> getWeightVcMark(size_t _number) {
		std::vector<Project> tmpPr;
		Project tmp;
		std::string strQuery;

		// Запрос для вычисления весов по проектам в ранже

		strQuery = "SELECT A.project_id, project.project_name,project.sum_credit, project.credit_time, ";
		strQuery += "project.sud_reestr, project.application_date, project.company_id, ";
		strQuery += "ROUND(SUM(A.mark)/(SELECT SUM(mark) FROM (";
		strQuery += "SELECT project1_id AS project_id, mark1 AS mark FROM mark WHERE number = ? ";
		strQuery += "UNION ALL ";
		strQuery += "SELECT project2_id AS project_id, mark2 AS mark FROM mark WHERE number = ?) B), 3) AS weight ";
		strQuery += "FROM (SELECT project1_id AS project_id, mark1 AS mark FROM mark WHERE number = ? ";
		strQuery += "UNION ALL ";
		strQuery += "SELECT project2_id AS project_id, mark2 AS mark FROM mark WHERE number = ?) A ";
		strQuery += "INNER JOIN project ON A.project_id = project.project_id ";
		strQuery += "GROUP BY A.project_id ";
		strQuery += "ORDER BY weight DESC;";

		pstmt = con->prepareStatement(strQuery);
		pstmt->setInt(1, _number);
		pstmt->setInt(2, _number);
		pstmt->setInt(3, _number);
		pstmt->setInt(4, _number);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setProject(result->getInt(1), result->getString(2), result->getInt(3), result->getInt(4),
				result->getString(5), result->getString(6), result->getInt(7));
			tmp.setWeight(result->getDouble(8));
			tmpPr.push_back(tmp);
		}
		delete result;
		return tmpPr;
	}

	// Получение сумм оценок по парам проектов
	std::vector<Mark> getVcTotalMarks(size_t _number /*, size_t _userId*/) {
		Mark tmp;
		std::vector<Mark> tmpMrk;
		std::string strQuery;

		strQuery += "SELECT project1_id, project2_id, ROUND(SUM(mark.mark1), 2) AS mark1, ROUND(SUM(mark.mark2), 2) AS mark2 ";
		strQuery += "FROM mark WHERE mark.number = ? ";
		strQuery += "GROUP BY mark.project1_id, project2_id";

		pstmt = con->prepareStatement(strQuery);
		pstmt->setInt(1, _number);
		result = pstmt->executeQuery();
		while (result->next()) {
			//setMark(size_t markId, size_t _number, size_t userId, size_t project1Id, size_t project2Id, float _value)
			tmp.setMark(0, 0, 0, result->getInt(1), result->getInt(2), result->getDouble(3)); //, result->getDouble(7)
			tmp.setValues(result->getDouble(3), result->getDouble(4));
			std::cout << tmp;
			tmpMrk.push_back(tmp);
		}
		delete result;
		return tmpMrk;
	}

	// Получение номеров ранжей
	std::vector<std::string> getNumbersMark() {
		vector<string> tmp;
		pstmt = con->prepareStatement("SELECT number, COUNT(number) * 2 AS count FROM mark GROUP BY number;");
		result = pstmt->executeQuery();
		while (result->next()) {
			//tmp.push_back(result->getString(1).c_str());
			tmp.emplace_back(result->getString(1).c_str());
		}
		delete result;
		return tmp;
	}

	size_t getNewNumber() {
		size_t tmp = 0;
		pstmt = con->prepareStatement("SELECT MAX(number) FROM mark;");
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp = result->getInt(1);
		}
		delete result;
		return tmp + 1;
	}

	size_t getCountExpert(size_t _number, size_t _uid) {
		size_t cnt = 0;
		pstmt = con->prepareStatement("SELECT COUNT(number) FROM mark WHERE number = ? AND user_id = ? GROUP BY number;");
		pstmt->setInt(1, _number);
		pstmt->setInt(2, _uid);
		result = pstmt->executeQuery();
		while (result->next()) {
			cnt = result->getInt(1);
		}
		delete result;
		return cnt;
	}


	// Удалить оценку по id
	void deleteMark(size_t m_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM mark WHERE mark_id = ?;");
			pstmt->setInt(1, m_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE mark : mark_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	// Удалить оценку по number и user_id
	void deleteMark(size_t _number, size_t _user_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM mark WHERE number = ? AND user_id = ?;");
			pstmt->setInt(1, _number);
			pstmt->setInt(2, _user_id);
			result = pstmt->executeQuery();
		}
		catch (const sql::SQLException& e) {
			cout << "Error {DELETE mark : number && user_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
};

#endif //DBWORK_H


/* Запрос проектов в ранже
SELECT
  A.*,
  project.project_name,
  project.sum_credit,
  project.credit_time,
  project.sud_reestr,
  project.application_date,
  project.company_id
FROM
  (
	SELECT
	  project1_id AS project_id
	FROM
	  mark
	WHERE
	  number = 3
	UNION ALL
	select
	  project2_id AS project_id
	FROM
	  mark
	WHERE
	  number = 3
  ) A
  INNER JOIN project ON A.project_id = project.project_id
GROUP BY
  A.project_id;
*/


/* Запрос суммирование
* SELECT mark.project1_id, mark.project2_id, ROUND(SUM(mark.mark1), 2), ROUND(SUM(mark.mark2), 2)
FROM mark WHERE mark.number = '3'
GROUP BY mark.project1_id, mark.project2_id
ORDER BY mark.project1_id ASC
*/


/* Запрос для вычисления весов по проектам в ранже
SELECT
  A.project_id,
  project.project_name,
  project.sum_credit,
  project.credit_time,
  project.sud_reestr,
  project.application_date,
  project.company_id,
  SUM(A.mark)/(
	SELECT
	  SUM(mark)
	FROM
	  (
		SELECT
		  project1_id AS project_id,
		  mark1 AS mark
		FROM
		  mark
		WHERE
		  number = 3
		UNION ALL
		select
		  project2_id AS project_id,
		  mark2 AS mark
		FROM
		  mark
		WHERE
		  number = 3
	  ) B
  ) as weight
FROM
  (
	SELECT
	  project1_id AS project_id,
	  mark1 AS mark
	FROM
	  mark
	WHERE
	  number = 3
	UNION ALL
	select
	  project2_id AS project_id,
	  mark2 AS mark
	FROM
	  mark
	WHERE
	  number = 3
  ) A
  INNER JOIN project ON A.project_id = project.project_id
GROUP BY
  A.project_id
ORDER BY
  weight DESC
*/