#ifndef CURSCLN_MENUSRV_H
#define CURSCLN_MENUSRV_H

#include "..\Utils\stdafx.h"
#include "User.h"
#include "Company.h"
#include "DBWork.h"
#include "Rating.h"

using namespace std;


class A_menu {
private:
	SOCKET sock;
	DBWork db;
	tUser userC;
	vector<size_t> vcProjectOp;	//Вектор проектов для оценивания
	vector<double> vcMarksOp;	//Вектор оценок проектов
public:
	//static vector<std::string> vcMainMenu = {"Логин", "Регистрация", "Выход"};

	std::string strMenuMain = "-=-=-=-=  М е н ю  =-=-=-=-#Логин#Регистрация#Логаут#Выход";
	std::string strMenuAdmin = "\tАдминистратор#Добавление#Редактировать#Удаление#Сохранение информации в бд#Поиск#Просмотр инвестиционных проектов#Сортировка инвестиционных проектов#Ранжировать инвестиционные проекты#Выход";
	std::string strMenuAdminAdd = "Вы хотите добавить: #Новых пользователей#Новые компании#Новые проекты#Назад";
	std::string strMenuAdminEdit = "Вы хотите редактировать данные:#Пользователей#Компании#Проектов#Назад";
	std::string strMenuAdminDel = "Вы хотите удалить: #Пользователя#Компанию#Проект#Назад";
	std::string strMenuAdminSave = "Вы хотите сохранить информацию о:#Пользователях#Компаниях#Проектах#Назад";
	std::string strMenuAdminRanking = "Ранжировать инвестиционные проекты:#Осуществить попарное сравнение проектов#Найти оценки#Вычислить веса проектов#Составить порядок предпочтений#Вывести результат ранжирования ИП#Назад";
	std::string strMenuCompany = "\tКомпания#Ввод данных#Редактировать данные#Сохранение информации в бд#Удаление данныхВыход";
	std::string strMenuCompanyAdd = "Вы хотите добавить данные:#Компании#Проектов#Назад";
	std::string strMenuCompanyEdit = "Вы хотите редактировать данные:#Компании#Проектов#Назад";
	std::string strMenuCompanySave = "Вы хотите сохранить информацию о:#Компании#Проектах#Назад";
	std::string strMenuCompanyDel = "Вы хотите удалить информацию о:#Компании#Проекте#Назад";
	std::string strMenuExpert = "\tЭксперт#Выставление оценок#Редактировать данные о себе#Просмотреть информацию об инвестиционных проектах#Выход";

	// Конструктор
	A_menu() {
		sock = NULL;
	};

	explicit A_menu(SOCKET connection) {
		sock = connection;
		// Зашифрованные имя_пользователя, пароль и имя_базы_данных
		//db.connect("-mysql.services.clever-cloud.com:3306", "xgbhnnb`id{ty<~`", "Ue;LioBjUwN4]:L|T5G", "o>jc`fzotxojunu88n");
		db.connect("10.182.67.148:3306", "myuser", "MyPas$curs2", "curs");
		//db.connect("tcp://127.0.0.1:3306", "myuser", "mypass", "curs");
	}

	// Деструктор
	~A_menu() = default;

	static int changeMenu(const std::string& str, int cntMenu, const std::string& begMenu, int back) {
		int choice = -1;
		std::string endMenu = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";
		cout << endl << begMenu << endl;
		cout << str << endl;
		if (back == 0) cout << " 0 - Выход" << endl;
		else cout << " 0 - Назад" << endl;
		cout << endMenu.substr(0, begMenu.size()) << endl;
		cout << "Выберите: ";
		do {
			Checks::CheckInput(choice);
			if ((choice > cntMenu) || (choice < 0)) {
				cout << "Такого варианта нет в меню, попробуйте снова" << endl;
			}
		} while ((choice < 0) || (choice > cntMenu));
		std::cin.clear();
		return choice;
	}

	static size_t choiceMenu(size_t cntMenu, const std::string& strMenu) {
		cout << " 0 - --= ОТМЕНА =-- " << endl;
		cout << "Выберите " << strMenu << ": ";
		size_t ch = -1;
		do {
			std::cin >> ch;
			fflush(stdin);
		} while ((ch < 0) || (ch > cntMenu));
		return ch;
	}

