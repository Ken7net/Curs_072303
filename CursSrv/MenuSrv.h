#ifndef CURSCLN_MENUSRV_H
#define CURSCLN_MENUSRV_H

#include "..\Utils\stdafx.h"
#include "User.h"
#include "Company.h"
#include "DBWork.h"

using namespace std;

class A_menu {
private:
	SOCKET sock;
	DBWork db;
	tUser userC;
public:
	//static vector<std::string> vcMainMenu = {"�����", "�����������", "�����"};
	//All_info ai;
	std::string strMenuMain = "-=-=-=-=  � � � �  =-=-=-=-#�����#�����������#������#�����";
	std::string strMenuAdmin = "\t�������������#�������� �������������� ��������#����������#��������#���������� ���������� � ��#�����#���������� �������������� ��������#����������� �������������� �������#������� ��������� ������������ ��#�������������#�����";
	std::string strMenuAdminAdd = "�� ������ ��������: #����� ��������#����� ���������#����� �������#�����";
	std::string strMenuAdminDel = "�� ������ �������: #��������#���������#�������#�����";
	std::string strMenuAdminSave = "�� ������ ��������� ���������� �:#��������#���������#��������#�����";
	std::string strMenuAdminRanking = "����������� �������������� �������:#����������� �������� ��������� ��������#����� ������#��������� ���� ��������#��������� ������� ������������#�����";
	std::string strMenuAdminEdit = "�� ������ ������������� ������:#��������#���������#��������#�����";
	std::string strMenuCompany = "\t��������#���� ������#������������� ������#�������� ������#���������� ���������� � ��#�������� ������ � �������#�����";
	std::string strMenuCompanyEdit = "�� ������ ������������� ������:#��������#��������#�����";
	std::string strMenuCompanySave = "�� ������ ��������� ���������� �:#��������#��������#�����";
	std::string strMenuExpert = "\t�������#����������� ������#������������� ������ � ����#����������� ���������� �� �������������� ��������#�����";

	// �����������
	A_menu() {
		sock = NULL;
	};

	explicit A_menu(SOCKET connection) {
		sock = connection;
		db.connect("tcp://10.182.67.148:3306", "myuser", "MyPas$curs2", "curs");
	}

	// ����������
	~A_menu() = default;

	static int changeMenu(const std::string& str, int cntMenu, const std::string& begMenu, int back) {
		int choice = -1;
		std::string endMenu = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";
		cout << endl << begMenu << endl;
		cout << str << endl;
		if (back == 0) cout << " 0 - �����" << endl;
		else cout << " 0 - �����" << endl;
		cout << endMenu.substr(0, begMenu.size()) << endl;
		cout << "��������: ";
		do {
			Checks::CheckInput(choice);
			if ((choice > cntMenu) || (choice < 0)) {
				cout << "������ �������� ��� � ����, ���������� �����" << endl;
			}
		} while ((choice < 0) || (choice > cntMenu));
		std::cin.clear();
		return choice;
	}

	static size_t choiceMenu(size_t cntMenu, const std::string& strMenu) {
		cout << " 0 - --= ������ =-- " << endl;
		cout << "�������� " << strMenu << ": ";
		size_t ch = -1;
		do {
			std::cin >> ch;
			fflush(stdin);
		} while ((ch < 0) || (ch > cntMenu));
		return ch;
	}

	static bool Confirm(const std::string& msg) {
		//        int ls;
		//        cout << msg << " (Y/�/[N/�]): ";
		//        do {
		//            ls = cin.get();
		//        } while (((ls != 'Y') && (ls != 'y') && (ls != 'N') && (ls != 'n') && (ls != '�') && (ls != '�') &&
		//                  (ls != '�') && (ls != '')) || (ls == 13));
		//        if ((ls == 'Y') || (ls == 'y') || (ls == '�') || (ls == '�')) return true;
		//        else if ((ls == 'N') || (ls == 'n') || (ls == '�') || (ls == '�') || (ls == 13)) return false;
		//        else return false;
		return true;
	}

