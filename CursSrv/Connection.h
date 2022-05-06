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
        wVersionRequested = MAKEWORD(2, 2);//первая цифра версии находится в младшем байте, вторая - в старшем.
        int err = WSAStartup(wVersionRequested, &wsaData);//инициализируем работу с WinSock dll
        if (err != 0) exit(-1); // Ошибка инициализации
    }
    void createConnect(const char *ip, size_t _port){
        struct sockaddr_in peer;//адресная структура
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_port);//порт соединения
        peer.sin_addr.s_addr = inet_addr(ip);// т.к. клиент и сервер на одном компьютере
        sock = socket(AF_INET, SOCK_STREAM, 0);//создаем TCP-сокет с интернет-адресацией

    }
    ~Connection(){
        if (sock.) != nullptr
        WSACleanup();
    }

};


#endif //CURSSRV_CONNECTION_H