	static bool Confirm(const std::string& msg) {
		//        int ls;
		//        cout << msg << " (Y/Д/[N/Н]): ";
		//        do {
		//            ls = cin.get();
		//        } while (((ls != 'Y') && (ls != 'y') && (ls != 'N') && (ls != 'n') && (ls != 'Д') && (ls != 'д') &&
		//                  (ls != 'Н') && (ls != '')) || (ls == 13));
		//        if ((ls == 'Y') || (ls == 'y') || (ls == 'Д') || (ls == 'д')) return true;
		//        else if ((ls == 'N') || (ls == 'n') || (ls == 'Н') || (ls == 'н') || (ls == 13)) return false;
		//        else return false;
		return true;
	}

	static int menuMain() {
		std::string strMenu = " 1 - Логин\n 2 - Регистрация";
		return changeMenu(strMenu, 6, "-=-=-=-=  М е н ю  =-=-=-=-", 0);
		//        return vcChoice("-=-=-=-=  М е н ю  =-=-=-=-", vcMainMenu, 0)
	}

	//----- Ввод нового пользователя -----
	void addUser(int flag = 2) {
		//UserSock user(sock, db.getRoles());
		UserSock user(sock, db.getGuide("user_role", 1, flag)); //, 2); // для вывода кроме админа (регистрация)
		user.enterUser();
		std::cout << user;
		db.addUser(user);
		std::cout << "---- Добавление пользователя ----" << std::endl;
		std::cout << user;
		db.deleteUser(user.getUid());
		std::cout << "---------------------------------" << std::endl;
	}

