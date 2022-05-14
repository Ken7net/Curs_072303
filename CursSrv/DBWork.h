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
	sql::Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
public:

	DBWork() {}

	DBWork(std::string _server, std::string _username, std::string _password, std::string _database) {
		server = std::move(_server);
		username = std::move(_username);
		password = std::move(_password);
		database = std::move(_database);
		try {
			driver = get_driver_instance();
			//for demonstration only. never save password in the code!
			con = driver->connect(server, username, password);
		}
		catch (sql::SQLException e) {
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			system("pause");
			exit(1);
		}
		con->setSchema(database);

		pstmt = con->prepareStatement("SET NAMES cp1251;");
		pstmt->execute();
	}

	void connect(std::string _server, std::string _username, std::string _password, std::string _database) {
		server = std::move(_server);
		username = std::move(_username);
		password = std::move(_password);
		database = std::move(_database);
		try {
			driver = get_driver_instance();
			//for demonstration only. never save password in the code!
			//con = driver->connect(server.c_str(), username.c_str(), password.c_str());
            con = driver->connect(server, username, password);
		}
		catch (sql::SQLException e) {
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			system("pause");
			exit(1);
		}
		con->setSchema(database);

		pstmt = con->prepareStatement("SET NAMES cp1251;");
		pstmt->execute();
	}

	~DBWork() {
		if (result != nullptr) delete result;
		delete pstmt;
		delete con;
	}

	// Получение справочников
	// _table : таблица
	// _numField : номер поля
	// exp :   = 1 - для отбора экспертов (по умолчанию 0)
	//			 2 - для не Администратора
	vector<string> getGuide(const std::string& _table, size_t _numField, size_t exp = 0) {
		vector<string> tmp;
		std::string str = "";
		if (exp == 1) str = " WHERE role = 'Эксперт'";
		if (exp == 2) str = " WHERE NOT role = 'Администратор'";
		pstmt = con->prepareStatement("SELECT * FROM " + _table + str + ";");
		result = pstmt->executeQuery();
		while (result->next()) {
			//printf("%s\n", result->getString(_numField).c_str());
			tmp.push_back(result->getString(_numField).c_str());
		}
		delete result;
		return tmp;
	}

	std::map<string, size_t> getGuideMap(const std::string& _table, size_t _numField, size_t exp = 0) {
		std::map<std::string, size_t> tmp;
		std::string str = "";
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
			tmp.push_back(result->getString(1).c_str());
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
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
	// Удалить пользователя по id
	void deleteUser(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM user WHERE user_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
	// Удалить компанию по id
	void deleteCompany(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM company WHERE company_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
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

	// Удалить проект по id
	void deleteProject(size_t u_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM project WHERE project_id = ?;");
			pstmt->setInt(1, u_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
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
		catch (sql::SQLException e) {
			cout << "Error {DELETE project project_name} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	//------- Mark -------
	// Добавить оценку
	void addMark(Mark mark) {
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
		catch (sql::SQLException e) {
			cout << "Error {INSERT mark} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Редактировать оценку
	void editMark(Mark mark, size_t m_id) {
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
		catch (sql::SQLException e) {
			cout << "Error {UPDATE mark}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
	// Получить оценку
	Mark getMark(const std::string& field_name, size_t m_id) {
		Mark tmp;
		pstmt = con->prepareStatement("SELECT * FROM mark WHERE " + field_name + " = ?;");
		pstmt->setInt(1, m_id);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setMark(result->getInt(1), result->getInt(2), result->getInt(3), result->getInt(4), result->getInt(5),
				result->getDouble(6)); //, result->getDouble(7)
		}
		delete result;
		return tmp;
	}
	// Получить отражение оценок для набора и пользователя
	std::map<size_t, std::vector<Mark>> getMpMarks(size_t _number /*, size_t _userId*/) {
		Mark tmp;
		std::map<size_t, std::vector<Mark>> tmpRn;
		std::vector<Mark> tmpMrk;
		pstmt = con->prepareStatement("SELECT * FROM mark WHERE number = ? ORDER BY user_id, project1_id, project2_id;"); // AND user_id = ? ; ");
		pstmt->setInt(1, _number);
		//pstmt->setInt(2, _userId);
		size_t lastUser = -1;
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setMark(result->getInt(1), result->getInt(2), result->getInt(3), result->getInt(4), result->getInt(5),
				result->getDouble(6)); //, result->getDouble(7)
			if (lastUser != -1 && lastUser != tmp.getUserId()) {
				tmpRn.insert(make_pair(lastUser, tmpMrk));
				lastUser = tmp.getUserId();
				tmpMrk.clear();
			}
			tmpMrk.push_back(tmp);
		}
		delete result;
		return tmpRn;
	}

	// Удалить оценку по id
	void deleteMark(size_t m_id) {
		try {
			pstmt = con->prepareStatement("DELETE FROM mark WHERE mark_id = ?;");
			pstmt->setInt(1, m_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {DELETE mark mark_id}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}
};

#endif //DBWORK_H