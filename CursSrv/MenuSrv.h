#ifndef CURSSRV_MENUSRV_H
#define CURSSRV_MENUSRV_H

#include "..\Utils\stdafx.h"
#include "User.h"
#include "Company.h"
#include "DBWork.h"
#include "Rating.h"

using namespace std;

extern size_t cntClients;

std::string strMenuMain = "-=-=-=-=  М е н ю  =-=-=-=-#Авторизация#Регистрация#Отключение#Выход";
std::string strMenuManager = "---= Менеджер =---#Добавление#Редактировать#Удаление#Сохранение информации#Поиск#Просмотр инвестиционных проектов#Сортировка инвестиционных проектов#Ранжировать инвестиционные проекты#Выход";
std::string strMenuManagerAdd = "Вы хотите добавить: #Пользователя#Компанию#Проект#Ранжирование#Назад";
std::string strMenuManagerEdit = "Вы хотите редактировать данные:#Пользователя#Компании#Проекта#Назад";
std::string strMenuManagerDel = "Вы хотите удалить: #Пользователя#Компанию#Проект#Назад";
std::string strMenuManagerSave = "Вы хотите сохранить информацию о:#Пользователях#Компаниях#Проектах#Назад";
std::string strMenuManagerSearch = "Вы хотите найти информацию о:#Пользователях#Компаниях#Проектах#Назад";
std::string strMenuManagerSort = "Вы хотите сортировать инвестиционные проекты по:#Id#Наименованию#Сумме кредита#Сроку#Назад";
std::string strMenuManagerRanking = "Ранжировать инвестиционные проекты:#Осуществить попарное сравнение проектов#Найти оценки#Вычислить веса проектов#Вывести результат ранжирования ИП#Назад";
std::string strMenuCompany = "-----= Консультант =-----#Ввод данных#Редактировать данные#Сохранение информации#Удаление данных#Выход";
std::string strMenuCompanyAdd = "Вы хотите добавить данные:#Компании#Проектов#Назад";
std::string strMenuCompanyEdit = "Вы хотите редактировать данные:#Компании#Проектов#О себе#Назад";
std::string strMenuCompanySave = "Вы хотите сохранить информацию о:#Компании#Проектах#Назад";
std::string strMenuCompanyDel = "Вы хотите удалить информацию о:#Компании#Проекте#Назад";
std::string strMenuExpert = "-----= Эксперт =-----#Выставление оценок#Редактировать данные о себе#Просмотреть информацию об инвестиционных проектах#Выход";
std::string strMenuEditUserSelf = "-= Редактирование пользователя#Фамилия Имя#Логин#Пароль";

class A_menu {
private:
	SOCKET sock;
	DBWork db;
	UserSock curUser;

public:

	// Конструктор
	A_menu() {
		sock = NULL;
	};

	explicit A_menu(SOCKET connection) {
		sock = connection;
		curUser.setSock(sock);
		// Зашифрованные имя_пользователя, пароль и имя_базы_данных
		//db.connect("-mysql.services.clever-cloud.com:3306", "xgbhnnb`id{ty<~`", "Ue;LioBjUwN4]:L|T5G", "o>jc`fzotxojunu88n");
		db.connect("tcp://127.0.0.1:3306", "myuser", "mypass", "curs");
	}

	~A_menu() = default;

	//----- Ввод нового пользователя -----
	void addUser(int flag = 2, bool cUser = false) {
		//UserSock user(sock, db.getRoles());
		UserSock user(sock, db.getGuide("user_role", 1, flag)); //, 2); // для вывода кроме админа (регистрация)
		user.enterUser(db.getGuide("user", 3));
		std::cout << user;
		db.addUser(user);
		std::cout << "---- Добавление пользователя ----" << std::endl;
		std::cout << user;
		//db.deleteUser(user.getUid());
		std::cout << "---------------------------------" << std::endl;
		if (cUser) {
			curUser.setUser(db.getUser("login", user.getLogin()));
		}
	}

	//-------- Список пользователей ---------
	int listUsers(vector<std::string>& vc, int flag = 0) {	// 0 - Все
		if (!vc.empty()) vc.clear();						// 1 - Эксперты
		vc = db.getGuide("user", 2, flag);// 2 - Не Менеджеры
		sendString(sock, "menu0");
		sendString(sock, toString(vc, "Выберите пользователя: "));
		return takeInt(sock);
	}

