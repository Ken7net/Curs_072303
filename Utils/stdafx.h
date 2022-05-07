#ifndef SRV_STDAFX_H
#define SRV_STDAFX_H

#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4619)
#pragma warning(disable : 4548)
#pragma warning(disable : 4668)
#pragma warning(disable : 4365)
#pragma warning(disable : 4710)
#pragma warning(disable : 4371)
#pragma warning(disable : 4826)
#pragma warning(disable : 4061)
#pragma warning(disable : 4640)

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <format>
#include <conio.h>   //для getch
//#include "Menu.h"
#include <sstream>
#include <WinSock2.h>

#pragma warning(pop) //For /Wall

namespace Checks {
    int checkInt() //проверка на целые числа
    {
        int a;
        while (true) {
            std::cin >> a;
            try {
                if ((std::cin.get() != '\n')) {
                    throw "Некорректный ввод. \n";
                }
                break;
            }
            catch (const char *err) {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout << err << "Повторите попытку: " << std::endl;
                //cin >> a;
                //break;
            }
        }
        return a;
    }

    bool checkNoNumbers(const std::string &value) {
        return std::all_of(value.begin(), value.end(), [](const char &ch) { return !isdigit(ch); });
    }

    bool checkNoLetters(const std::string &value) {
        return std::all_of(value.begin(), value.end(), [](const char &ch) { return !isalpha(ch); });
    }

    std::string checkString(size_t length = 20) {
        std::string str;
        while (true) {
            std::cin >> str;
            try {
                if (str.length() > length) {
                    throw "Cлишком длинная строка!!!";
                }
                break;
            }
            catch (const char *err) {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout << err << "Повторите попытку: " << std::endl;
            }
        }
        return str;
    }

