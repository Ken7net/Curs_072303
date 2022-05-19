#ifndef CURSCLN_MENUCLN_H
#define CURSCLN_MENUCLN_H

#include "..\Utils\stdafx.h"

using namespace std;

int vcChoiceOld(std::vector<std::string> vc, const std::string& strMenu = "", bool back = false) {
	std::cout << "Выберите " + strMenu + ": " << std::endl;
	for (size_t i = 0; i < vc.size(); ++i) {
		std::cout << std::setw(2) << i + 1 << ". " << vc[i] << std::endl;
	}
	if (back) std::cout << "0. -= ОТМЕНА =- " << std::endl;
	int ch = -1;
	do {
		std::cin >> ch;
		fflush(stdin);
	} while ((ch < 0) || (ch > vc.size()));
	return ch;
}
int vcChoice(std::vector<std::string> vc, std::string topStr, size_t cancel = 1) {
	std::cout << vc[0] << std::endl;
	for (size_t i = 1; i < vc.size(); ++i) {
		std::cout << std::setw(2) << i << ". " << vc[i] << std::endl;
	}
	std::cout << "- - - - - - - - - -" << std::endl;
	size_t topVc = vc.size() - 1;
	if (cancel == vc.size()) {
		topVc = cancel;
		cancel = 0;
		std::cout << std::setw(2) << topVc << ". -= " + topStr + " = -" << std::endl;
	}
	if (cancel == 0) {
		std::cout << " 0. -= ОТМЕНА =-" << std::endl;
		std::cout << "- - - - - - - - - -" << std::endl;
	}
	//std::cout << "Выберите: ";
	int ch = -1;
	do {
		std::cout << "Выберите: ";
		std::cin >> ch;
		fflush(stdin);
	} while ((ch < cancel) || (ch > topVc));
	return ch;
}

class A_menu {
private:
	SOCKET cn;
public:
	std::vector<std::string> vcMenu;

	// Конструктор
	A_menu() {
		cn = NULL;
	};

	explicit A_menu(SOCKET connection) {
		cn = connection;
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

	void toStream(std::string st, ostream& fout = std::cout) const {
		fout << st;
	}

	void start() {
		int v = 1;
		std::string str;
		//char str[50];
		while (true) {
			//system("cls");
			//v = menuMain();
			str = takeString(cn);
			if (str.find("menu") != std::string::npos) {
				if (str.size() > 4) {
					v = stoi(str.substr(4, str.size() - 4));
				}
				if (!vcMenu.empty()) {
					vcMenu.clear();
				}
				vcMenu = takeMenu(cn);
				std::string topS = "Все";
				if (v != 1 && v != 0) { 
					if (v == 303) topS = "Новый";
					v = vcMenu.size();
				}
				sendInt(cn, vcChoice(vcMenu, topS, v));
				str = "";
				v = 1;
			}
			if (str == "data") {
				// получение и отправка данных
				str = takeString(cn);
				while (str != "end") {
					std::cout << str;
					if (str.find("Пароль", 0) != std::string::npos) {
						str = input_pass();
					}
					else {
						std::cin >> str;
					}
					sendString(cn, str);
					str = takeString(cn);
				}
			}
			if (str.find("output") != std::string::npos) {
				// получение и отправка данных 
				bool flagFile = false;
				ofstream fout;
				if (str.find("file") != std::string::npos) {
					flagFile = true;
					fout.exceptions(ofstream::badbit);
					try {
						fout.open("report.txt", ios::app | ios_base::in);
					}
					catch (const ofstream::failure& e) {
						cout << "Ошибка открытия файла (report.txt)!" << endl;
						break;
					}
				}
				str = takeString(cn);
				while (str != "end") {
					if (flagFile) toStream(str, fout);
					else toStream(str);
					str = takeString(cn);
				}
				//system("pause");
				if (flagFile) {
					fout.close();
				}
			}
			if (str == "exit") {
				return;
			}
		}
	}
};

#endif //CURSCLN_MENUCLN_H
