#include <iostream>
#include "Client.h"
#include "MenuCln.h"
//#include <locale.h>
#include<Windows.h>
#include "WinSock2.h"


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4996)

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F0");

	//Client cln;
	//setlocale(LC_ALL, "1251");
	//system("mode con: cols=70 lines=20");
	size_t modeMenu = 1;
	std::string str;
	ifstream infile("CursCln.ini");//создать объект типа ifstream
	if (!infile.is_open()) {
		std::cout << "Не получается открыть файл для чтения данных!" << std::endl;
		return 0;
	}
	std::pair<std::string, size_t> adrServer = make_pair("127.0.0.1", 1280);
	while (getline(infile, str)) {//пока не достигнут конец файла поместить очередную строку в переменную str1
		//std::cout << str << endl;//выводим на экран str
		if (str.find("IP") != std::string::npos) {
			adrServer.first = str.substr(str.find("=") + 1, str.size() - str.find("="));
		}
		if (str.find("Port") != std::string::npos) {
			adrServer.second = static_cast<size_t>(std::stoi(str.substr(str.find("=") + 1, str.size() - str.find("="))));
		}
		if (str.find("Menu") != std::string::npos) {
			modeMenu = static_cast<size_t>(std::stoi(str.substr(str.find("=") + 1, str.size() - str.find("="))));
		}
	}

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);//первая цифра версии находится в младшем байте, вторая - в старшем.
	int err = WSAStartup(wVersionRequested, &wsaData);//инициализируем работу с WinSock dll
	if (err != 0) return -1;

	struct sockaddr_in peer {};//адресная структура
	peer.sin_family = AF_INET;
	peer.sin_port = htons(adrServer.second);//порт соединения

	peer.sin_addr.s_addr = inet_addr(adrServer.first.c_str());// т.к. клиент и сервер на одном компьютере
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//создаем TCP-сокет с интернет-адресацией
	//connect(s, (struct sockaddr*)&peer, sizeof(peer));//запрос на открытие соединения
	if (connect(s, (SOCKADDR*)&peer, sizeof(peer)) != 0) {
		std::cout << "Ошибка, не получилось подсоединиться к серверу" << endl;
		system("pause");
		return 1;
	}
	else {
		std::cout << takeString(s);
		sendInt(s, modeMenu);
		A_menu menu(s);
		menu.start(modeMenu);
		sendString(s, "Отключаюсь...");
	}
	closesocket(s);
	WSACleanup();
}
