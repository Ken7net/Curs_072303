#ifndef CURSCLN_MENUCLN_H
#define CURSCLN_MENUCLN_H

#include "..\Utils\stdafx.h"

using namespace std;

int vcChoiceOld(std::vector<std::string> vc, const std::string& strMenu = "", bool back = false) {
	std::cout << "�������� " + strMenu + ": " << std::endl;
	for (size_t i = 0; i < vc.size(); ++i) {
		std::cout << std::setw(2) << i + 1 << ". " << vc[i] << std::endl;
	}
	if (back) std::cout << "0. -= ������ =- " << std::endl;
	int ch = -1;
	do {
		std::cin >> ch;
		fflush(stdin);
	} while ((ch < 0) || (ch > vc.size()));
	return ch;
}
int vcChoice(std::vector<std::string> vc) {
	std::cout << vc[0] << std::endl;
	for (size_t i = 1; i < vc.size(); ++i) {
		std::cout << std::setw(2) << i << ". " << vc[i] << std::endl;
	}
	std::cout << "��������: ";
	int ch = -1;
	do {
		std::cin >> ch;
		fflush(stdin);
	} while ((ch < 1) || (ch > vc.size() - 1));
	return ch;
}

class A_menu {
private:
	SOCKET cn;
public:
	std::vector<std::string> vcMenu;
	//All_info ai;

	// �����������
	A_menu() {
		cn = NULL;
	};

	explicit A_menu(SOCKET connection) {
		cn = connection;
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

	void start() {
		int v;
		std::string str;
		//char str[50];
		while (true) {
			//system("cls");
			//v = menuMain();
			str = takeString(cn);
			if (str == "menu") {
				if (!vcMenu.empty()) {
					vcMenu.clear();
				}
				vcMenu = takeMenu(cn);
				sendInt(cn, vcChoice(vcMenu));
			}
			if (str == "data") {
				// ��������� � �������� ������
				str = takeString(cn);
				while (str != "end") {
                    std::cout << str;
					if (str.find("������: ", 0) != std::string::npos) {
						str = input_pass();
					}
					else {
						std::cin >> str;
					}
					sendString(cn, str);
					str = takeString(cn);
				}
			}
			if (str == "exit") {
				return;
			}
		}
	}
};

#endif //CURSCLN_MENUCLN_H