	static int menuMain() {
		std::string strMenu = " 1 - �����\n 2 - �����������";
		return changeMenu(strMenu, 6, "-=-=-=-=  � � � �  =-=-=-=-", 0);
		//        return vcChoice("-=-=-=-=  � � � �  =-=-=-=-", vcMainMenu, 0)
	}

	void menuAdmin() {
		sendString(sock, "menu");
		sendString(sock, strMenuAdmin);
		//sendMenu(sock, split(strMenuAdmin));
		char p[200];
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (int c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
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

	void start() {
		int c;
		char p[200], com[200];//�������� ����� � �������
		char curU[20];
		curU[0] = '\0';
		com[0] = '\0';
		p[0] = '\0';
		std::cout << "���������� �����������." << std::endl;
		char* message = new char[100];
		strcat(p, "Server connected...\n");
		//send((SOCKET)newS, p, sizeof(p), 0);//�������� ����������� ��� ����������
		std::string strMenu = "-=-=-=-=  � � � �  =-=-=-=-#�����#�����������#������#�����";
		Date tmpDate{};
		tmpDate.setDateStr("2022/05/06");
		sendString(sock, "Server connected...\n"+tmpDate.getDateStr());
		sendString(sock, "menu");
		sendString(sock, strMenu);
		strcpy(p, "");
		p[0] = '\0';
		std::string command;
		while (c = recv(sock, p, sizeof(p), 0) != 0) { //���� ����������� �������
			command = p;
			//std::cout << "<- " << com << std::endl;
			int i = atoi(command.c_str());
			User oldUser;
			vector<std::string> users;
			size_t ch;
			UserSock user(sock, db.getGuide("user_role", 1));
			switch (i) {
			case 1:
				//����������� ������������
				
				sendString(sock, "data");
				sendString(sock, "�����: ");
				userC.login = takeString(sock);
				// ���� ������������ � ����
				oldUser = db.getUser("login", userC.login);
				// ���� �������
				sendString(sock, "������: ");
				userC.pass = takeString(sock);
				sendString(sock, "end");
				if (userC.pass == oldUser.getPass()) {
					userC.Role = oldUser.getRole();
					std::cout << "login: " << userC.login << " - " << userC.pass << " - " << userC.Role << endl;
					if (oldUser.getRole() == "�������������") menuAdmin();
					else if (oldUser.getRole() == "�������") menuExpert();
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
			case 2 :
				//����������� ������������
//                tUser user{};
//                sendString(sock, "data");
//                sendString(sock, "������� �����: ");
//                user.login = takeString(sock);
//                // ���� ������������ � ����
//                // ���� �������
//                sendString(sock, "������� ������: ");
//                user.pass = takeString(sock);
//                sendString(sock, "end");
//                std::cout << "register: " << user.login << " - " << user.pass << endl;
//
//                menuExpert();
				//
				////----- ���� ������ ������������ -----

				////UserSock user(sock, db.getRoles());
				//UserSock user(sock, db.getGuide("user_role", 1));
				//user.enterUser();
				//std::cout << user;
				//db.addUser(user);

				//----- �������������� ������������ -----

				//UserSock user(sock, db.getRoles());
				users = db.getGuide("user", 2);
				sendString(sock, "menu");
				sendString(sock, toString(users));
				ch = takeInt(sock);
				if (ch != 0) oldUser = db.getUser("user_name", users[ch - 1]);
				else return;
				//UserSock user(sock, db.getGuide("user_role", 1));
				user.enterUser();
				std::cout << user;
				db.editUser(user, oldUser.getUid());
				menuAdmin();
				break;
			case 3:
				//���������� ������������
				//cout << "User " << curU << " logout" << endl;
				sendString(sock, "exit");
				closesocket(sock);//��������� �����
				return;
				//break;
			case 4:
				//�����
				sendString(sock, "exit");
				closesocket(sock);//��������� �����
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
