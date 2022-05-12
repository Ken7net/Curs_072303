#ifndef CURSCLN_MENUSRV_H
#define CURSCLN_MENUSRV_H

#include "..\Utils\stdafx.h"
#include "User.h"
#include "Company.h"
#include "DBWork.h"


using namespace std;

//void OS::expert_opinion(Server& obj) //метод для ранжирования проектов методом парных сравнений
//{
//	Rating rating;
//
//	Read_Rating(rating, File_Rating);
//	if (vecProgram.size() == 0 || rating.number_of_programs == 0 || vecProgram.size() != rating.number_of_programs)
//	{
//		int ch = 1;
//		char buf[100];
//		itoa(ch, buf, 10);
//		send(obj.GetNewConnection(), buf, sizeof(buf), 0);		// отправили 1, если вектор программ пустой или кол-во программ == 0 или размер вектора!=кол-ву программ
//	}															// нечего оценивать или ошибка
//	else if (vecProgram.size() == 1) {							
//		int ch = 2;
//		char buf[100];
//		itoa(ch, buf, 10);
//		send(obj.GetNewConnection(), buf, sizeof(buf), 0);		// отправили 2, если размер вектора проектов == 1
//	}															// мало проектов для оценивания
//	else
//	{
//		int ch = 0;
//		char buf[100];
//		itoa(ch, buf, 10);
//		send(obj.GetNewConnection(), buf, sizeof(buf), 0);		// иначе отправили 0
//
//		//-------- Создание массива оценок --------
//		double** a = NULL;
//		a = new double* [rating.number_of_experts];
//		for (int i = 0; i < rating.number_of_experts; i++)
//		{
//			a[i] = new double[rating.width];
//		}
//		int k = 0;
//		for (int i = 0; i < rating.number_of_experts; i++)
//		{
//			for (int j = 0; j < rating.width; j++) {
//				a[i][j] = rating.vec[k];
//				k++;
//			}
//		}
//		//-----------------------------------------
// 
//		//---- Создание вектора сумм столбцов -----
//		vector<double> Sum(rating.width);
//		for (int i = 0; i < rating.width; i++) Sum[i] = 0;
//		for (int j = 0; j < rating.width; j++)
//		{
//			for (int i = 0; i < rating.number_of_experts; i++)
//			{
//					Sum[j] += a[i][j];
//			}
//		}
//		//-----------------------------------------
// 
//		//- Создание вектора сумм оценок проектов -
//		vector<double> AlmostFinalSum(rating.number_of_programs);
//
//		int f = 0;
//		int g = f;
//		int counter = 0, counter1 = 1, counter2 = 1;
//		int koef = 0, koef1 = 0;
//		for (int j = 0; j < Sum.size() / 2; j++)
//		{
//			if (f == rating.number_of_programs - 1) {
//				counter++;
//				counter1 = counter + 1;
//				g++;
//				f = g;
//			}
//			AlmostFinalSum[counter] += Sum[koef];
//			AlmostFinalSum[counter1] += Sum[counter2 + koef1];
//
//			counter1++;
//			counter2++;
//			f++;
//			koef += 2, koef1++;
//		}
//		//-----------------------------------------
//
//		//------ Сумма всех оценок проектов -------
//		double FinalSum = 0;
//		for (int i = 0; i < rating.number_of_programs; i++) {
//			FinalSum += AlmostFinalSum[i];
//		}
//		cout << FinalSum;
//		//-----------------------------------------
//
//		//-------- Вектор весов проектов ----------
//		vector<double> weight(rating.number_of_programs);
//		for (int i = 0; i < rating.number_of_programs; i++)
//		{
//			weight[i] = AlmostFinalSum[i] / FinalSum;
//		}
//		//-----------------------------------------
// 
//		//------ Вектор проектов с весами ---------
//		vector<Rating> strRating(rating.number_of_programs);
//		for (int i = 0; i < vecProgram.size(); i++) {
//			strRating[i].programName = vecProgram[i].getNameProgram();
//			strRating[i].weight = weight[i];
//		}
//		//-----------------------------------------
// 
//		//------- Сортировка вектора весов --------
//		Rating temp;
//		for (int i = 0; i < strRating.size() - 1; i++)
//		{
//			for (int j = 0; j < strRating.size() - i - 1; j++)
//			{
//				if (strRating[j].weight < strRating[j + 1].weight)
//				{
//					temp = strRating[j];
//					strRating[j] = strRating[j + 1];
//					strRating[j + 1] = temp;
//				}
//			}
//		}
//		//-----------------------------------------
// 
//		//----- Печать вектора весов проектов -----
//		obj.SendInt(vecProgram.size());
//
//		for (int i = 0; i < vecProgram.size(); i++) {
//			obj.SendString(strRating[i].programName);
//			obj.SendDouble(strRating[i].weight);
//		}
//		//-----------------------------------------
// 
//		//----------- Удаление массива ------------
//		for (int i = 0; i < rating.number_of_experts; i++) {
//			delete a[i];
//		}
//		delete[] a;
//		//-----------------------------------------
//	}
//}
// 
// void OS::add_rating(Server& obj) //метод добавления оценок экспертами
//{
//while (true) {
//	obj.SendInt(vecProgram.size());
//	printf_Program(obj);
//	int number = obj.TakeInt();
//	if (number == 0)
//	{
//		break;
//	}
// 
//	//------- Подсчет количества оценок -------
//	Rating rating;
//	rating.number_of_programs = vecProgram.size();
//	int k = 1;
//	rating.width = 0;
//	while (rating.number_of_programs - k != 0) {
//		rating.width += (rating.number_of_programs - k) * 2;
//		k++;
//	}
//	//-----------------------------------------
//	obj.SendInt(rating.width);
//	rating.number_of_experts = obj.TakeInt();
// 
//	//---- Ввод (получение) массива оценок ----
//	double a;
//	for (int i = 0; i < rating.number_of_experts; ++i)
//	{
//		for (int j = 0; j < rating.width; ++j) {
//			a = obj.TakeDouble();
//			rating.vec.push_back(a);
//		}
//	}
//	//-----------------------------------------
//	File file;
//	file.Write_Info(rating.number_of_experts, rating.number_of_programs, rating.width, rating.vec, File_Rating);
//	break;
//}
//}

