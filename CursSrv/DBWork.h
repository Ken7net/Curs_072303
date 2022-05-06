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

	vector<string> getGuide(const std::string& _table, size_t _numField) {
		vector<string> tmp;
		pstmt = con->prepareStatement("SELECT * FROM " + _table + ";");
		result = pstmt->executeQuery();
		while (result->next()) {
			//printf("%s\n", result->getString(_numField).c_str());
			tmp.push_back(result->getString(_numField).c_str());
		}
		delete result;
		return tmp;
	}

	//--------- User --------

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

	void addCompany(const Company& company) {
		try {
			pstmt = con->prepareStatement("INSERT INTO company (company_name, activity, finance) VALUES(?, ?, ?);");
			pstmt->setString(1, company.getName());
			pstmt->setString(2, company.getActivity());
			pstmt->setDouble(3, company.getFinance());
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {INSERT company} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	void editCompany(const Company& company, size_t c_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE company SET company_name = ?, activity = ? finance = ? where company_id = ?;");
			pstmt->setString(1, company.getName());
			pstmt->setString(2, company.getActivity());
			pstmt->setDouble(3, company.getFinance());
			pstmt->setInt(4, c_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {UPDATE company}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	Company getCompany(const std::string& field_name, const std::string& u_name) {
		Company tmp;
		pstmt = con->prepareStatement("SELECT * FROM company WHERE " + field_name + " = ?;");
		pstmt->setString(1, u_name);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setId(result->getInt(1));
			tmp.setCompany(result->getString(2), result->getString(3), result->getDouble(4));
		}
		delete result;
		return tmp;
	}

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

	void addProject(const Project& project) {
		try {
			pstmt = con->prepareStatement(
				"INSERT INTO project (project_name, sum_credit, sud_reest, application_date, company_id) VALUES(?, ?, ?, ?, ?);");
			pstmt->setString(1, project.getProjectName());
			pstmt->setInt(2, project.getSumCredit());
			pstmt->setString(3, project.getSudReestr());
			pstmt->setString(4, project.getApplicationDate().getDateStr());
			pstmt->setInt(5, project.getCompanyId());
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {INSERT project} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	void editProject(const Project& project, size_t p_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE project SET project_name = ?, sum_credit = ?, sud_reest = ?, application_date = ?, company_id = ? where project_id = ?;");
			pstmt->setString(1, project.getProjectName());
			pstmt->setInt(2, project.getSumCredit());
			pstmt->setString(3, project.getSudReestr());
			pstmt->setString(4, project.getApplicationDate().getDateStr());
			pstmt->setInt(5, project.getCompanyId());
			pstmt->setInt(6, p_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {UPDATE project}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

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

	void addMark(Mark mark) {
		try {
			pstmt = con->prepareStatement(
				"INSERT INTO mark (user_id, project1_id, project2_id, mark, mark2) VALUES(?, ?, ?, ?, ?);");
			pstmt->setInt(1, mark.getUserId());
			pstmt->setInt(2, mark.getProject1Id());
			pstmt->setInt(3, mark.getProject2Id());
			pstmt->setDouble(4, mark.getValue1());
			pstmt->setDouble(5, mark.getValue2());
			//pstmt->setInt(6, m_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {INSERT mark} message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	void editMark(Mark mark, size_t m_id) {
		try {
			pstmt = con->prepareStatement(
				"UPDATE mark SET user_id = ?, project1_id = ?, project2_id = ?, mark = ?, mark2 =? where mark_id = ?;");
			pstmt->setInt(1, mark.getUserId());
			pstmt->setInt(2, mark.getProject1Id());
			pstmt->setInt(3, mark.getProject2Id());
			pstmt->setDouble(4, mark.getValue1());
			pstmt->setDouble(5, mark.getValue2());
			pstmt->setInt(6, m_id);
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e) {
			cout << "Error {UPDATE mark}  message: " << e.what() << endl;
			system("pause");
			//exit(1);
		}
	}

	Mark getMark(const std::string& field_name, size_t m_id) {
		Mark tmp;
		pstmt = con->prepareStatement("SELECT * FROM mark WHERE " + field_name + " = ?;");
		pstmt->setInt(1, m_id);
		result = pstmt->executeQuery();
		while (result->next()) {
			tmp.setMark(result->getInt(1), result->getInt(2), result->getInt(3), result->getInt(4),
				result->getDouble(5));
		}
		delete result;
		return tmp;
	}

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