	//----- Редактирование пользователя -----
	void editUser() {
		vector<std::string> users;
		User oldUser;
		int ch = listUsers(users, 0);
		if (ch != 0) oldUser = db.getUser("user_name", users[ch - 1]);
		else {
			std::cout << "- Редактирование пользователя отменено -" << std::endl;
			return;
		}
		UserSock user(sock, db.getGuide("user_role", 1));
		user.enterUser(db.getGuide("user", 3));
		user.setUid(oldUser.getUid());
		db.editUser(user, oldUser.getUid());
		std::cout << "-- Редактирование пользователя --" << std::endl;
		std::cout << oldUser;
		std::cout << "---------------------------------" << std::endl;
		std::cout << user;
		std::cout << "---------------------------------" << std::endl;
	}

	// Удаление текущего логина из списков логинов
	static std::vector<std::string> delCurrentLogin(std::vector<string> vc, const std::string& curLogin) {
		std::vector<std::string> tmp;
		// Фильтруем элементы, перемещая уцелевшие элементы в начало массива
		auto newEnd = std::remove_if(vc.begin(), vc.end(), [curLogin](const std::string& str) {
			return str == curLogin;
			});
		// Удаляем все элементы, следующие после
		vc.erase(newEnd, vc.end());
		tmp = std::move(vc);
		return tmp;
	}

	//-- Редактирование пользователя собой --
	void editUserSelf() {

		sendString(sock, "menu" + std::to_string(split(strMenuEditUserSelf).size()));
		sendString(sock, strMenuEditUserSelf);

		/*sendString(sock, "menu" + std::to_string(mode));
		sendString(sock, toString(tmp, "Выберите проект: "));*/
		UserSock oldUser = curUser;
		size_t ch = takeInt(sock);
		if (ch > 0) {
			std::cout << ch << std::endl;
			//UserSock user(sock, db.getGuide("user_role", 1));
			//user.enterUser();
			//user.setUid(oldUser.getUid());
			curUser.editUserSock(delCurrentLogin(db.getGuide("user", 3), oldUser.getLogin()), ch);
			db.editUser(curUser, oldUser.getUid());
			std::cout << "-- Редактирование пользователя --" << std::endl;
			std::cout << oldUser;
			std::cout << "---------------------------------" << std::endl;
			std::cout << curUser;
			std::cout << "---------------------------------" << std::endl;
		}
		else {
			std::cout << "- Редактирование пользователя отменено -" << std::endl;
			return;
		}
	}

	//----- Удаление пользователя -----
	void deleteUser() {
		vector<std::string> users;
		User oldUser;
		int ch = listUsers(users, 0);
		if (ch != 0) oldUser = db.getUser("user_name", users[ch - 1]);
		else {
			std::cout << "- Удаление пользователя отменено -" << std::endl;
			return;
		}
		std::cout << "----- Удаление пользователя -----" << std::endl;
		std::cout << oldUser;
		db.deleteUser(oldUser.getUid());
		std::cout << "---------------------------------" << std::endl;
	}

	//----- Вывод пользователей -----
	void printUsers(std::vector<User> users, const std::string& toFile = "") const {
		sendString(sock, "output" + toFile);
		for (auto& it : users) {
			it.printUser(it == users[0]);
			//UserSock::printUserSock(sock, it, "", it == users[0]);
			UserSock::printUserSock(sock, it, it == users[0]);
		}
		sendString(sock, "end");
	}

	//--------- Поиск пользователей ---------
	void searchUsers(const std::string& toFile = "") {
		sendString(sock, "data");
		sendString(sock, "Введите Фамилию (или ее часть) для поиска: ");
		std::string fndUser = takeString(sock);
		std::cout << "Ищем пользователей, ФамилияИмя которых содержит - " << fndUser << std::endl;
		sendString(sock, "end");
		std::vector<User> tmp = db.getUsers(fndUser);
		if (!tmp.empty()) printUsers(tmp);
		else {
			sendString(sock, "output");
			sendString(sock, "Ничего не нашли! :(");
			std::cout << "Ничего не нашли! :(" << std::endl;
			sendString(sock, "end");
		}
	}

	//--------- Ввод новой компании ---------
	void addCompany() {
		CompanySock cmp(sock);
		cmp.enterCompany();
		db.addCompany(cmp);
		std::cout << "------ Добавление компании ------" << std::endl;
		std::cout << cmp;
		std::cout << "---------------------------------" << std::endl;
	}

	//----------- Список компаний -----------
	int listCompany(vector<string>& vc) {
		if (!vc.empty()) vc.clear();
		vc = db.getGuide("company", 2);
		sendString(sock, "menu0");
		sendString(sock, toString(vc, "Выберите компанию: "));
		return takeInt(sock);
	}

