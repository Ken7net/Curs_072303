#ifndef CURSSRV_CONNECTION_H
#define CURSSRV_CONNECTION_H

#include <string>
#include "WinSock2.h"


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4996)

class Connection {
private:
    WSADATA wsaData;
    WORD wVersionRequested;

public:
    SOCKET sock;
    Connection() {
        wVersionRequested = MAKEWORD(2, 2);//������ ����� ������ ��������� � ������� �����, ������ - � �������.
        int err = WSAStartup(wVersionRequested, &wsaData);//�������������� ������ � WinSock dll
        if (err != 0) exit(-1); // ������ �������������
    }
    void createConnect(const char *ip, size_t _port){
        struct sockaddr_in peer;//�������� ���������
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_port);//���� ����������
        peer.sin_addr.s_addr = inet_addr(ip);// �.�. ������ � ������ �� ����� ����������
        sock = socket(AF_INET, SOCK_STREAM, 0);//������� TCP-����� � ��������-����������

    }
    ~Connection(){
        if (sock.) != nullptr
        WSACleanup();
    }

};


#endif //CURSSRV_CONNECTION_H
