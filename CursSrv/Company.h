#ifndef CURSSRV_COMPANY_H
#define CURSSRV_COMPANY_H

#include <ostream>
#include <utility>
#include <WinSock2.h>
#include "../Utils/stdafx.h"

using namespace std;
using namespace Checks;

class Company {
private:
	size_t id;
	std::string name;
	std::string activity;
	size_t finance;
public:

	Company() {
		id = 0;
		finance = 0;
	}

	Company(string _name, string _activity, size_t _finance) : id(0), name(std::move(_name)), activity(std::move(_activity)),
		finance(_finance) {}


	const string& getName() const {
		return name;
	}

	const string& getActivity() const {
		return activity;
	}

	size_t getFinance() const {
		return finance;
	}

	size_t getId() {
		return id;
	}


	void setId(size_t _id) {
		Company::id = _id;
	}

	void setName(const string& _name) {
		Company::name = _name;
	}

	void setActivity(const string& _activity) {
		Company::activity = _activity;
	}

	void setFinance(size_t _finance) {
		Company::finance = _finance;
	}

	void setCompany(std::string _name, std::string _activity, size_t _finance) {
		this->name = std::move(_name);
		this->activity = std::move(_activity);
		this->finance = _finance;
		//return *this;
	}

	void setCompany(const Company& rhs) {
		this->name = rhs.name;
		this->activity = rhs.activity;
		this->finance = rhs.finance;
		//return *this;
	}

	// Функция для просмотра сотрудника администратором
	void printCompany(bool one = false, ostream& fout = std::cout) const {
		//        if (one) {
		//            fout << "+-------------------------------------+------------------------+--------+-------------+----------+"
		//                 << endl;
		//        }
		//        fout << "|" << setw(37) << left << this->getFio() << "|"
		//             << setw(24) << left << this->getLogin() << "|"
		//             << setw(13) << this->getPass() << "|"
		//             << setw(13) << this->getRole() << "|" << endl;
		//        fout << "+-------------------------------------+------------------------+--------+-------------+----------+"
		//             << endl;
	}

	static void enterCompany(Company& tmp) {
		std::string _name, _activity, _financeStr;
		size_t _finance;
		//Наименование компании
		do {
			cout << "Наименование: ";
			cin >> _name;
			if (Checks::checkNoNumbers(_name)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Вид деятельности: ";
			cin >> _activity;
			if (Checks::checkNoNumbers(_activity)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Финансовое состояние: ";
			cin >> _financeStr;
			if (Checks::checkNoLetters(_financeStr)) {
				_finance = stol(_financeStr);
				break;
			}
			else
				std::cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		tmp.setCompany(_name, _activity, _finance);
	}

	friend ostream& operator<<(ostream& os, const Company& company) {
		os << "Компания: " << company.name << endl;
		os << "Вид деятельности: " << company.activity << endl;
		os << "Финансовое состояние: " << company.finance << endl;;
		return os;
	}
};

class CompanySock : public Company {
public:
	SOCKET sock;

	explicit CompanySock(SOCKET _sock) {
		this->sock = _sock;
	}

	void enterCompany() {
		//        CompanySock &tmp
		std::string _name, _activity, _financeStr;
		size_t _finance;
		sendString(sock, "data");
		//Наименование компании
		sendString(sock, "Наименование: ");
		_name = takeString(sock);
		sendString(sock, "Вид деятельности: ");
		do {
			_activity = takeString(sock);
			if (Checks::checkNoNumbers(_activity)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nВид деятельности: ");
		} while (true);
		sendString(sock, "Финансовое состояние: ");
		do {
			_financeStr = takeString(sock);
			if (Checks::checkNoLetters(_financeStr)) {
				_finance = stol(_financeStr);
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nФинансовое состояние: ");
		} while (true);
		sendString(sock, "end");
		setCompany(_name, _activity, _finance);
		//tmp.setCompany(_name, _activity, _finance);
	}

};

#endif //CURSSRV_COMPANY_H