	//------- Редактирование компании -------
	void editCompany() {
		vector<std::string> companies;
		Company oldCompany;
		int ch = listCompany(companies);
		if (ch != 0) oldCompany = db.getCompany("company_name", companies[ch - 1]);
		else {
			std::cout << "-- Редактирование компании отменено --" << std::endl;
			return;
		}
		std::cout << "------- Редактирование компании -------" << std::endl;
		std::cout << oldCompany;
		std::cout << "---------------------------------" << std::endl;
		CompanySock newCompany(sock);
		newCompany.enterCompany();
		newCompany.setId(oldCompany.getId());
		db.editCompany(newCompany, oldCompany.getId());
		std::cout << newCompany;
		std::cout << "---------------------------------" << std::endl;
	}

	//---------- Удаление компании ----------
	void deleteCompany() {
		vector<std::string> companies;
		Company oldCompany;
		int ch = listCompany(companies);
		if (ch != 0) oldCompany = db.getCompany("company_name", companies[ch - 1]);
		else {
			std::cout << "--- Удаление компании отменено ---" << std::endl;
			return;
		}
		std::cout << "------- Удаление компании -------" << std::endl;
		std::cout << oldCompany;
		db.deleteCompany(oldCompany.getId());
		std::cout << "---------------------------------" << std::endl;
	}

	//------------ Вывод компаний -----------
	void printCompanies(std::vector<Company> vcCmps, const std::string& fout = "") const {
		sendString(sock, "output" + fout);
		for (const auto& it : vcCmps) {
			it.printCompany(it == vcCmps[0]);
			it.printCompanySock(sock, it == vcCmps[0]);
		}
		sendString(sock, "end");
	}

	//------------ Поиск компаний -----------
	void searchCompanies() {
		sendString(sock, "data");
		sendString(sock, "Введите Наименование (или его часть) для поиска: ");
		std::string fndCompany = takeString(sock);
		std::cout << "Ищем компании, Наименование которых содержит - " << fndCompany << std::endl;
		sendString(sock, "end");
		std::vector<Company> tmp = db.getCompanies(fndCompany);
		if (!tmp.empty()) printCompanies(tmp);
		else {
			sendString(sock, "output");
			sendString(sock, "Ничего не нашли! :(");
			std::cout << "Ничего не нашли! :(" << std::endl;
			sendString(sock, "end");
		}
	}

	//------------ Вывод проектов -----------
	void printProjects(std::vector<Project> vcPrs, const std::string& fout = "") {
		sendString(sock, "output" + fout);
		for (auto& it : vcPrs) {
			it.printProject(db.getCompany("company_id", std::to_string(it.getCompanyId())).getName(), it == vcPrs[0]);
			it.printProjectSock(sock, db.getCompany("company_id", std::to_string(it.getCompanyId())).getName(), it == vcPrs[0]);
		}
		sendString(sock, "end");
	}

	//------------ Поиск проектов -----------
	void searchProjects() {
		sendString(sock, "data");
		sendString(sock, "Введите Наименование (или его часть) для поиска: ");
		std::string fndProjects = takeString(sock);
		std::cout << "Ищем проекты, Наименование которых содержит - " << fndProjects << std::endl;
		sendString(sock, "end");
		std::vector<Project> tmp = db.getProjects(fndProjects);
		if (!tmp.empty()) printProjects(tmp);
		else {
			sendString(sock, "output");
			sendString(sock, "Ничего не нашли! :(");
			std::cout << "Ничего не нашли! :(" << std::endl;
			sendString(sock, "end");
		}
	}

	//--------- Ввод нового проекта ---------
	void addProject() {
		ProjectSock project(sock, db.getGuide("company", 2));
		project.enterProject();
		project.setCompanyId(db.getCompany("company_name", project.Companies[project.getCompanyId()]).getId());
		std::cout << project;
		db.addProject(project);
		std::cout << "--------- Ввод нового проекта ---------" << std::endl;
		std::cout << project;
		std::cout << "---------------------------------------" << std::endl;
	}

	//------- Список проектов vector --------
	size_t listProject(vector<string>& vc, size_t mode = 0) {
		if (!vc.empty()) vc.clear();
		vc = db.getGuide("project", 2);
		sendString(sock, "menu" + std::to_string(mode));
		sendString(sock, toString(vc, "Выберите проект: "));
		return takeInt(sock);
	}

