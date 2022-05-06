#ifndef CURSSRV_USER_H
#define CURSSRV_USER_H

#include <utility>

#include "../Utils/stdafx.h"

using namespace std;
using namespace Checks;

class User {
private:
	size_t uid;				// Id
	std::string name;		// Фамилия Имя
	std::string login;      // Логин
	std::string pass;       // Телефон
	std::string role;       // Роль
public:
	vector<string> Roles;// = { "Администратор", "Представитель компании", "Эксперт" };

	friend ostream& operator<<(ostream& out, User& obj) {
		out << "Фамилия Имя: " << obj.getName() << endl;
		out << "Логин: " << obj.getLogin() << endl;
		out << "Пароль: " << obj.getPass() << endl;
		out << "Роль: " << obj.getRole() << endl;
		return out;
	}

	User() {
		uid = 0;
	};

	explicit User(vector<string> vc) {
		Roles = std::move(vc);
		uid = 0;
	}

	User(string _name, string _login, string _pass,
		string _role) : name(std::move(_name)),
		login(std::move(_login)),
		pass(std::move(_pass)),
		role(std::move(_role)) {
		uid = 0;
	}

	virtual ~User() = default;

	size_t getUid() const {
		return uid;
	}

	const string& getName() {
		return name;
	}

	string getLogin() {
		return login;
	}

	string getPass() {
		return pass;
	}

	string getRole() {
		return role;
	}

	void setUid(const size_t& _uid) {
		User::uid = _uid;
	}

	void setName(const string& _name) {
		User::name = _name;
	}

	void setLogin(const string& _login) {
		User::login = _login;
	}

	void setPass(const string& _pass) {
		User::pass = _pass;
	}

	void setRole(const string& _role) {
		User::role = _role;
	}

	void setUser(string _name, string _login, string _pass, string _role) {
		this->name = std::move(_name);
		this->login = std::move(_login);
		this->pass = std::move(_pass);
		this->role = std::move(_role);
		//return *this;
	}

	void setUser(const User& rhs) {
		this->name = rhs.name;
		this->login = rhs.login;
		this->pass = rhs.pass;
		this->role = rhs.role;
		//return *this;
	}

	// Функция для просмотра сотрудника администратором
	void printUser(bool one = false, ostream& fout = std::cout) {
		if (one) {
			fout << "+-------------------------------------+------------------------+--------+-------------+----------+"
				<< endl;
		}
		fout << "|" << setw(37) << left << getName() << "|"
			<< setw(24) << left << login << "|"
			<< setw(13) << pass << "|"
			<< setw(13) << role << "|" << endl;
		fout << "+-------------------------------------+------------------------+--------+-------------+----------+"
			<< endl;
	}

    virtual void enterUser() {
		//User tmp;
		string _name, _login, _pass, _role;
		//ФИО
		cout << "Введите ФИО сотрудника:\n";
		do {
			cout << "Фамилия Имя: ";
			cin >> _name;
			if (Checks::checkNoNumbers(_name)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		do {
			cout << "Логин: ";
			cin >> _login;
			if (Checks::checkNoNumbers(_login)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		//Номер телефона
		do {
			cout << "Введите номер телефона:\n";
			cin >> _pass;
			if (Checks::checkNoLetters(_pass)) {
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		//Роль
		size_t ch = vcChoice("Роль", Roles);
		if (ch > 0) _role = Roles[ch - 1];
		else return;
		setUser(_name, _login, _pass, _role);
	}

	bool operator==(const User& rhs) const {
		return name == rhs.name &&
			login == rhs.login &&
			pass == rhs.pass &&
			role == rhs.role;
	}

	bool operator!=(const User& rhs) const {
		return !(rhs == *this);
	}

	bool operator<(const User& rhs) const {
		if (name < rhs.name)
			return true;
		return (rhs.login < login);
	}

	bool operator>(const User& rhs) const {
		return rhs < *this;
	}

	bool operator<=(const User& rhs) const {
		return !(rhs < *this);
	}

	bool operator>=(const User& rhs) const {
		return !(*this < rhs);
	}
};

// Сравнение по фамилии
bool compareName(User obj1, User obj2) {
	if (strcmp(obj1.getName().c_str(), obj2.getName().c_str()) < 0)
		return true;
	else
		return false;
}

class UserSock : public User {
private:
	//SOCKET sock;
public:
	SOCKET sock;

	UserSock(SOCKET _sock, std::vector<std::string> vc) : User(std::move(vc)) {
		this->sock = _sock;
	}

	void enterUser() override {
		//UserSock tmp;
		string _name, _login, _pass, _role;
		sendString(sock, "data");
		//-----------------------------------
		sendString(sock, "Фамилия: ");
		do {
			_name = takeString(sock);
			if (Checks::checkNoNumbers(_name)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nФамилия: ");
		} while (true);
		sendString(sock, "Имя: ");
		std::string _name2;
		do {
			_name2 = takeString(sock);
			if (Checks::checkNoNumbers(_name2)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nИмя: ");
		} while (true);
		_name += " " + _name2;
		//-----------------------------------
		sendString(sock, "Логин: ");
		_login = takeString(sock);
		//do {
		//	_login = takeString(sock);
		//	if (Checks::checkNoNumbers(_login)) {
		//		break;
		//	}
		//	else
		//		sendString(sock, "Некорректный ввод. Повторите попытку.\nЛогин: ");
		//} while (true);
		//-----------------------------------
		sendString(sock, "Пароль: ");
		do {
			_pass = takeString(sock);
			if (Checks::checkNoLetters(_pass)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nПароль: ");
		} while (true);
		//-----------------------------------
		sendString(sock, "end");
		//size_t ch = vcChoice("Роль", Roles);
		size_t ch;
		sendString(sock, "menu");
		sendString(sock, toString(Roles));
		ch = takeInt(sock);
		if (ch > 0) _role = Roles[ch - 1];
		else return;
		//setlocale(LC_ALL, ".1251");
		//sendString(sock, "end");
		setUser(_name, _login, _pass, _role);
		//tmp.setUser(_name, _surname, _login, _pass, _role);
	}
};

#endif //CURSSRV_USER_H
