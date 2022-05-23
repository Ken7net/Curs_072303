#ifndef CURSSRV_SERVER_H
#define CURSSRV_SERVER_H

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <cstdlib>
#include "iostream"
#include "WinSock2.h"
#include <string>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4996)
#pragma warning (disable: 4099)

class Server {
private:
    SOCKET newConnection;
public:
    int Accept() const {
        SOCKADDR_IN FromAddr{};
        int len = sizeof(FromAddr);
        SOCKET sock;
        sock = accept(newConnection, (SOCKADDR *) &FromAddr, &len);

        printf("IP клиента  %3u.%3u.%3u.%3u\n",
               FromAddr.sin_addr.S_un.S_un_b.s_b1,
               FromAddr.sin_addr.S_un.S_un_b.s_b2,
               FromAddr.sin_addr.S_un.S_un_b.s_b3,
               FromAddr.sin_addr.S_un.S_un_b.s_b4);
        return 0;
    }

    void startServer() {
        WSADATA Wsdata{};
        WORD version = MAKEWORD(2, 1);

        if (WSAStartup(version, &Wsdata) != 0) {
            std::cout << "Не удалось загузить библиотеку WSdata" << std::endl;
            exit(1);
        }
        SOCKADDR_IN addr{};
        int size = sizeof(addr);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(1111);    //порт соединения
        addr.sin_family = AF_INET;

        SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //создаем TCP-сокет с интернет-адресацией
        bind(sListen, (SOCKADDR *) &addr, sizeof(addr));//связываем адрес с сокетом
        listen(sListen, SOMAXCONN);
        newConnection = accept(sListen, (SOCKADDR *) &addr, &size);

        if (newConnection == 0) {
            std::cout << "error";
        } else {
            std::cout << "Клиент присоединился к серверу!" << std::endl;
            Accept();
        }
    }

    SOCKET GetNewConnection() const {
        return newConnection;
    }

    void SendInt(int a) const {
        char msg[200];
        itoa(a, msg, 10);
        send(newConnection, msg, sizeof(msg), 0);
    }

    void SendDouble(double a) const {
        std::string b;
        b = std::to_string(a);
        char msg[200];
        strcpy(msg, b.c_str());
        send(newConnection, msg, sizeof(msg), 0);
    }

    void SendString(std::string a) const {
        char msg[200];
        strcpy(msg, a.c_str());
        send(newConnection, msg, sizeof(msg), 0);
    }

    int TakeInt() {
        char msg[200];
        int a;
        recv(newConnection, msg, sizeof(msg), 0);
        a = atoi(msg);
        return a;
    }

    double TakeDouble() const {
        char msg[200];
        recv(newConnection, msg, sizeof(msg), 0);
        double a = atof(msg);
        return a;
    }

    std::string TakeString() const {
        char msg[200];
        recv(newConnection, msg, sizeof(msg), 0);
        std::string str = std::string(msg);
        return str;
    }
};

#endif //CURSSRV_SERVER_H
