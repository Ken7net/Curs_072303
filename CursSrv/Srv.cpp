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

void Work(void* newS) {//����� ������������
	A_menu menu((SOCKET)newS);
	menu.start();
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//setlocale(LC_ALL, "1251");// utf - 8");//��� �� ������ ������
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);//������ ����� ������ ��������� � ������� �����, ������ - � �������.
	int err = WSAStartup(wVersionRequested, &wsaData);//�������������� ������ � WinSock dll
	if (err != 0) return err;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//������� TCP-����� � ��������-����������
	struct sockaddr_in local;//�������� ���������
	local.sin_family = AF_INET;
	local.sin_port = htons(1280);//���� ����������
	local.sin_addr.s_addr = htonl(INADDR_ANY);//�������� ��� ��������� ������ ����� ����� IP-����� ������� ����������
	bind(s, (struct sockaddr*)&local, sizeof(local));//��������� ����� � �������
	listen(s, 5);//��������� � ������������� � 5-�� �������� ��������� ����������
	while (true) {//���� ���������� �������� �� ����������� �� �������
		sockaddr_in remote;//����� �������(����������� ��������)
		int j = sizeof(remote);//����� �������(����������� ��������)
		SOCKET newS = accept(s, (struct sockaddr*)&remote, &j);//�����������
		std::cout << remoteIP(remote) << std::endl;
		_beginthread(Work, 0, (void*)newS);//��������� ����� ��� ������������ �������
	}
	WSACleanup();
	return 0;
};
