#ifndef CURSSRV_DATE_H
#define CURSSRV_DATE_H

#include <ostream>
#include <ctime>
#include "../Utils/stdafx.h"

class Date {
public:
    size_t year;       //���
    size_t month;      //�����
    size_t day;        //����

    static size_t CheckYear(size_t y) {
        if ((y > 2022) || (y < 2000)) {
            std::cout << "�������� ���! ��������� ����." << std::endl;
            return 0;
        }
        return y;
    }

    static size_t CheckMon(size_t y, size_t m) {
        size_t md;
        if ((m < 1) || (m > 12)) {
            std::cout << "�������� �����! ��������� ����." << std::endl;
            return 0;
        }
        switch (m) {
            case 1:
                md = 31;
                break;
            case 2:
                if (y % 4 == 0) md = 29;
                else md = 28;
                break;
            case 3:
                md = 31;
                break;
            case 4:
                md = 30;
                break;
            case 5:
                md = 31;
                break;
            case 6:
                md = 30;
                break;
            case 7:
                md = 31;
                break;
            case 8:
                md = 31;
                break;
            case 9:
                md = 30;
                break;
            case 10:
                md = 31;
                break;
            case 11:
                md = 30;
                break;
            case 12:
                md = 31;
                break;
            default:
                md = 0;
                break;
        }
        return md;
    }

    static size_t CheckDay(size_t md, size_t d) {
        if ((d > 0) && (d <= md)) return d;
        std::cout << "�������� ����! ��������� ����." << std::endl;
        return 0;
    }

    std::string getDateStr() const {
        std::string str;
        str = std::to_string(year);
        str += '/';
        if (month < 10) str += "0";
        str += std::to_string(month);
        str += '/';
        if (day < 10) str += "0";
        str += std::to_string(day);
        return str;
    }

    void setDateStr(std::string str) {
        year = std::stoi(str.substr(0, str.find("/")));
        month = std::stoi(str.substr(str.find("/") + 1, str.rfind("/") - str.find("/")));
        day = std::stoi(str.substr(str.rfind("/") + 1, 2));
    }

    void inputDate() {
        do {
            std::cout << "���: ";
            Checks::CheckInput(year);
        } while (CheckYear(year) == 0);
        size_t max_day;
        do {
            std::cout << "�����: ";
            Checks::CheckInput(month);
            max_day = CheckMon(year, month);
        } while (max_day == 0);
        do {
            std::cout << "����: ";
            Checks::CheckInput(day);
            day = CheckDay(max_day, day);
        } while (day == 0);
    }

    Date &operator=(const Date &S) = default;

    bool operator==(const Date &rhs) const {
        return year == rhs.year &&
               month == rhs.month &&
               day == rhs.day;
    }

    bool operator!=(const Date &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Date &date) {
        os << date.getDateStr();
        return os;
    }
    // ��������� ������� ���� � ������� YYYY/MM/DD
    static std::string currentDateTime() {
        time_t     now = time(nullptr);
        struct tm  tstruct{};
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
        return buf;
    }
};

#endif //CURSSRV_DATE_H