	//-------- Список пользователей ---------
	int listUsers(vector<std::string>& vc, int flag = 0) {	// 0 - Все
		int ch = 0;											// 1 - Эксперты
		if (!vc.empty()) vc.clear();						// 2 - Не Администраторы
		vc = db.getGuide("user", 2, flag);
		sendString(sock, "menu0");
		sendString(sock, toString(vc, "Выберите пользователя: "));
		ch = takeInt(sock);
		return ch;
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
		user.enterUser();
		user.setUid(oldUser.getUid());
		db.editUser(user, oldUser.getUid());
		std::cout << "-- Редактирование пользователя --" << std::endl;
		std::cout << oldUser;
		std::cout << "---------------------------------" << std::endl;
		std::cout << user;
		std::cout << "---------------------------------" << std::endl;
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
	void printUsers(std::string toFile = "") {
		std::vector<User> users = db.getUsers();

		for (auto& it : users) {
			it.printUser(it == users[0]);
			UserSock::printUserSock(sock, it, "", it == users[0]);
			UserSock::printUserSock(sock, it, toFile, it == users[0]);
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
		int ch = 0;
		if (!vc.empty()) vc.clear();
		vc = db.getGuide("company", 2);
		sendString(sock, "menu0");
		sendString(sock, toString(vc, "Выберите компанию: "));
		ch = takeInt(sock);
		return ch;
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
	void printCompanies() {
		std:vector<Company> vcCmps = db.getCompanies();
		for (auto& it : vcCmps) {
			it.printCompany(it == vcCmps[0]);
		}
	}

	//------- Вывод компаний в сокет --------
	void printCompaniesSock(std::string fout = "") {
		sendString(sock, "output" + fout);
		std:vector<Company> vcCmps = db.getCompanies();
		for (auto& it : vcCmps) {
			//it.printCompanySock(sock, db.getCompany("company_id", std::to_string(it.getCompanyId())).getName(), it == vcPrs[0]);
			it.printCompanySock(sock, it == vcCmps[0]);
		}
		sendString(sock, "end");
	}

	//------------ Вывод проектов -----------
	void printProjects() {
	std:vector<Project> vcPrs = db.getProjectVc();
		for (auto& it : vcPrs) {
			it.printProject(db.getCompany("company_id", std::to_string(it.getCompanyId())).getName(), it == vcPrs[0]);
		}
	}

	//------- Вывод проектов в сокет --------
	void printProjectsSock(std::string fout = "") {
		sendString(sock, "output" + fout);
	std:vector<Project> vcPrs = db.getProjectVc();
		for (auto& it : vcPrs) {
			it.printProjectSock(sock, db.getCompany("company_id", std::to_string(it.getCompanyId())).getName(), it == vcPrs[0]);
		}
		sendString(sock, "end");
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
		int ch = 0;
		if (!vc.empty()) vc.clear();
		vc = db.getGuide("project", 2);
		sendString(sock, "menu" + std::to_string(mode));
		sendString(sock, toString(vc, "Выберите проект: "));
		ch = takeInt(sock);
		return ch;
	}

	//--------- Список проектов map ---------
	size_t listProject(map<string, size_t>& vc, size_t mode = 0) {
		int ch = 0;
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
		int ch = listProject(projects);
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
		int ch = 0;
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

	// Меню Администратора
	void menuAdmin() {
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
		sendString(sock, strMenuAdmin);
		//sendMenu(sock, split(strMenuAdmin));
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			size_t i = atoi(p);
			std::cout << "<- " << split(strMenuAdmin)[i] << endl;
			switch (i) {
			case 1:
				// Добавление
				menuAdminAdd();
				break;
			case 2:
				// Редактировать
				menuAdminEdit();
				break;
			case 3:
				// Удаление
				menuAdminDel();
				break;
			case 4:
				// Сохранение
				menuAdminSave();
				break;
			case 5:
				// Поиск
				break;
			case 6:
				// Просмотр Инвестиционных проектов
				break;
			case 7:
				// Сортировка Инвестиционных проектов
				break;
			case 8:
				// Ранжировать Инвестиционные проекты
				menuAdminRanking();
				break;
			case 9:
				//sendString(sock, "menu");
				//sendString(sock, strMenuMain);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdmin);
		}
	}

	// Меню Администратора. Добавление
	void menuAdminAdd() {
		/*
		* 1. Новых пользователей
		* 2. Новые компании
		* 3. Новые проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuAdminAdd);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminAdd)[i] << endl;
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
				//sendString(sock, "menu");
				//sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminAdd);
		}
	}

	// Меню Администратора. Редактирование
	void menuAdminEdit() {
		/*
		* 1. Пользователей
		* 2. Компанию
		* 3. Проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuAdminEdit);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminEdit)[i] << endl;
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
				//sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminEdit);
		}
	}

	// Меню Администратора. Удаление
	void menuAdminDel() {
		/*
		* 1. Пользователей
		* 2. Компанию
		* 3. Проекты
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuAdminDel);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminDel)[i] << endl;
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
				//sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminDel);
		}
	}

	// Меню Администратора. Сохранение
	void menuAdminSave() {
		/*
		* 1. Пользователях#
		* 2. Компаниях#
		* 3. Проектах#
		* 4. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuAdminSave);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminSave)[i] << endl;
			switch (i) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminSave);
		}
	}

	// Ранжировать инвестиционные проекты
	void menuAdminRanking() {
		/*
		* 1. Найти оценки#
		* 2. Осуществить попарное сравнение проектов#
		* 3. Вычислить веса проектов#
		* 4. Составить порядок предпочтений#
		* 5. Вывести результат ранжирования ИП#
		* 6. Назад
		*/
		sendString(sock, "menu");
		sendString(sock, strMenuAdminRanking);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		Rating rating(sock);
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminRanking)[i] << endl;
			switch (i) {
			case 1:
				// Найти оценки

				rating.setNumber(3);

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
				// Составить порядок предпочтений

				// Вывод результата ранжирования
				rating.printRating();
				rating.printRatingSock();
				rating.printRatingSock("file");
				break;
			case 5:
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
			case 6:
				//sendString(sock, "menu");
				//sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminRanking);
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
		//sendMenu(sock, split(strMenuAdmin));
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
		* 3. Назад
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
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
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
				break;
			case 2:
				break;
			case 3:
				//sendString(sock, "menu");
				//sendString(sock, strMenuCompany);
				return;
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
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanyDel);
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
		int c = -1;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuExpert)[i] << endl;
			switch (i) {
			case 1:
				// Выставление оценок
				break;
			case 2:
				// Редактировать информацию о себе
				break;
			case 3:
				// Информация о инвестиционных проектах
				break;
			case 4:
				//sendString(sock, "menu");
				//sendString(sock, strMenuMain);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuExpert);
		}
	}

	void start() {
		int c;
		char p[200], com[200];//основной буфер и команда
		char curU[20];
		curU[0] = '\0';
		com[0] = '\0';
		p[0] = '\0';
		std::cout << "Соединение установлено." << std::endl;
		char* message = new char[100];
		strcat(p, "Server connected...\n");
		//send((SOCKET)newS, p, sizeof(p), 0);//посылаем приветствие при соединении
		std::string strMenu = "-=-=-=-=  М е н ю  =-=-=-=-#Логин#Регистрация#Логаут#Выход";
		Date tmpDate{};
		tmpDate.setDateStr(Date::currentDate());
		sendString(sock, "Server connected...\n" + tmpDate.getDateStr() + "\n");
		sendString(sock, "menu" + std::to_string(1));
		sendString(sock, strMenu);
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			command = p;
			//std::cout << "<- " << com << std::endl;
			int i = atoi(command.c_str());
			User oldUser;
			vector<std::string> users;
			std::map<std::string, size_t> userS;
			size_t ch;
			UserSock user(sock, db.getGuide("user_role", 1));
			CompanySock cmp(sock);
			ProjectSock project(sock, db.getGuide("company", 2));
			Rating rating(sock);
			switch (i) {
			case 1:
				//Подключение пользователя

				//sendString(sock, "data");
				//sendString(sock, "Логин: ");
				//userC.login = takeString(sock);
				//// ищем пользователя в базе
				//oldUser = db.getUser("login", userC.login);
				//// если находим
				//sendString(sock, "Пароль: ");
				//userC.pass = takeString(sock);
				//sendString(sock, "end");
				//if (userC.pass == oldUser.getPass()) {
				//	userC.Role = oldUser.getRole();
				//	std::cout << "login: " << userC.login << " - " << userC.pass << " - " << userC.Role << endl;
				//	if (oldUser.getRole() == "Администратор") menuAdmin();
				//	else if (oldUser.getRole() == "Эксперт") menuExpert();
				//	else menuCompany();
				//}
				//else {
				//	sendString(sock, "menu" + std::to_string(1));
				//	sendString(sock, strMenu);
				//}
				//                std::cout << "login: " << user.login << " - " << user.pass << endl;
				//if user = Admin
				menuAdmin();
				//menuCompany();
				break;
			case 2:
				//подключение пользователя
//                tUser user{};
//                sendString(sock, "data");
//                sendString(sock, "Введите логин: ");
//                user.login = takeString(sock);
//                // ищем пользователя в базе
//                // если находим
//                sendString(sock, "Введите пароль: ");
//                user.pass = takeString(sock);
//                sendString(sock, "end");
//                std::cout << "register: " << user.login << " - " << user.pass << endl;
//
//                menuExpert();
				//



				//ch = listProject(userS, 2); //, 2
				//std::cout << ch;

				//deleteMark();

				//// Выбор экспертов
				//rating.selectExperts(db.getGuideMap("user", 2, 1));
				//// Выбор проектов
				//rating.selectProjects(db.getProjectMp());
				//// Ввод оценок
				//rating.setNumber(3);
				//rating.enterRanks(db.getNewNumber());
				//for (auto& it : rating.ranking) {
				//	for (auto& iit : it.second) {
				//		db.addMark(iit);
				//	}
				//}



				//rating.setNumber(listNumberMark());
				//rating.selectNumber(db.getNumbersMark());

				//// Загрузка оценок
				//rating.ranking = db.getMpMarks(rating.getNumber());

				//// Загрузка экспертов в ранже
				//rating.mpExperts = db.getExpertMapMark(rating.getNumber());
				//rating.setCntExperts();

				//// Загрузка проектов в ранже
				//rating.vcProjects = db.getProjectVcMark(rating.getNumber());
				//rating.setCntProjects();

				////// Вывод проектов ранжа
				////rating.printVcProjects();
				////rating.printVcProjectsSock();
				////rating.printVcProjectsSock("file");

				//// Расчет весов и сумм по парным сравнениям
				//rating.vcProjects = db.getWeightVcMark(rating.getNumber());
				//rating.rankingTotal = db.getVcTotalMarks(rating.getNumber());

				//// Вывод таблицы ранжирования
				//rating.printRatingTable();
				//rating.printRatingTableSock();
				//rating.printRatingTableSock("file");

				//// Вывод результата ранжирования
				//rating.printRating();
				//rating.printRatingSock();
				//rating.printRatingSock("file");

				// вывод пользователей в консоль сервера
				//printUsers("file");

				//// вывод проектов в консоль сервера
				//printProjects();
				//printProjectsSock();
				//printProjectsSock("file");

				printCompanies();
				printCompaniesSock();
				printCompaniesSock("file");

				//menuAdmin();
				break;
			case 3:
				//отключение пользователя
				//cout << "User " << curU << " logout" << endl;
				sendString(sock, "exit");
				closesocket(sock);//закрываем сокет
				return;
				//break;
			case 4:
				//выход
				sendString(sock, "exit");
				closesocket(sock);//закрываем сокет
				exit(EXIT_SUCCESS);
				break;
			default:
				break;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuMain);
		}
	}
};

#endif //CURSCLN_MENUSRV_H