	//--------- Список проектов map ---------
	size_t listProject(map<string, size_t>& vc, size_t mode = 0) {
		size_t ch = 0;
		if (!vc.empty()) vc.clear();
		vc = db.getGuideMap("project", 2);
		vector<string> tmp = toVector(vc);
		sendString(sock, "menu" + std::to_string(mode));
		sendString(sock, toString(tmp, "Выберите проект: "));
		ch = takeInt(sock);
		if (ch != 0 && ch != tmp.size() + 1) {
			////ch = vc[tmp[ch]];
			//ch = ch - 1;
			std::cout << ch << std::endl;
			//std::cout << tmp[ch] << " " << tmp[ch - 1] << " " << vc[tmp[ch - 1]] << endl;
			std::cout << " " << tmp[ch - 1] << " " << vc[tmp[ch - 1]] << endl;
			ch = vc[tmp[ch - 1]];
		}
		return ch;
	}

	//-------- Редактирование проекта--------
	void editProject() {
		vector<std::string> projects;
		Project oldProject;
		size_t ch = listProject(projects);
		if (ch != 0)
			oldProject = db.getProject("project_name", projects[ch - 1]);
		else {
			std::cout << "--- Редактирование проекта отменено ---" << std::endl;
			return;
		}
		ProjectSock newProject(sock, db.getGuide("company", 2));
		std::cout << "-------- Редактирование проекта--------" << std::endl;
		std::cout << oldProject;
		std::cout << "---------------------------------------" << std::endl;
		newProject.enterProject();
		newProject.setProjectId(oldProject.getProjectId());
		newProject.setCompanyId(db.getCompany("company_name", newProject.Companies[newProject.getCompanyId()]).getId());
		db.editProject(newProject, oldProject.getProjectId());
		std::cout << newProject;
		std::cout << "---------------------------------------" << std::endl;
	}

	//----------- Удаление проекта-----------
	void deleteProject() {
		vector<std::string> projects;
		Project oldProject;
		size_t ch = listProject(projects);
		if (ch != 0) oldProject = db.getProject("project_name", projects[ch - 1]);
		else {
			std::cout << "------ Удаление проекта отменено ------" << std::endl;
			return;
		}
		std::cout << "----------- Удаление проекта-----------" << std::endl;
		std::cout << oldProject;
		std::cout << "---------------------------------------" << std::endl;
		db.deleteProject(oldProject.getProjectId());
	}

	//---------- Ввод новой оценки ----------
	void addMark() {
		MarkSock mark(sock);
		mark.enterMarkAll(db.getGuideMap("user", 2, 1), db.getGuideMap("project", 2));
		//mark.setCompanyId(db.getCompany("company_name", project.Companies[project.getCompanyId()]).getId());
		std::cout << mark;
		db.addMark(mark);
		std::cout << "---------- Ввод новой оценки ----------" << std::endl;
		std::cout << mark;
		std::cout << "---------------------------------------" << std::endl;
	}

	// Выбор номера ранжа
	//--------- Список оценок map ---------
	size_t listNumberMark(/*map<string, size_t>& vc*/) {
		int ch = 0;
		/*if (!vc.empty()) vc.clear();
		vc = db.getGuideMap("mark", 2);
		vector<string> tmp = toVector(vc);*/
		vector<string> tmp = db.getNumbersMark();
		sendString(sock, "menu151");
		sendString(sock, toString(tmp, "Выберите номер оценивания: "));
		ch = takeInt(sock);
		if (ch != 0) {
			////ch = vc[tmp[ch]];
			//std::cout << tmp[ch] << " " << tmp[ch - 1] << " " << vc[tmp[ch - 1]] << endl;
			//ch = vc[tmp[ch - 1]];
		}
		return ch;
	}

	//--------- Список оценок map ---------
	size_t listMark(map<string, size_t>& vc) {
		size_t ch = 0;
		if (!vc.empty()) vc.clear();
		vc = db.getGuideMap("mark", 2);
		vector<string> tmp = toVector(vc);
		sendString(sock, "menu0");
		sendString(sock, toString(tmp, "Выберите оценку: "));
		ch = takeInt(sock);
		if (ch != 0) {
			//ch = vc[tmp[ch]];
			std::cout << tmp[ch] << " " << tmp[ch - 1] << " " << vc[tmp[ch - 1]] << endl;
			ch = vc[tmp[ch - 1]];
		}
		return ch;
	}

