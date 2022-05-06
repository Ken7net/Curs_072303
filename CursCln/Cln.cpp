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

    //Client cln;
    //setlocale(LC_ALL, "1251");
    system("mode con: cols=70 lines=20");

    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);//������ ����� ������ ��������� � ������� �����, ������ - � �������.
    int err = WSAStartup(wVersionRequested, &wsaData);//�������������� ������ � WinSock dll
    if (err != 0) return -1;

    struct sockaddr_in peer;//�������� ���������
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);//���� ����������
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");// �.�. ������ � ������ �� ����� ����������
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//������� TCP-����� � ��������-����������
    connect(s, (struct sockaddr *) &peer, sizeof(peer));//������ �� �������� ����������
//    if (connect(s, (SOCKADDR *) &peer, sizeof(peer)) != 0) {
//
//    if (cln.startClient() != 0) {
//        cout << "������, ������������ �������������� � �������" << endl;
//        return 1;
//    } else {
    std::cout << takeString(s);
    A_menu menu(s);
    menu.start();
    sendString(s, "����������");
    //    }
    closesocket(s);
    WSACleanup();
    //system("pause");
}
