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
	std::string pass;       // Пароль
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
		this->uid = rhs.uid;
		this->name = rhs.name;
		this->login = rhs.login;
		this->pass = rhs.pass;
		this->role = rhs.role;
		//return *this;
	}

	bool isEmpty() {
		return (login.empty());
	}

	bool isEmptyId() const {
		return (uid == 0);
	}

	// Очистка
	void clear() {
		uid = 0;
		name = "";
		login = "";
		pass = "";
		role = "";
	}

	// Функция для просмотра сотрудника администратором
	void printUser(bool one = false, ostream& fout = std::cout) {
		if (one) {
			fout << "+-------------------------------------+------------------------+-------------+----------------------+" << endl;
			fout << "| Фамилия Имя                         | Логин                  | Пароль      | Роль                 |" << endl;
			fout << "+-------------------------------------+------------------------+-------------+----------------------+" << endl;
		}
		fout << "|" << setw(37) << left << getName() << "|"
			<< setw(24) << left << login << "|"
			<< setw(13) << pass << "|"
			<< setw(22) << role << "|" << endl;
		fout << "+-------------------------------------+------------------------+-------------+----------------------+"
			<< endl;
	}

	// Проверка существования логина
	static bool checkExistLogin(const std::vector<std::string>& vc, const std::string& str) {
		for (const auto& it : vc) {
			if (it == str) {
				return true;
			}
		}
		return false;
//        return std::ranges::any_of(vc.cbegin(), vc.cend(), str);
	}

	virtual void enterUser(const std::vector<std::string>& existLogins) {
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
			//std::cout << "Логин " << str << "существует! Попробуйте другой." << std::endl;
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

	UserSock() {
		sock = NULL;
	}

	UserSock(SOCKET _sock, std::vector<std::string> vc) : User(std::move(vc)) {
		this->sock = _sock;
	}

	void setSock(SOCKET sc) {
		sock = sc;
	}

	//

	//- Редактирование Фамилии Имени -
	void editName(const std::string& oldStr) {
		std::string _name, _name2;
		//-----------------------------------
		sendString(sock, "Старое значение: " + oldStr + "\nФамилия: ");
		do {
			_name = takeString(sock);
			if (Checks::checkNoNumbers(_name)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nФамилия: ");
		} while (true);
		sendString(sock, "Имя: ");
		do {
			_name2 = takeString(sock);
			if (Checks::checkNoNumbers(_name2)) {
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nИмя: ");
		} while (true);
		_name += " " + _name2;
		setName(_name);
	}

	//- Редактирование Логина -
	void editLogin(const std::vector<std::string>& vc, const std::string& oldStr) {
		std::string _login;
		sendString(sock, "Логин (" + oldStr + "): ");
		do {
			_login = takeString(sock);
			if (checkExistLogin(vc, _login)) {
				sendString(sock, "Логин - " + _login + " существует! Попробуйте еще раз!\nЛогин: ");
			} else {
                break;
            }
		} while (true);
		setLogin(_login);
	}

	//- Редактирование пароля -
	void editPass(const std::string& oldStr) {
		std::string _pass;
		sendString(sock, "Пароль (" + oldStr + "): ");
		_pass = takeString(sock);
		setPass(encryptChars(_pass));
	}

	//- Редактирование роли -
	void editRole(const std::string& oldStr) {
		std::string _role;
		size_t ch;
		sendString(sock, "Старое значение: " + oldStr);
		sendString(sock, "end");
		sendString(sock, "menu");
		sendString(sock, toString(Roles));
		ch = takeInt(sock);
		if (ch > 0) _role = Roles[ch - 1];
		else return;
		setRole(_role);
	}

	//- Редактирование пользователя -
	void editUserSock(const std::vector<std::string>& existLogins, size_t mode = 4) {
		sendString(sock, "data");
		switch (mode)
		{
		case 1:
			editName(getName());
			break;
		case 2:
			editLogin(existLogins, getLogin());
			break;
		case 3:
			editPass(encryptChars(getPass()));
			break;
		case 4:
			editName(getName());
			editLogin(existLogins, getLogin());
			editPass(getPass());
			break;
		default:
			break;
		}
		sendString(sock, "end");
	}

	//- Ввод нового пользователя ----
	void enterUser(const std::vector<std::string>& existLogins) override {
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
		do {
			_login = takeString(sock);
			if (checkExistLogin(existLogins, _login)) {
				sendString(sock, "Логин - " + _login + " существует! Попробуйте еще раз!\nЛогин: ");
			} else {
                break;
            }
		} while (true);

		//-----------------------------------
		sendString(sock, "Пароль: ");
		_pass = takeString(sock);
		//-----------------------------------
		sendString(sock, "end");
		size_t ch;
		sendString(sock, "menu");
		sendString(sock, toString(Roles));
		ch = takeInt(sock);
		if (ch > 0) _role = Roles[ch - 1];
		else return;
		setUser(_name, _login, encryptChars(_pass), _role);
		//tmp.setUser(_name, _surname, _login, encryptChars(_pass), _role);
	}

	//- Вывод пользователя в сокет --
	static void printUserSock(SOCKET _sock, User& us, /*std::string fout = "",*/ bool one = false) {
		std::stringstream ss;
		//sendString(_sock, "output" + fout);
		if (one) {
			sendString(_sock, "+-------------------------------------+------------------------+-------------+----------------------+\n");
			sendString(_sock, "| Фамилия Имя                         | Логин                  | Пароль      | Роль                 |\n");
			sendString(_sock, "+-------------------------------------+------------------------+-------------+----------------------+\n");
		}
		ss << "|" << setw(37) << left << us.getName() << "|"
			<< setw(24) << left << us.getLogin() << "|"
			<< setw(13) << encryptChars(us.getPass()) << "|"
			<< setw(22) << us.getRole() << "|" << endl;
		sendString(_sock, ss.str());
		ss.str("");
		sendString(_sock, "+-------------------------------------+------------------------+-------------+----------------------+\n");
		//sendString(_sock, "end");
	}

	friend ostream& operator<<(ostream& out, UserSock& obj) {
		out << "Фамилия Имя: " << obj.getName() << endl;
		out << "Логин: " << obj.getLogin() << endl;
		out << "Пароль: " << obj.getPass() << endl;
		out << "Роль: " << obj.getRole() << endl;
		return out;
	}
};

#endif //CURSSRV_USER_H
