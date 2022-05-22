#ifndef CURSCLN_MENUCLN_H
#define CURSCLN_MENUCLN_H

#include "..\Utils\stdafx.h"

using namespace std;

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
	std::cout << "Выберите: ";
	int ch = -1;
	do {
		std::cin >> ch;
		fflush(stdin);
		if ((ch < cancel) || (ch > topVc)) {
			std::cout << "Неверный выбор!!! Повторите! Ваш вариант должен быть от " << cancel << " до " << topVc << ".\nВыберите: ";
		}
		else break;
	} while (true);
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
				//system("cls");
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
				//system("pause");
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
					std::string toFile = "report";
					fout.exceptions(ofstream::badbit);
					try {
						if (str.find("#") != std::string::npos) {
							toFile = str.substr(11, str.size() - 11);
						}
						fout.open(toFile + ".txt"); // , ios::app | ios_base::in);
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
