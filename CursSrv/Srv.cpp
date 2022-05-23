#include "Server.h"
#include <iostream>
#include <process.h> /* _beginthread, _endthread */
//#include <Windows.h>
#include <fstream>
#include "MenuSrv.h"


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4996)
#pragma warning (disable: 4099)

using namespace std;

static size_t cntClients = 0;
size_t dbLocation = 3;


void Work(void* newS) {//поток обслуживания
	A_menu menu((SOCKET)newS, dbLocation); // , dbLocation);
	menu.start();
}

int main(/*int argc, char* argv[]*/) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F0");

	//dbLocation = argc; // =0 remote
	//if (argc > 1)// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
	//{
	//	cout << argv[1] << endl;// вывод второй строки из массива указателей на строки(нумерация в строках начинается с 0 )
	//	if (strcmp(argv[1], "-local") == 0) dbLocation = 1;
	//	else if (strcmp(argv[1], "-remote") == 0) dbLocation = 0;
	//}

	std::string str;
	ifstream infile("CursSrv.ini");//создать объект типа ifstream
	if (!infile.is_open()) {
		std::cout << "Не получается открыть файл для чтения данных!" << std::endl;
		return 0;
	}
	size_t _port = 1280;
	while (getline(infile, str)) {//пока не достигнут конец файла поместить очередную строку в переменную str1
		//std::cout << str << endl;//выводим на экран str
		if (str.find("Location") != std::string::npos) {
			dbLocation = static_cast<size_t>(std::stoi(str.substr(str.find("=") + 1, str.size() - str.find("="))));
		}
		if (str.find("Port") != std::string::npos) {
			_port = static_cast<size_t>(std::stoi(str.substr(str.find("=") + 1, str.size() - str.find("="))));
		}
	}

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);//первая цифра версии находится в младшем байте, вторая - в старшем.
	int err = WSAStartup(wVersionRequested, &wsaData);//инициализируем работу с WinSock dll
	if (err != 0) return err;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//создаем TCP-сокет с интернет-адресацией
	struct sockaddr_in local {};//адресная структура
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);//порт соединения
	local.sin_addr.s_addr = htonl(INADDR_ANY);//посылать или принимать данные через любой IP-адрес данного компьютера
	bind(s, (struct sockaddr*)&local, sizeof(local));//связываем адрес с сокетом
	listen(s, 5);//установка в прослушивание с 5-ти кратными попытками соединения
	std::cout << "Сервер запущен и ожидает соединений." << std::endl;
	while (true) {//цикл извлечения запросов на подключение из очереди
		sockaddr_in remote{};//адрес клиента(заполняется системой)
		int j = sizeof(remote);//адрес клиента(заполняется системой)
		SOCKET newS = accept(s, (struct sockaddr*)&remote, &j);//подключение
		cntClients++;
		std::cout << "Клиент подключен. Текущее количество подключений: " << cntClients << std::endl;
		std::cout << remoteIP(remote) << std::endl;
		_beginthread(Work, 0, (void*)newS);//запускаем поток для обслуживания клиента
	}
	WSACleanup();
	return 0;
};