// 
//template<class T>
//void File::Read_Info(vector<T>& vec, string name) //шаблонный метод для считывания данных из файла
//{
//	try
//	{
//		ifstream file;
//		file.open(name);
//		if (file.peek() == EOF)
//		{
//		}
//		else
//		{
//			while (!file.eof())
//			{
//				T obj;
//				file >> obj;
//				vec.push_back(obj);
//			}
//		}
//		file.close();
//	}
//	catch (Exception& ex)
//	{
//		ex.messegErrorFile(name);
//		exit(1);
//	}
//}



class A_menu {
private:
	SOCKET sock;
	DBWork db;
	tUser userC;
	vector<size_t> vcProjectOp;	//Вектор проектов для оценивания
	vector<double> vcMarksOp;	//Вектор оценок проектов
public:
	//static vector<std::string> vcMainMenu = {"Логин", "Регистрация", "Выход"};
	//All_info ai;
	std::string strMenuMain = "-=-=-=-=  М е н ю  =-=-=-=-#Логин#Регистрация#Логаут#Выход";
	std::string strMenuAdmin = "\tАдминистратор#Просмотр инвестиционных проектов#Добавление#Удаление#Сохранение информации в бд#Поиск#Сортировка инвестиционных проектов#Ранжировать инвестиционные проекты#Вывести результат ранжирования ИП#Редактировать#Выход";
	std::string strMenuAdminAdd = "Вы хотите добавить: #Новые компании#Новых экспертов#Новые проекты#Назад";
	std::string strMenuAdminDel = "Вы хотите удалить: #Компанию#Экспертов#Проекты#Назад";
	std::string strMenuAdminSave = "Вы хотите сохранить информацию о:#Компании#Экспертах#Проектах#Назад";
	std::string strMenuAdminRanking = "Ранжировать инвестиционные проекты:#Осуществить попарное сравнение проектов#Найти оценки#Вычислить веса проектов#Составить порядок предпочтений#Назад";
	std::string strMenuAdminEdit = "Вы хотите редактировать данные:#Компании#Экспертов#Проектов#Назад";
	std::string strMenuCompany = "\tКомпания#Ввод данных#Редактировать данные#Добавить проект#Сохранение информации в бд#Удаление данных о проекте#Выход";
	std::string strMenuCompanyEdit = "Вы хотите редактировать данные:#Компании#Проектов#Назад";
	std::string strMenuCompanySave = "Вы хотите сохранить информацию о:#Компании#Проектах#Назад";
	std::string strMenuExpert = "\tЭксперт#Выставление оценок#Редактировать данные о себе#Просмотреть информацию об инвестиционных проектах#Выход";

