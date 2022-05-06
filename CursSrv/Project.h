#ifndef CURSSRV_PROJECT_H
#define CURSSRV_PROJECT_H

#include <utility>
#include <ostream>
#include <WinSock2.h>

#include "../Utils/stdafx.h"
#include "Date.h"

using namespace std;
using namespace Checks;

class Project {
private:
    size_t project_id;
    std::string project_name;
    size_t sum_credit;
    size_t credit_time;
    std::string sud_reestr;
    Date application_date{};
    size_t company_id;
public:

    Project() {
        project_id = 0;
        sum_credit = 0;
        credit_time = 0;
        application_date = {};
        company_id = 0;
    }

    Project(size_t _projectId, std::string _projectName, size_t _sumCredit, size_t _creditTime,
            std::string _sudReestr, const Date &_applicationDate, size_t _companyId) : project_id(_projectId),
                                                                                       project_name(
                                                                                               std::move(_projectName)),
                                                                                       sum_credit(_sumCredit),
                                                                                       credit_time(_creditTime),
                                                                                       sud_reestr(
                                                                                               std::move(_sudReestr)),
                                                                                       application_date(
                                                                                               _applicationDate),
                                                                                       company_id(_companyId) {}

    size_t getProjectId() const {
        return project_id;
    }

    const std::string &getProjectName() const {
        return project_name;
    }

    double getSumCredit() const {
        return sum_credit;
    }

    size_t getCreditTime() const {
        return credit_time;
    }

    const std::string &getSudReestr() const {
        return sud_reestr;
    }

    const Date &getApplicationDate() const {
        return application_date;
    }

    size_t getCompanyId() const {
        return company_id;
    }

    void setProjectId(size_t _projectId) {
        project_id = _projectId;
    }

    void setProjectName(const std::string &_projectName) {
        project_name = _projectName;
    }

    void setSumCredit(size_t _sumCredit) {
        sum_credit = _sumCredit;
    }


    void setCreditTime(size_t _creditTime) {
        credit_time = _creditTime;
    }


    void setSudReestr(const std::string &_sudReestr) {
        sud_reestr = _sudReestr;
    }

    void setApplicationDate(const Date &_applicationDate) {
        application_date = _applicationDate;
    }

    void setCompanyId(size_t _companyId) {
        company_id = _companyId;
    }

    bool operator==(const Project &rhs) const {
        return project_id == rhs.project_id &&
               project_name == rhs.project_name &&
               sum_credit == rhs.sum_credit &&
               credit_time == rhs.credit_time &&
               sud_reestr == rhs.sud_reestr &&
               application_date == rhs.application_date &&
               company_id == rhs.company_id;
    }

    bool operator!=(const Project &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Project &project) {
        os << "project_id: " << project.project_id << " project_name: " << project.project_name << " sum_credit: "
           << project.sum_credit << " credit_time: " << project.credit_time << " sud_reestr: " << project.sud_reestr
           << " application_date: " << project.application_date << " company_id: " << project.company_id;
        return os;
    }

    void setProject(size_t _projectId, std::string _projectName, size_t _sumCredit, size_t _creditTime,
                    std::string _sudReestr, std::string _applicationDate, size_t _companyId) {
        project_id = _projectId;
        project_name = std::move(_projectName);
        sum_credit = _sumCredit;
        credit_time = _creditTime;
        sud_reestr = std::move(_sudReestr);
        application_date.setDateStr(_applicationDate);
        company_id = _companyId;
    }

    void setProject(const Project &rhs) {
        project_id = rhs.project_id;
        project_name = rhs.project_name;
        sum_credit = rhs.sum_credit;
        credit_time = rhs.credit_time;
        sud_reestr = rhs.sud_reestr;
        application_date = rhs.application_date;
        company_id = rhs.company_id;
    }