    template<typename T>
    void CheckInput(T &tmp) {
        while (true) {
            std::cin >> tmp;
            if (std::cin.get() != '\n') {
                std::cout << " Ошибка ввода, повторите попытку: ";
                std::cin.clear();
                std::cin.ignore(32767, '\n');
            } else break;
        }
    }
}
//----------------------------------------------------------------------------
void sendInt(SOCKET newConnection, int a) {
    char msg[200];
    itoa(a, msg, 10);
    send(newConnection, msg, sizeof(msg), 0);
}
//----------------------------------------------------------------------------
void sendDouble(SOCKET newConnection, double a) {
    std::string b;
    b = std::to_string(a);
    char msg[200];
    strcpy(msg, b.c_str());
    send(newConnection, msg, sizeof(msg), 0);
}
//----------------------------------------------------------------------------
void sendString(SOCKET newConnection, std::string a) {
    //    char* msg = new char[a.size()];
    char msg[300];
    //    strcpy(msg, a.c_str());
    strcpy(msg, a.c_str());
    //size_t size_a = a.size();
    send(newConnection, msg, sizeof(msg), 0);
}
//----------------------------------------------------------------------------
int takeInt(SOCKET newConnection) {
    char msg[200];
    int a;
    recv(newConnection, msg, sizeof(msg), 0);
    a = atoi(msg);
    return a;
}
//----------------------------------------------------------------------------
double takeDouble(SOCKET newConnection) {
    char msg[200];
    recv(newConnection, msg, sizeof(msg), 0);
    double a = atof(msg);
    return a;
}
//----------------------------------------------------------------------------
std::string takeString(SOCKET newConnection) {
    char msg[300];
    recv(newConnection, msg, sizeof(msg), 0);
    std::string str = std::string(msg);
    return str;
}
//----------------------------------------------------------------------------
std::string remoteIP(SOCKADDR_IN FromAddr) {
    return "IP клиента: " + std::to_string(FromAddr.sin_addr.S_un.S_un_b.s_b1) + "." +
           std::to_string(FromAddr.sin_addr.S_un.S_un_b.s_b2) + "." +
           std::to_string(FromAddr.sin_addr.S_un.S_un_b.s_b3) + "." +
           std::to_string(FromAddr.sin_addr.S_un.S_un_b.s_b4) + "\n";
}
//----------------------------------------------------------------------------
struct tUser {
    std::string name;
    std::string login;
    std::string pass;
    std::string Role;
};
//----------------------------------------------------------------------------
std::string input_pass(/*char *str1*/)    //Функция ввода массива символов
{                                                                //name - что вводим,
    //str строка,
    //num=1 если нужны только символы
    size_t nn = 0;
    size_t mm = 0;
    int num = 0;
    char str[20];
    do {
        nn = _getch();
        if ((nn != 10) && (nn != 13)) {
            if (num == 1) {
                if ((nn >= '0') && (nn <= '9')) {
                    str[mm] = nn;
                    printf("%c", str[mm]);
                    mm++;
                }
            } else {
                str[mm] = nn;
                //printf("%c", str[mm]);
                printf("*");
                mm++;
            }
        }
    } while ((nn != 10) && (nn != 13));
    printf("\n");
    str[mm] = '\0';
    //strncpy(str1, str, mm);
    return str;
}
//----------------------------------------------------------------------------
std::string encryptChars(char *str) {
    char mask = (sin(3.14) * 400) - (cos(3.14) * 250 / 20);
    for (size_t i = 0; i < (strlen(str)); i++)
        str[i] = (str[i] ^ mask);
    return str;
}
//----------------------------------------------------------------------------
void encryptChars(std::string &str) {
    char mask = (sin(3.14) * 400) - (cos(3.14) * 250 / 20);
    for (size_t i = 0; i < (str.size()); i++)
        str[i] = (str[i] ^ mask);
}
//----------------------------------------------------------------------------
std::vector<std::string> split(const std::string &s, char delimiter = '#') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
//----------------------------------------------------------------------------
std::string toString(std::vector<std::string> a, std::string text = "") {
    for (auto it: a) {
        text += "#" + it;
    }
    return text;
}
//----------------------------------------------------------------------------
void sendMenu(SOCKET Connection, std::vector<std::string> a) {
    char msg1[300];
    for (auto it: a) {
        strcpy(msg1, it.c_str());
        send(Connection, msg1, sizeof(msg1), 0);
    }
    strcpy(msg1, "end");
    send(Connection, msg1, sizeof(msg1), 0);
}
//----------------------------------------------------------------------------
std::vector<std::string> takeMenu(SOCKET Connection) {
    char msg[300];
    recv(Connection, msg, sizeof(msg), 0);
    std::string str = std::string(msg);
    return split(str, '#');
}
//----------------------------------------------------------------------------
int vcChoice(const std::string &strMenu, std::vector<std::string> vc, bool back = false) {
    std::cout << "Выберите " + strMenu + ": " << std::endl;
    for (size_t i = 0; i < vc.size(); ++i) {
        std::cout << std::setw(2) << i + 1 << ". " << vc[i] << std::endl;
    }
    if (back) std::cout << "0. -= ОТМЕНА =- " << std::endl;
    int ch = -1;
    do {
        std::cin >> ch;
        fflush(stdin);
    } while ((ch < 0) || (ch > vc.size()));
    return ch;
}
//----------------------------------------------------------------------------
std::wstring str_to_wstr(const std::string& s, const unsigned cp)
{
    std::wstring res;
    unsigned length =
        MultiByteToWideChar
        (
            cp, //CodePage
            0, //dwFlags
            s.c_str(), //lpMultiByteStr
            -1, //cchMultiByte
            0, //lpWideCharStr
            0 //cchWideChar
        );
    wchar_t* buffer = new wchar_t[length];
    if
        (
            MultiByteToWideChar
            (
                cp, //CodePage
                0, //dwFlags
                s.c_str(), //lpMultiByteStr
                -1, //cchMultiByte
                buffer, //lpWideCharStr
                length //cchWideChar
            )
            )
        res = buffer;
    delete[] buffer;
    return res;
}
//----------------------------------------------------------------------------
std::string wstr_to_str(const std::wstring& s, const unsigned cp)
{
    std::string res;
    unsigned length =
        WideCharToMultiByte
        (
            cp, //CodePage
            0, //dwFlags
            s.c_str(), //lpWideCharStr
            -1, //cchWideChar
            0, //lpMultiByteStr
            0, //cchMultiByte
            0, //lpDefaultChar
            0 //lpUsedDefaultChar
        );
    char* buffer = new char[length];
    if
        (
            WideCharToMultiByte
            (
                cp, //CodePage
                0, //dwFlags
                s.c_str(), //lpWideCharStr
                -1, //cchWideChar
                buffer, //lpMultiByteStr
                length, //cchMultiByte
                0, //lpDefaultChar
                0 //lpUsedDefaultChar
            )
            )
        res = buffer;
    delete[] buffer;
    return res;
}
//----------------------------------------------------------------------------
std::string acp(const std::string& s)
{
    return wstr_to_str(str_to_wstr(s, CP_UTF8), CP_ACP);
}
//----------------------------------------------------------------------------

#endif //SRV_STDAFX_H