	// Конструктор
	A_menu() {
		sock = NULL;
	};

	explicit A_menu(SOCKET connection) {
		sock = connection;
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

	void menuAdmin() {
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
				break;
			case 2:
				menuAdminAdd();
				break;
			case 3:
				menuAdminDel();
				break;
			case 4:
				menuAdminSave();
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				menuAdminRanking();
				break;
			case 8:
				break;
			case 9:
				menuAdminEdit();
				break;
			case 10:
				sendString(sock, "menu");
				sendString(sock, strMenuMain);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdmin);
		}
	}

	void menuAdminAdd() const {
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
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				sendString(sock, "menu");
				sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminAdd);
		}
	}

	void menuAdminDel() const {
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
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				sendString(sock, "menu");
				sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminDel);
		}
	}

	void menuAdminSave() const {
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
				sendString(sock, "menu");
				sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminSave);
		}
	}

	void menuAdminRanking() const {
		sendString(sock, "menu");
		sendString(sock, strMenuAdminRanking);
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //пока принимаются команды
			int i = atoi(p);
			std::cout << "<- " << split(strMenuAdminRanking)[i] << endl;
			switch (i) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				sendString(sock, "menu");
				sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminRanking);
		}
	}

	void menuAdminEdit() const {
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
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				sendString(sock, "menu");
				sendString(sock, strMenuAdmin);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuAdminEdit);
		}
	}

	void menuCompany() {
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
				break;
			case 2:
				menuCompanyEdit();
				break;
			case 3:
				//menuCompanyEdit();
				break;
			case 4:
				menuCompanySave();
				break;
			case 5:
				break;
			case 6:
				sendString(sock, "menu");
				sendString(sock, strMenuMain);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompany);
		}
	}

	void menuCompanyEdit() const {
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
				break;
			case 2:
				break;
			case 3:
				sendString(sock, "menu");
				sendString(sock, strMenuCompany);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanyEdit);
		}
	}

	void menuCompanySave() {
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
				sendString(sock, "menu");
				sendString(sock, strMenuCompany);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuCompanySave);
		}
	}

	void menuExpert() const {
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
				break;
			case 2:
				//menuAdminAdd();
				break;
			case 3:
				//menuCompanyEdit();
				break;
			case 4:
				sendString(sock, "menu");
				sendString(sock, strMenuMain);
				return;
			}
			sendString(sock, "menu");
			sendString(sock, strMenuExpert);
		}
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
		MarkSock mark(sock, db.getGuideMap("user", 2, 1), db.getGuideMap("project", 2));
		mark.enterMark();
		//mark.setCompanyId(db.getCompany("company_name", project.Companies[project.getCompanyId()]).getId());
		std::cout << mark;
		db.addMark(mark);
		std::cout << "---------- Ввод новой оценки ----------" << std::endl;
		std::cout << mark;
		std::cout << "---------------------------------------" << std::endl;
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
		MarkSock newMark(sock, db.getGuideMap("user", 2, 1), db.getGuideMap("project", 2));
		newMark.enterMark();
		newMark.setMarKId(oldMark.getMarkId());
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
		tmpDate.setDateStr("2022/05/06");
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
			switch (i) {
			case 1:
				//Подключение пользователя

				sendString(sock, "data");
				sendString(sock, "Логин: ");
				userC.login = takeString(sock);
				// ищем пользователя в базе
				oldUser = db.getUser("login", userC.login);
				// если находим
				sendString(sock, "Пароль: ");
				userC.pass = takeString(sock);
				sendString(sock, "end");
				if (userC.pass == oldUser.getPass()) {
					userC.Role = oldUser.getRole();
					std::cout << "login: " << userC.login << " - " << userC.pass << " - " << userC.Role << endl;
					if (oldUser.getRole() == "Администратор") menuAdmin();
					else if (oldUser.getRole() == "Эксперт") menuExpert();
					else menuCompany();
				}
				else {
					sendString(sock, "menu");
					sendString(sock, strMenu);
				}
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



				ch = listProject(userS, 2); //, 2
				std::cout << ch;

				//deleteMark();



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
			sendString(sock, strMenu);
		}
	}
};

#endif //CURSCLN_MENUSRV_H