	//-------- Редактирование оценки --------
	void editMark() {
		std::map<std::string, size_t> marks;
		Mark oldMark;
		size_t ch = listMark(marks);
		if (ch != 0) oldMark = db.getMark("mark_id", ch);
		else {
			std::cout << "---- Редактирование оценки отменено ---" << std::endl;
			return;
		}
		std::cout << "-------- Редактирование оценки --------" << std::endl;
		std::cout << oldMark;
		std::cout << "---------------------------------------" << std::endl;
		MarkSock newMark(sock);
		//newMark.enterMark(db.getGuideMap("user", 2, 1), db.getGuideMap("project", 2));
		newMark.enterMark(oldMark.getNumber(), oldMark.getUserId(), oldMark.getProject1Id(), oldMark.getProject2Id());
		newMark.setMarkId(oldMark.getMarkId());
		//newProject.setCompanyId(db.getCompany("company_name", newProject.Companies[newProject.getCompanyId()]).getId());
		//db.editProject(newProject, oldProject.getProjectId());
		db.editMark(newMark, newMark.getMarkId());
		std::cout << newMark;
		std::cout << "---------------------------------------" << std::endl;
	}

	//----------- Удаление оценки -----------
	void deleteMark() {
		std::map<std::string, size_t> marks;
		Mark oldMark;
		size_t ch = listMark(marks);
		if (ch != 0) oldMark = db.getMark("mark_id", ch);
		else {
			std::cout << "------ Удаление оценки отменено -------" << std::endl;
			return;
		}
		std::cout << "----------- Удаление оценки -----------" << std::endl;
		std::cout << oldMark;
		std::cout << "---------------------------------------" << std::endl;
		db.deleteMark(oldMark.getMarkId());
	}

