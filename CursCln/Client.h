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

class Client {
private:
    SOCKET newConnection{};
public:
//    int Accept() const {
//        SOCKADDR_IN FromAddr;
//        int len = sizeof(FromAddr);
//        SOCKET sock;
//        sock = accept(newConnection, (SOCKADDR *) &FromAddr, &len);
//
//        printf("IP клиента  %3u.%3u.%3u.%3u\n",
//               FromAddr.sin_addr.S_un.S_un_b.s_b1,
//               FromAddr.sin_addr.S_un.S_un_b.s_b2,
//               FromAddr.sin_addr.S_un.S_un_b.s_b3,
//               FromAddr.sin_addr.S_un.S_un_b.s_b4);
//        return 0;
//    }
    Client() {
        //
    }

    int startClient() {
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(2, 2);//первая цифра версии находится в младшем байте, вторая - в старшем.
        int err = WSAStartup(wVersionRequested, &wsaData);//инициализируем работу с WinSock dll
        if (err != 0) {
            std::cout << "Ошибка подключения к серверу!";
            return -1;
        }

        struct sockaddr_in peer{};//адресная структура
        peer.sin_family = AF_INET;
        peer.sin_port = htons(1280);//порт соединения
        peer.sin_addr.s_addr = inet_addr("127.0.0.1");// т.к. клиент и сервер на одном компьютере
        newConnection = socket(AF_INET, SOCK_STREAM, 0);//создаем TCP-сокет с интернет-адресацией
        connect(newConnection, (struct sockaddr *) &peer, sizeof(peer));//запрос на открытие соединения
        return 0;
    }

    ~Client() {
        closesocket(newConnection);//закрываем сокет
        WSACleanup();
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

    void SendString(const std::string& a) const {
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
