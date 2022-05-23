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
	float weight;
public:
	vector<string> Companies;

	Project() {
		project_id = 0;
		sum_credit = 0;
		credit_time = 0;
		application_date = {};
		company_id = 0;
		weight = 0;
	}

	explicit Project(vector<string> vc) {
		Companies = std::move(vc);
		project_id = 0;
		sum_credit = 0;
		credit_time = 0;
		application_date = {};
		company_id = 0;
		weight = 0;
	}

	Project(size_t _projectId, std::string _projectName, size_t _sumCredit, size_t _creditTime,
		std::string _sudReestr, const Date& _applicationDate, size_t _companyId) : project_id(_projectId),
		project_name(
			std::move(_projectName)),
		sum_credit(_sumCredit),
		credit_time(_creditTime),
		sud_reestr(
			std::move(_sudReestr)),
		application_date(
			_applicationDate),
		company_id(_companyId) {
		weight = 0;
	}

	size_t getProjectId() const {
		return project_id;
	}

	const std::string& getProjectName() const {
		return project_name;
	}

	size_t getSumCredit() const {
		return sum_credit;
	}

	size_t getCreditTime() const {
		return credit_time;
	}

	const std::string& getSudReestr() const {
		return sud_reestr;
	}

	const Date& getApplicationDate() const {
		return application_date;
	}

	size_t getCompanyId() const {
		return company_id;
	}

	float getWeight() const {
		return weight;
	}

	void setProjectId(size_t _projectId) {
		project_id = _projectId;
	}

	void setProjectName(const std::string& _projectName) {
		project_name = _projectName;
	}

	void setSumCredit(size_t _sumCredit) {
		sum_credit = _sumCredit;
	}

	void setCreditTime(size_t _creditTime) {
		credit_time = _creditTime;
	}

	void setSudReestr(const std::string& _sudReestr) {
		sud_reestr = _sudReestr;
	}

	void setApplicationDate(const Date& _applicationDate) {
		application_date = _applicationDate;
	}

	void setCompanyId(size_t _companyId) {
		company_id = _companyId;
	}

	void setWeight(float _weight) {
		weight = _weight;
	}

	bool operator==(const Project& rhs) const {
		return project_id == rhs.project_id &&
			project_name == rhs.project_name &&
			sum_credit == rhs.sum_credit &&
			credit_time == rhs.credit_time &&
			sud_reestr == rhs.sud_reestr &&
			application_date == rhs.application_date &&
			company_id == rhs.company_id &&
			weight == rhs.weight;
	}

	bool operator!=(const Project& rhs) const {
		return !(rhs == *this);
	}

	friend std::ostream& operator<<(std::ostream& os, const Project& project) {
		//os << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
		os << " project_id: " << project.project_id << endl;
		os << " project_name: " << project.project_name << endl;
		os << " sum_credit: " << project.sum_credit << endl;
		os << " credit_time: " << project.credit_time << endl;
		os << " sud_reestr: " << project.sud_reestr << endl;
		os << " application_date: " << project.application_date << endl;
		os << " company_id: " << project.company_id << endl;
		//os << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
		return os;
	}

	void setProject(size_t _projectId, std::string _projectName, size_t _sumCredit, size_t _creditTime,
		std::string _sudReestr, const std::string& _applicationDate, size_t _companyId) {
		project_id = _projectId;
		project_name = std::move(_projectName);
		sum_credit = _sumCredit;
		credit_time = _creditTime;
		sud_reestr = std::move(_sudReestr);
		application_date.setDateStr(_applicationDate);
		company_id = _companyId;
	}

	void setProject(const Project& rhs) {
		project_id = rhs.project_id;
		project_name = rhs.project_name;
		sum_credit = rhs.sum_credit;
		credit_time = rhs.credit_time;
		sud_reestr = rhs.sud_reestr;
		application_date = rhs.application_date;
		company_id = rhs.company_id;
		weight = rhs.weight;
	}

	static void enterProject(Project& tmp) {
		size_t _projectId, _creditTime, _companyId;
		size_t _sumCredit;
		std::string _projectName, _sudReestr, _sumCreditStr, _creditTimeStr, _companyIdStr;
		Date _applicationDate{};
		_projectId = 0;
		//Наименование проекта
		do {
			cout << "Наименование: ";
			cin >> _projectName;
			if (Checks::checkNoNumbers(_projectName)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Сумму кредита: ";
			cin >> _sumCreditStr;
			if (Checks::checkNoLetters(_sumCreditStr)) {
				_sumCredit = stol(_sumCreditStr);
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Срок кредитования: ";
			cin >> _creditTimeStr;
			if (Checks::checkNoLetters(_creditTimeStr)) {
				_creditTime = stol(_creditTimeStr);
				break;
			}
			else
				std::cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Судебный реестр: ";
			cin >> _projectName;
			if (Checks::checkNoNumbers(_sudReestr)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		cout << "Дата подачи заявки: ";
		_applicationDate.inputDate();
		//_applicationDate.getDateStr(); //std::string
		do {
			cout << "Код Компании: ";
			cin >> _companyIdStr;
			if (Checks::checkNoLetters(_companyIdStr)) {
				_companyId = stol(_companyIdStr);
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		tmp.setProject(_projectId, _projectName, _sumCredit, _creditTime, _sudReestr, _applicationDate.getDateStr(), _companyId);
	}

	// Вывод проекта c весами
	void printProject(const std::string& companyStr, bool head = false) {
		if (head) {
			// заголовок
			std::cout << "+----+----------------------------------------+-----------+-------+---+----------+-------------------+" << std::endl;
			std::cout << "| Id | Наименование проекта                   | Сумма     | Срок  |СР |  Дата    | Компания          |" << std::endl;
			std::cout << "+----+----------------------------------------+-----------+-------+---+----------+-------------------+" << std::endl;
		}
		std::cout << "|" << std::right << std::setw(3) << project_id << " |" << std::left << std::setw(40) << project_name << "|"
			<< std::setw(11) << sum_credit << "|" << std::setw(7) << credit_time << "|" << std::setw(3) << sud_reestr << "|" << application_date.getDateStr() << "|" << std::setw(19) << companyStr << "|" << std::endl;

		std::cout << "+----+----------------------------------------+-----------+-------+---+----------+-------------------+" << std::endl;

	}

	// Вывод проекта c весами
	void printProjectWeight(size_t flag, size_t head = 0) {
		if (head != 0) {
			// заголовок
			std::cout << "+----+----------------------------------------+------+" << std::endl;
			std::cout << "| Id | Наименование проекта                   | Вес  |" << std::endl;
			std::cout << "+----+----------------------------------------+------+" << std::endl;
		}
		if (flag == 2) {	// для ранжа
			std::cout << "|" << std::right << std::setw(3) << project_id << " |" << std::left << std::setw(40) << project_name << "|" << std::setw(6) << weight << "|" << std::endl;
			std::cout << "+----+----------------------------------------+------+" << std::endl;
		}
	}

	// Вывод проекта в сокет
	void printProjectSock(SOCKET sc, const std::string& companyStr, bool head = false) {
			if (head) {
				// заголовок
				sendString(sc, "+----+----------------------------------------+-----------+-------+---+----------+-------------------+\n");
				sendString(sc, "| Id | Наименование проекта                   | Сумма     | Срок  |СР |  Дата    | Компания          |\n");
				sendString(sc, "+----+----------------------------------------+-----------+-------+---+----------+-------------------+\n");
			}
			std::stringstream ss;
			ss << "|" << std::right << std::setw(3) << project_id << " |" << std::left << std::setw(40) << project_name << "|"
				<< std::setw(11) << sum_credit << "|" << std::setw(7) << credit_time << "|" << std::setw(3) << sud_reestr << "|" << application_date.getDateStr() << "|" << std::setw(19) << companyStr << "|" << std::endl;
			sendString(sc, ss.str());
			ss.str("");
			sendString(sc, "+----+----------------------------------------+-----------+-------+---+----------+-------------------+\n");
	}

	// Вывод проекта с весами в сокет
	void printProjectWeightSock(SOCKET sc, size_t flag, size_t head = 0) {
		if (head != 0) {
			// заголовок
			sendString(sc, "+----+----------------------------------------+------+\n");
			sendString(sc, "| Id | Наименование проекта                   | Вес  |\n");
			sendString(sc, "+----+----------------------------------------+------+\n");
		}
		std::stringstream ss;
		if (flag == 2) {	// для ранжа
			ss << "|" << std::right << std::setw(3) << project_id << " |" << std::left << std::setw(40) << project_name << "|" << std::setw(6) << weight << "|" << std::endl;
			sendString(sc, ss.str());
			sendString(sc, "+----+----------------------------------------+------+\n");
		}
	}

	//----------------------------------------------------------------------------
	/*static std::vector<std::string> toVector(std::map<std::string, Project> a) {
		std::vector<std::string> tmp;
		for (auto it : a) {
			tmp.push_back(it.first);
		}
		return tmp;
	}*/
};

// Сравнение по весу
bool compareWeight(const Project& obj1, const Project& obj2) {
	return (obj1.getWeight() < obj2.getWeight());
}

// Сравнение по project_id
bool compareProjectId(const Project& obj1, const Project& obj2) {
	return (obj1.getProjectId() > obj2.getProjectId());
}

class ProjectSock : public Project {
public:
	SOCKET sock;

	explicit ProjectSock(const SOCKET& _sock, std::vector<std::string> vc) : Project(std::move(vc)) {
		this->sock = _sock;
	}

	Date inputDateSock() const {
		Date tmp{};
		std::string _year, _month, _day;
		do {
			sendString(sock, "Год: ");
			do {
				_year = takeString(sock);
				if (Checks::checkNoLetters(_year)) {
					tmp.year = stol(_year);
					if (Date::CheckYear(tmp.year) == 0) sendString(sock, "Неверный год! Повторите ввод.\nГод: ");
					else break;
				}
				else
					sendString(sock, "Неверный год! Повторите ввод.\nГод: ");
			} while (true);
			break;
		} while (true);
		size_t max_day = 0;
		do {
			std::cout << "Месяц: ";
			Checks::CheckInput(tmp.month);
			max_day = Date::CheckMon(tmp.year, tmp.month);
		} while (max_day == 0);
		do {
			std::cout << "День: ";
			Checks::CheckInput(tmp.day);
			tmp.day = Date::CheckDay(max_day, tmp.day);
		} while (tmp.day == 0);
	}

	void enterProject() {
		size_t _projectId, _creditTime, _companyId;
		size_t _sumCredit;
		std::string _projectName, _sudReestr, _sumCreditStr, _creditTimeStr; // , _companyIdStr;
		Date _applicationDate{};
		_projectId = 0;
		sendString(sock, "data");

		//Наименование проекта
		sendString(sock, "Наименование проекта: ");
		do {
			_projectName = takeString(sock);
			if (Checks::checkNoNumbers(_projectName)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nНаименование: ");
		} while (true);

		sendString(sock, "Сумма кредита: ");
		do {
			_sumCreditStr = takeString(sock);
			if (Checks::checkNoLetters(_sumCreditStr)) {
				_sumCredit = stol(_sumCreditStr);
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nСумма кредита: ");
		} while (true);

		sendString(sock, "Срок кредитования: ");
		do {
			_creditTimeStr = takeString(sock);
			if (Checks::checkNoLetters(_creditTimeStr)) {
				_creditTime = stol(_creditTimeStr);
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nСрок кредитования: ");
		} while (true);

		/*sendString(sock, "Судебный реестр: ");
		do {
			_sudReestr = takeString(sock);
			if (Checks::checkNoNumbers(_sudReestr)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nСудебный реестр: ");
		} while (true);*/

		sendString(sock, "end");

		size_t ch;
		sendString(sock, "menu");
		sendString(sock, "Судебный реестр:#Нет#Да");
		ch = takeInt(sock);
		if (ch > 0) {
			switch (ch) {
			case 1:
				_sudReestr = "Нет";
				break;
			case 2:
				_sudReestr = "Да";
				break;
			default:
				break;
			}
		}
		else return;
		//sendString(sock, "Дата подачи заявки: ");
		//_applicationDate.inputDate();
		_applicationDate.setDateStr(Date::currentDate()); //std::string

		/*sendString(sock, "Наименование компании: ");
		do {
			_companyIdStr = takeString(sock);
			if (Checks::checkNoNumbers(_companyIdStr)) {
				_companyId = stol(_companyIdStr);
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nНаименование компании: ");
		} while (true);*/
		//sendString(sock, "end");

		//size_t ch = vcChoice("Компания", Companies);
		//size_t ch;
		sendString(sock, "menu");
		sendString(sock, toString(Companies));
		ch = takeInt(sock);
		if (ch > 0) _companyId = ch - 1;
		else return;
		setProject(_projectId, _projectName, _sumCredit, _creditTime, _sudReestr, _applicationDate.getDateStr(), _companyId);
	}

	friend std::ostream& operator<<(std::ostream& os, const ProjectSock& project) {
		//os << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
		os << " project_id: " << project.getProjectId() << endl;
		os << " project_name: " << project.getProjectName() << endl;
		os << " sum_credit: " << project.getSumCredit() << endl;
		os << " credit_time: " << project.getCreditTime() << endl;
		os << " sud_reestr: " << project.getSudReestr() << endl;
		os << " application_date: " << project.getApplicationDate().getDateStr() << endl;
		os << " company_id: " << project.getCompanyId() << endl;
		//os << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
		return os;
	}
};


#endif //CURSSRV_PROJECT_H