    static void enterProject(Project &tmp) {
        size_t _projectId, _creditTime, _companyId;
        size_t _sumCredit;
        std::string _projectName, _sudReestr, _sumCreditStr, _creditTimeStr, _companyIdStr;
        Date _applicationDate{};
        _projectId = 0;
        //������������ �������
        do {
            cout << "������������: ";
            cin >> _projectName;
            if (Checks::checkNoNumbers(_projectName)) {
                break;
            } else
                cout << "������������ ����. ��������� �������.\n";
        } while (true);
        do {
            cout << "����� �������: ";
            cin >> _sumCreditStr;
            if (Checks::checkNoLetters(_sumCreditStr)) {
                _sumCredit = stod(_sumCreditStr);
                break;
            } else
                cout << "������������ ����. ��������� �������.\n";
        } while (true);
        do {
            cout << "���� ������������: ";
            cin >> _creditTimeStr;
            if (Checks::checkNoLetters(_creditTimeStr)) {
                _creditTime = stol(_creditTimeStr);
                break;
            } else
                std::cout << "������������ ����. ��������� �������.\n";
        } while (true);
        do {
            cout << "�������� ������: ";
            cin >> _projectName;
            if (Checks::checkNoNumbers(_sudReestr)) {
                break;
            } else
                cout << "������������ ����. ��������� �������.\n";
        } while (true);
        cout << "���� ������ ������: ";
        _applicationDate.inputDate();
        //_applicationDate.getDateStr(); //std::string
        do {
            cout << "��� ��������: ";
            cin >> _companyIdStr;
            if (Checks::checkNoLetters(_companyIdStr)) {
                _companyId = stol(_companyIdStr);
                break;
            }
            else
                cout << "������������ ����. ��������� �������.\n";
        } while (true);
        tmp.setProject(_projectId, _projectName, _sumCredit, _creditTime, _sudReestr, _applicationDate.getDateStr(), _companyId);
    }
};

class ProjectSock : public Project {
public:
    SOCKET sock;

    explicit ProjectSock(const SOCKET &sock) : sock(sock) {}

    Date inputDateSock() const {
        Date tmp{};
        std::string _year, _month, _day;
        do {
            sendString(sock, "���: ");
            do {
                _year = takeString(sock);
                if (Checks::checkNoLetters(_year)) {
                    tmp.year = stol(_year);
                    if(Date::CheckYear(tmp.year) == 0) sendString(sock, "�������� ���! ��������� ����.\n���: ");
                    else break;
                } else
                    sendString(sock, "�������� ���! ��������� ����.\n���: ");
            } while (true);
            break;
        } while (true);
        size_t max_day;
        do {
            std::cout << "�����: ";
            Checks::CheckInput(tmp.month);
            max_day = Date::CheckMon(tmp.year, tmp.month);
        } while (max_day == 0);
        do {
            std::cout << "����: ";
            Checks::CheckInput(tmp.day);
            tmp.day = Date::CheckDay(max_day, tmp.day);
        } while (tmp.day == 0);
    }

    void enterProject() {
        size_t _projectId, _creditTime, _companyId;
        size_t _sumCredit;
        std::string _projectName, _sudReestr, _sumCreditStr, _creditTimeStr, _companyIdStr;
        Date _applicationDate{};
        _projectId = 0;
        sendString(sock, "data");
        //������������ �������
        sendString(sock, "������������ �������: ");
        do {
            _projectName = takeString(sock);
            if (Checks::checkNoNumbers(_projectName)) {
                break;
            } else
                sendString(sock, "������������ ����. ��������� �������.\n������������: ");
        } while (true);
        sendString(sock, "����� �������: ");
        do {
            _sumCreditStr = takeString(sock);
            if (Checks::checkNoLetters(_sumCreditStr)) {
                _sumCredit = stod(_sumCreditStr);
                break;
            } else
                sendString(sock, "������������ ����. ��������� �������.\n����� �������: ");
        } while (true);
        sendString(sock, "���� ������������: ");
        do {
            _creditTimeStr = takeString(sock);
            if (Checks::checkNoLetters(_creditTimeStr)) {
                _creditTime = stol(_creditTimeStr);
                break;
            } else
                sendString(sock, "������������ ����. ��������� �������.\n���� ������������: ");
        } while (true);
        sendString(sock, "�������� ������: ");
        do {
            _projectName = takeString(sock);
            if (Checks::checkNoNumbers(_sudReestr)) {
                break;
            } else
                sendString(sock, "������������ ����. ��������� �������.\n�������� ������: ");
        } while (true);
        //sendString(sock, "���� ������ ������: ");
        //_applicationDate.inputDate();
        _applicationDate.getDateStr(); //std::string
        sendString(sock, "������������ ��������: ");
        do {
            _companyIdStr = takeString(sock);
            if (Checks::checkNoNumbers(_companyIdStr)) {
                _companyId = stol(_companyIdStr);
                break;
            } else
                sendString(sock, "������������ ����. ��������� �������.\n������������ ��������: ");
        } while (true);
        sendString(sock, "end");
        setProject(_projectId, _projectName, _sumCredit, _creditTime, _sudReestr, _applicationDate.getDateStr(), _companyId);
    }
};


#endif //CURSSRV_PROJECT_H