	// Меню Менеджера
	void menuManager() {
		/*
		* 1. Добавление
		* 2. Редактировать
		* 3. Удаление
		* 4. Сохранение информации в бд
		* 5. Поиск#
		* 6. Просмотр инвестиционных проектов#
		* 7. Сортировка инвестиционных проектов#
		* 8. Ранжировать инвестиционные проекты#
		* 9. Выход"
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManager);
		//sendMenu(sock, split(strMenuManager));
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			size_t i = atoi(p);
			std::cout << "<- " << split(strMenuManager)[i] << endl;
			switch (i) {
			case 1:
				// Добавление
				menuManagerAdd();
				break;
			case 2:
				// Редактировать
				menuManagerEdit();
				break;
			case 3:
				// Удаление
				menuManagerDel();
				break;
			case 4:
				// Сохранение
				menuManagerSave();
				break;
			case 5:
				// Поиск
				menuManagerSearch();
				break;
			case 6:
				// Просмотр Инвестиционных проектов
				//// вывод проектов в консоль сервера
				printProjects(db.getProjects());
				//printProjectsSock();
				//printProjectsSock("file");
				break;
			case 7:
				// Сортировка Инвестиционных проектов
				menuManagerSort();
				break;
			case 8:
				// Ранжировать Инвестиционные проекты
				menuManagerRanking();
				break;
			case 9:
				//sendString(sock, "menu");
				//sendString(sock, strMenuMain);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManager);
		}
	}

	// Создать новое ранжирование
	void addRanking() {
		Rating rt(sock);
		// Выбор экспертов
		rt.selectExperts(db.getGuideMap("user", 2, 1));
		// Выбор проектов
		rt.selectProjects(db.getProjectMp());
		// Ввод оценок
		rt.setNumber(db.getNewNumber());
		rt.enterRanks();
		for (auto& it : rt.ranking) {
			for (auto& iit : it.second) {
				db.addMark(iit);
			}
		}
	}

	// Меню Менеджера. Добавление
	void menuManagerAdd() {
		/*
		* 1. Новых пользователей
		* 2. Новые компании
		* 3. Новые проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerAdd);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerAdd)[i] << endl;
			switch (i) {
			case 1:
				// Новые пользователи
				addUser(0);
				break;
			case 2:
				// Новые компании
				addCompany();
				break;
			case 3:
				// Новые проекты
				addProject();
				break;
			case 4:
				// Новое ранжирование
				addRanking();
				break;
			case 5:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerAdd);
		}
	}

	// Меню Менеджера. Редактирование
	void menuManagerEdit() {
		/*
		* 1. Пользователей
		* 2. Компанию
		* 3. Проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerEdit);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerEdit)[i] << endl;
			switch (i) {
			case 1:
				//
				editUser();
				break;
			case 2:
				//
				editCompany();
				break;
			case 3:
				//
				editProject();
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerEdit);
		}
	}

	// Меню Менеджера. Удаление
	void menuManagerDel() {
		/*
		* 1. Пользователей
		* 2. Компанию
		* 3. Проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerDel);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerDel)[i] << endl;
			switch (i) {
			case 1:
				deleteUser();
				break;
			case 2:
				deleteCompany();
				break;
			case 3:
				deleteProject();
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerDel);
		}
	}

	// Меню Менеджера. Сохранение
	void menuManagerSave() {
		/*
		* 1. Пользователях#
		* 2. Компаниях#
		* 3. Проектах#
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerSave);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerSave)[i] << endl;
			switch (i) {
			case 1:
				// вывод пользователей в консоль сервера и в файл на клиенте
				printUsers(db.getUsers(), "file#users");
				break;
			case 2:
				// вывод компаний в консоль сервера и в файл на клиенте
				printCompanies(db.getCompanies(), "file#companies");
				break;
			case 3:
				// вывод проектов в консоль сервера и в файл на клиенте
				printProjects(db.getProjects(), "file#projects");
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerSave);
		}
	}

	// Меню Менеджера. Сохранение
	void menuManagerSort() {
		/*
		* 1. Id
		* 2. Наименование
		* 3. Сумма кредита
		* 4. Срок
		* 5. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerSort);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerSort)[i] << endl;
			std::string strOrder;
			switch (i) {
			case 1:
				// сортирова
				strOrder = "project_id";
				break;
			case 2:
				// вывод компаний в консоль сервера и в файл на клиенте
				strOrder = "project_name";
				break;
			case 3:
				// вывод проектов в консоль сервера и в файл на клиенте
				strOrder = "sum_credit";
				break;
			case 4:
				strOrder = "credit_time";
				break;
			case 5:
				return;
			default:
				break;
			}
			printProjects(db.getProjects("", strOrder));
			sendString(sock, "menu");
			sendString(sock, strMenuManagerSort);
		}
	}

	// Меню Менеджера. Поиск
	void menuManagerSearch() {
		/*
		* 1. Пользователях#
		* 2. Компаниях#
		* 3. Проектах#
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerSearch);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerSearch)[i] << endl;
			switch (i) {
			case 1:
				// поиск и вывод пользователей в консоль сервера и в файл на клиенте
				//printUsers(db.getUsers("ил"), "file#users");
				searchUsers();
				break;
			case 2:
				// поиск и вывод компаний в консоль сервера и в файл на клиенте
				//printCompanies(db.getUsers("ил"), "file#companies");
				searchCompanies();
				break;
			case 3:
				// вывод проектов в консоль сервера и в файл на клиенте
				//printProjects(db.getProjects(), "file#projects");
				searchProjects();
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerSearch);
		}
	}

	// Ранжировать инвестиционные проекты
	void menuManagerRanking() {
		/*
		* 1. Найти оценки#
		* 2. Осуществить попарное сравнение проектов#
		* 3. Вычислить веса проектов#
		* 4. Вывести результат ранжирования ИП#
		* 5. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuManagerRanking);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		Rating rating(sock);
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuManagerRanking)[i] << endl;
			switch (i) {
			case 1:
				// Найти оценки

				rating.selectNumber(db.getNumbersMark(), "2");

				// Загрузка оценок
				rating.ranking = db.getMpMarks(rating.getNumber());

				// Загрузка экспертов в ранже
				rating.mpExperts = db.getExpertMapMark(rating.getNumber());
				rating.setCntExperts();

				// Загрузка проектов в ранже
				rating.vcProjects = db.getProjectVcMark(rating.getNumber());
				rating.setCntProjects();

				break;
			case 2:
				// Осуществить попарное сравнение проектов

				// Расчет сумм по парным сравнениям
				rating.rankingTotal = db.getVcTotalMarks(rating.getNumber());

				reCreateVcRankTotal(rating);

				// Вывод таблицы ранжирования
				rating.printRatingTable();
				rating.printRatingTableSock();
				rating.printRatingTableSock("file");
				break;
			case 3:
				// Вычислить веса проектов

				// Расчет весов
				rating.vcProjects = db.getWeightVcMark(rating.getNumber());

				break;
			case 4:
				// Вывести результат ранжирования ИП

				// Вывод таблицы ранжирования
				rating.printRatingTable();
				rating.printRatingTableSock();
				rating.printRatingTableSock("file");
				// Вывод результата ранжирования
				rating.printRating();
				rating.printRatingSock();
				rating.printRatingSock("file");
				break;
			case 5:
				//sendString(sock, "menu");
				//sendString(sock, strMenuManager);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuManagerRanking);
		}
	}

	// Меню Компания
	void menuCompany() {
		/*
		* 1. Ввод данных#
		* 2. Редактировать данные#
		* 3. Сохранение информации в бд#
		* 4. Удаление данных#
		* 6. Выход
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuCompany);
		//sendMenu(sock, split(strMenuManager));
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		int c = -1;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuCompany)[i] << endl;
			switch (i) {
			case 1:
				// Ввод данных
				menuCompanyAdd();
				break;
			case 2:
				// Редактировать данные
				menuCompanyEdit();
				break;
			case 3:
				// Сохранение информации в бд
				menuCompanySave();
				break;
			case 4:
				// Удаление данных о проекте
				menuCompanyDel();
				break;
			case 5:
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompany);
		}
	}

	// Меню Компания. Добавить
	void menuCompanyAdd() {
		/*
		* 1. Компании#
		* 2. Проектов#
		* 3. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuCompanyAdd);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuCompanyEdit)[i] << endl;
			switch (i) {
			case 1:
				//
				addCompany();
				break;
			case 2:
				//
				addProject();
				break;
			case 3:
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanyAdd);
		}
	}

	// Меню Компания. Редактировать
	void menuCompanyEdit() {
		/*
		* 1. Компании#
		* 2. Проектов#
		* 3. О себе#
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuCompanyEdit);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuCompanyEdit)[i] << endl;
			switch (i) {
			case 1:
				// Редактировать компанию
				editCompany();
				break;
			case 2:
				// Редактировать проект
				editProject();
				break;
			case 3:
				// Редактировать информацию о себе
				editUserSelf();
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanyEdit);
		}
	}

	// Меню Компания. Сохранить
	void menuCompanySave() {
		/*
		* 1. Компании#
		* 2. Проектов#
		* 3. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuCompanySave);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuCompanySave)[i] << endl;
			switch (i) {
			case 1:
				// вывод компаний в консоль сервера и в файл на клиенте
				printCompanies(db.getCompanies(), "file#companies");
				//printCompaniesSock();
				//printCompaniesSock("file#companies");
				break;
			case 2:
				// вывод проектов в консоль сервера и в файл на клиенте
				printProjects(db.getProjects(), "file#projects");
				//printProjectsSock();
				//printProjectsSock("file#projects");
				break;
			case 3:
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanySave);
		}
	}

	// Меню Компания. Удалить
	void menuCompanyDel() {
		/*
		* 1. Компании#
		* 2. Проектов#
		* 3. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuCompanyDel);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuCompanyDel)[i] << endl;
			switch (i) {
			case 1:
				// Удалить компанию
				deleteCompany();
				break;
			case 2:
				// Удалить проект
				deleteProject();
				break;
			case 3:
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanyDel);
		}
	}

	void reCreateVcProjects(Rating& rt) {
		if (!rt.vcProjects.empty()) rt.vcProjects.clear();
		auto itt = rt.ranking.begin();
		size_t tmpPrId = (*itt).second[0].getProject1Id();
		Project tmpPr = db.getProject("project_id", std::to_string(tmpPrId));
		rt.vcProjects.push_back(tmpPr);
		for (const auto& it : itt->second) {
			if (tmpPrId == it.getProject1Id()) {
				tmpPr = db.getProject("project_id", std::to_string(it.getProject2Id()));
				rt.addProject(tmpPr);
			}
			else break;
		}
	}

	static std::pair<float, float> getORT(const std::vector<Mark>& mrk, size_t pr1Id, size_t pr2Id) {
		for (const Mark& it : mrk) {
			if (pr1Id == it.getProject1Id() && pr2Id == it.getProject2Id())
				return make_pair(it.getValue1(), it.getValue2());
		}
		return make_pair((float)0, (float)0);
	}

	static void reCreateVcRankTotal(Rating& rt) {
		std::vector<Mark> tmpMrks;
		Mark tmpMrk;
		auto itt = rt.ranking.begin();
		for (const auto& it : itt->second) {
			tmpMrk.setValues(getORT(rt.rankingTotal, it.getProject1Id(), it.getProject2Id()));
			tmpMrks.push_back(tmpMrk);
		}
		rt.rankingTotal = tmpMrks;
	}

	// Меню Эксперт. Выставление оценок
	void menuExpertMarking() {
		Rating rating(sock);
		rating.clear();
		rating.selectNumber(db.getNumbersMark(), "303");
		if (rating.getNumber() == db.getNewNumber()) {
			// Новый Ранж
			rating.selectProjects(db.getProjectMp());
			rating.addExpert(curUser.getName(), curUser.getUid());
			rating.enterRank(make_pair(curUser.getName(), curUser.getUid()), 1);
			db.addMarkVc(rating.ranking[curUser.getUid()]);
		}
		else {
			rating.ranking.clear();
			rating.ranking = db.getMpMarks(rating.getNumber());
			reCreateVcProjects(rating);
			rating.ranking = db.getMpMarks(rating.getNumber(), curUser.getUid());
			if (rating.ranking.empty()) {
				//rating.ranking.insert(make_pair(curUser.getUid(), db.getVcTotalMarks(rating.getNumber())));
				rating.enterRank(make_pair(curUser.getName(), curUser.getUid()), 1);
			}
			else rating.editRank(make_pair(curUser.getName(), curUser.getUid()), 1);
			db.addMarkVc(rating.ranking[curUser.getUid()]);
		}
	}

	// Эксперт
	void menuExpert() {
		/*
		* 1. Выставление оценок#
		* 2. Редактировать данные о себе#
		* 3. Просмотреть информацию об инвестиционных проектах#
		* 4. Выход
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuExpert);
		//sendMenu(sock, split(strMenuExpert));
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		Rating rating(sock);
		int c = -1;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuExpert)[i] << endl;
			switch (i) {
			case 1:
				// Выставление оценок
				menuExpertMarking();
				break;
			case 2:
				// Редактировать информацию о себе
				if (!curUser.isEmptyId()) editUserSelf();
				break;
			case 3:
				// Информация о инвестиционных проектах
				//// вывод проектов в консоль сервера
				printProjects(db.getProjects());
				//printProjectsSock();
				//printProjectsSock("file");
				break;
			case 4:
				return;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuExpert);
		}
	}

	// Авторизация
	void loginUser() {
		User oldUser;
		tUser userC;
		sendString(sock, "data");
		sendString(sock, "Логин: ");
		userC.login = takeString(sock);
		// ищем пользователя в базе
		oldUser = db.getUser("login", userC.login);
		// если находим
		if (!oldUser.isEmptyId()) {
			sendString(sock, "Пароль: ");
			userC.pass = takeString(sock);
			sendString(sock, "end");
			//if (userC.pass == encryptChars(oldUser.getPass())) {
			if (userC.pass == oldUser.getPass()) {
				//if (userC.pass == oldUser.getPass()) {
				userC.Role = oldUser.getRole();
				curUser.setUser(oldUser);
				std::cout << "login: " << curUser.getLogin() << " - " << curUser.getPass() << " - " << curUser.getRole() << endl;
			}
		}
		else {
			sendString(sock, "end");
			sendString(sock, "output");
			sendString(sock, "Ошибка! Нет такого пользователя!!!\n");
			sendString(sock, "end");
		}
	}

	// Запуск меню в зависимости от роли
	void userStart() {
		if (!curUser.isEmptyId()) {
			if (curUser.getRole() == "Менеджер") menuManager();
			else if (curUser.getRole() == "Консультант банка") menuCompany();
			else menuExpert();
		}
	}

	void cryptPasses() {
		for (auto& it : db.getUsers()) {
			it.setPass(encryptChars(it.getPass()));
			db.editUser(it, it.getUid());
		}
	}

	void start() {
		int c;
		char p[200]{}; //, com[200];//основной буфер и команда
		//char curU[20];
		//curU[0] = '\0';
		//com[0] = '\0';
		p[0] = '\0';
		std::cout << "Соединение установлено." << std::endl;
		strcat(p, "Server connected...\n");
		Date tmpDate{};
		tmpDate.setDateStr(Date::currentDate());
		sendString(sock, "Server connected...\n" + tmpDate.getDateStr() + "\n");
		sendString(sock, "menu" + std::to_string(1));
		sendString(sock, strMenuMain);
		curUser.setSock(sock);
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			command = p;
			int i = atoi(command.c_str());
			curUser.clear();
			switch (i) {
			case 1:
				//Подключение пользователя
				loginUser();
				userStart();

				//menuManager();
				//menuCompany();
				//menuExpert();
				break;
			case 2:
				//подключение пользователя
				curUser.clear();
				addUser(2, true);
				userStart();

				//menuManager();
				//menuCompany();
				//menuExpert();
				break;
			case 3:
				//отключение пользователя
				//cout << "User " << curU << " logout" << endl;

				//cryptPasses();
				menuManager();
				//menuCompany();
				//menuExpert();

				//cntClients--;
				//std::cout << "Клиент " << curUser.getLogin() << "отключен.\nТекущее количество подключений: " << cntClients << std::endl;
				//sendString(sock, "exit");
				//closesocket(sock);//закрываем сокет
				//return;


				//break;
			case 4:
				//выход
				cntClients--;
				std::cout << "Клиент отключен. Текущее количество подключений: " << cntClients << std::endl;
				sendString(sock, "exit");
				closesocket(sock);//закрываем сокет
				exit(EXIT_SUCCESS);
                //return;
				break;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuMain);
		}
	}
};

#endif //CURSSRV_MENUSRV_H