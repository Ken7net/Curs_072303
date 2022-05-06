#include "Server.h"
#include <iostream>
#include <process.h> /* _beginthread, _endthread */
#include <Windows.h>
#include <fstream>
#include "MenuSrv.h"


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4996)

using namespace std;

void Work(void* newS) {//поток обслуживания
	A_menu menu((SOCKET)newS);
	menu.start();
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//setlocale(LC_ALL, "1251");// utf - 8");//это на всякий случай
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);//первая цифра версии находится в младшем байте, вторая - в старшем.
	int err = WSAStartup(wVersionRequested, &wsaData);//инициализируем работу с WinSock dll
	if (err != 0) return err;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//создаем TCP-сокет с интернет-адресацией
	struct sockaddr_in local;//адресная структура
	local.sin_family = AF_INET;
	local.sin_port = htons(1280);//порт соединения
	local.sin_addr.s_addr = htonl(INADDR_ANY);//посылать или принимать данные через любой IP-адрес данного компьютера
	bind(s, (struct sockaddr*)&local, sizeof(local));//связываем адрес с сокетом
	listen(s, 5);//установка в прослушивание с 5-ти кратными попытками соединения
	while (true) {//цикл извлечения запросов на подключение из очереди
		sockaddr_in remote;//адрес клиента(заполняется системой)
		int j = sizeof(remote);//адрес клиента(заполняется системой)
		SOCKET newS = accept(s, (struct sockaddr*)&remote, &j);//подключение
		std::cout << remoteIP(remote) << std::endl;
		_beginthread(Work, 0, (void*)newS);//запускаем поток для обслуживания клиента
	}
	WSACleanup();
	return 0;
};
