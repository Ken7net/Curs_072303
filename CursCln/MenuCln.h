#ifndef CURSCLN_MENUCLN_H
#define CURSCLN_MENUCLN_H

#include "..\Utils\stdafx.h"

using namespace std;

/*
colors = {
  uncolorize: (str: string) = > str.replace(/ \x1B\[\d + m / gi, ''),
  reset: '\x1b[0m',
  bright : '\x1b[1m',
  dim : '\x1b[2m',     // bold
  italic : '\x1b[3m',  // non-standard feature
  underscore : '\x1b[4m',
  blink : '\x1b[5m',
  reverse : '\x1b[7m',
  hidden : '\x1b[8m',

  fg : {
	black: '\x1b[30m',
	red : '\x1b[31m',
	green : '\x1b[32m',
	yellow : '\x1b[33m',
	blue : '\x1b[34m',
	magenta : '\x1b[35m',
	cyan : '\x1b[36m',
	white : '\x1b[37m',
	crimson : '\x1b[38m',
  },

  bg: {
	black: '\x1b[40m',
	red : '\x1b[41m',
	green : '\x1b[42m',
	yellow : '\x1b[43m',
	blue : '\x1b[44m',
	magenta : '\x1b[45m',
	cyan : '\x1b[46m',
	white : '\x1b[47m',
	crimson : '\x1b[48m',
  },
}*/

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

//Функция выбора 
int vcChoicePress(std::vector<std::string> vc, std::string topStr, size_t cancel = 1) {
	int count = 2;
	size_t cntVc = vc.size(), cAll = 0, cCancel=0;
	std::cout << "- - - - - - - - - -" << std::endl;
	size_t topVc = vc.size() - 1;
	if (cancel == vc.size()) {
		topVc = cancel;
		cAll = cancel;
		cancel = 0;
		vc.push_back(" -= "+topStr+" = -");
	}
	if (cancel == 0) {
		vc.push_back(" -= ОТМЕНА =-");
		cCancel = vc.size();
	}
	while (true) {//Бесконечный цикл 

		system("cls");//очистка экрана 
		std::cout << vc[0] << std::endl;		//Вывод сообщения на экран 
		for (size_t i = 1; i < vc.size(); ++i) {//Вывод опций на экран
			if ((i + 1) == count) cout << "\x1b[48m" << "\x1b[31m>" << vc[i] << "\x1b[48m" << "\x1b[30m" << endl;
			else cout << " " << vc[i] << endl;
		}

		char pressed = _getch();	//Ожидание нажатия 
		if ((pressed == 'W' || pressed == 'w' || pressed == 'Ц' ||
			pressed == 'ц' || pressed == 72) && count != 2)	//Если нажатие клавиши вверх и не 1 опция
			count--;	//Количество нажатий уменьшаем 
		if ((pressed == 'S' || pressed == 's' || pressed == 'Ы' ||
			pressed == 'ы' || pressed == 80) && count != vc.size())	//Если  нажатие клавишей вниз и не последняя опция
			count++;	//Количество нажатий увеличиваем 
		if (pressed == '\r' || pressed == '\n') break;	//Если переход на новую строку, то выходим из цикла
	}

	if (cancel == 0) {
		if (count == cCancel) return 0;
		//if (count == cAll) return cAll;
	}
	if (cAll != 0) {
		if (count == cAll) return cAll;
	}
	return count - 1;
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

	void start(const size_t& modeMenu) {
		int v = 1;
		std::string str;
		//char str[50];
		while (true) {
			//system("cls");
			//v = menuMain();
			int chs = 1;
			str = takeString(cn);
			if (str.find("menu") != std::string::npos) {
				//system("cls");
				modeStat = 0;
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
				if (modeMenu == 1) chs = vcChoice(vcMenu, topS, v);
				else chs = vcChoicePress(vcMenu, topS, v);
				sendInt(cn,chs);
				str = "";
				v = 1;
			}
			if (str == "data") {
				modeStat = 1;
				// получение и отправка данных
				str = takeString(cn);
				while (str != "end") {
					std::cout << str;
					if (str.find("Пароль", 0) != std::string::npos) {
						str = input_pass();
					}
					else {
						//std::cin >> str;
						std::cin.clear();
						std::getline(std::cin >> std::ws, str);
						//std::getline(std::cin, str);
						std::cin.clear();
					}
					sendString(cn, str);
					str = takeString(cn);
				}
			}
			if (str.find("output") != std::string::npos) {
				modeStat = 2;
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
			if (str == "pause") {
				system("pause");
				std::cin.clear();
			}
			if (str == "exit") {
				toStream("- - - - - - - - - - - - - - - - - - - -\nСтатистика по подключению:\n");
				toStream("Получено и обработано меню: " + std::to_string(vcStats[0]) + "\n");
				toStream("Получено и передано строк данных: " + std::to_string(vcStats[1]) + "\n");
				toStream("Получено строк данных для вывода: " + std::to_string(vcStats[2]) + "\n");
				toStream("Получена команда выхода: 1.\n");
				system("pause");
				return;
			}
		}
	}
};

#endif //CURSCLN_MENUCLN_H
