#ifndef CURSSRV_MARK_H
#define CURSSRV_MARK_H

#include <ostream>
#include <winsock.h>
#include "../Utils/stdafx.h"

using namespace std;
using namespace Checks;

class Mark : public User {
private:
	size_t mark_id;		// ID Оценки
	size_t number;		// # Ранжирования
	size_t user_id;		// ID эксперта
	size_t project1_id;	// Проект 1
	size_t project2_id;	// Проект 2
	float value1;		// Оценка проекту 1
	float value2;		// Оценка проекту 2
public:
	//map<string, size_t> mpUsers;
	//map<string, size_t> mpProjects;

	Mark() {
		mark_id = 0;
		number = 0;
		user_id = 0;
		project1_id = 0;
		project2_id = 0;
		value1 = 0;
		value2 = 0;
	}

	//Mark(std::map<std::string, size_t> mpU, std::map<string, size_t> mpP) {
	//	mpUsers = std::move(mpU);
	//	mpProjects = std::move(mpP);
	//	mark_id = 0;
	//	number = 0;
	//	user_id = 0;
	//	project1_id = 0;
	//	project2_id = 0;
	//	value1 = 0;
	//	value2 = 0;
	//}

	Mark(size_t markId, size_t _number, size_t userId, size_t project1Id, size_t project2Id, float _value) : mark_id(
		markId), number(_number), user_id(userId), project1_id(project1Id), project2_id(project2Id), value1(_value), value2(1 - _value) {}

	size_t getMarkId() const {
		return mark_id;
	}

	size_t getNumber() const {
		return number;
	}

	size_t getUserId() const {
		return user_id;
	}

	size_t getProject1Id() const {
		return project1_id;
	}

	size_t getProject2Id() const {
		return project2_id;
	}

	float getValue1() const {
		return value1;
	}

	float getValue2() const {
		return value2;
	}

	void setMarkId(size_t markId) {
		mark_id = markId;
	}

	void setNumber(size_t _number) {
		number = _number;
	}

	void setUserId(size_t userId) {
		user_id = userId;
	}

	void setProject1Id(size_t project1Id) {
		project1_id = project1Id;
	}

	void setProject2Id(size_t project2Id) {
		project2_id = project2Id;
	}

	void setValue(float _mark) {
		value1 = _mark;
		value2 = 1 - _mark;
	}

	void setValues(float _mark1, float _mark2) {
		value1 = _mark1;
		value2 = _mark2;
	}

	void setValues(std::pair<float, float> _mark) {
		value1 = _mark.first;
		value2 = _mark.second;
	}

	void clear() {
		mark_id = 0;
		number = 0;
		user_id = 0;
		project1_id = 0;
		project2_id = 0;
		value1 = 0;
		value2 = 0;
	}

	Mark& operator=(const Mark& M) = default;

	bool operator==(const Mark& rhs) const {
		return mark_id == rhs.mark_id &&
			number == rhs.number &&
			user_id == rhs.user_id &&
			project1_id == rhs.project1_id &&
			project2_id == rhs.project2_id &&
			value1 == rhs.value1;
	}

	bool operator!=(const Mark& rhs) const {
		return !(rhs == *this);
	}

	Mark& operator+= (Mark const& rhs) {
		project1_id = rhs.getProject1Id();
		project2_id = rhs.getProject2Id();
		value1 += rhs.getValue1();
		value2 += rhs.getValue2();
		return *this;
	}

	void setMark(size_t markId, size_t _number, size_t userId, size_t project1Id, size_t project2Id, float _value) {
		mark_id = markId;
		number = _number;
		user_id = userId;
		project1_id = project1Id;
		project2_id = project2Id;
		value1 = _value;
		value2 = 1 - _value;
	}

	void setMark(Mark& tmp) {
		mark_id = tmp.mark_id;
		number = tmp.number;
		user_id = tmp.user_id;
		project1_id = tmp.project1_id;
		project2_id = tmp.project2_id;
		value1 = tmp.value1;
		value2 = tmp.value2;
	}

	friend std::ostream& operator<<(std::ostream& os, const Mark& mark) {
		os << "- - - - - - - - -  - - - - - - -" << std::endl;
		os << " mark_id: " << mark.mark_id << std::endl;
		os << " number: " << mark.number << std::endl;
		os << " user_id: " << mark.user_id << std::endl;
		os << " project1_id: " << mark.project1_id << std::endl;
		os << " project2_id: " << mark.project2_id << std::endl;
		os << " value1: " << mark.value1 << std::endl;
		os << " value2: " << mark.value2 << std::endl;
		return os;
	}

	static void enterMark(Mark& tmp) {
		size_t markId;
		std::string _valueStr;
		if (tmp.getMarkId() != 0) markId = tmp.getMarkId();
		else markId = 0;
		float _value;
		// получение
		// NumberMark авто из БД
		// expert_Id из БД (текущий пользователь)
		// project1_id и project2_id выбор из БД
		do {
			cout << "Оценку: ";
			cin >> _valueStr;
			if (Checks::checkNoLetters(_valueStr)) {
				_value = std::stof(_valueStr);
				if (_value < 0 || _value>1) {
					cout << "Некорректный ввод. Повторите попытку.\n";
					continue;
				}
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		tmp.setMark(markId, tmp.getNumber(), tmp.getUserId(), tmp.getProject1Id(), tmp.getProject2Id(), _value);
	}
};

class MarkSock : public Mark {
public:
	SOCKET sock;

	explicit MarkSock(const SOCKET& _sock) {
		this->sock = _sock;
	}

	void enterMark(size_t _number, size_t userId, size_t project1Id, size_t project2Id) {
		std::string _valueStr;
		float _value;

		// получение
		// Number авто из БД
		// expert_Id из БД (текущий пользователь)
		// project1_id и project2_id выбор из БД
		// ----- Выбор эксперта -----
		std::string str = "Оценкa(" + to_string(project1Id);
		str += " <-> " + to_string(project2Id);
		str += "): ";
		sendString(sock, "data");
		sendString(sock, str);
		do {
			_valueStr = takeString(sock);
			if (Checks::checkNoLetters(_valueStr)) {
				_value = std::stof(_valueStr);
				if (_value < 0 || _value>1) {
					/*sendString(sock, "end");
					sendString(sock, "output");
					sendString(sock, "Некорректный ввод. Повторите попытку.\nОценка должна быть от 0 до 1.\n");
					sendString(sock, "data");
					sendString(sock, str);*/

					sendString(sock, "Некорректный ввод. Повторите попытку.\nОценка должна быть от 0 до 1.\n" + str);
					continue;
				}
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\nОценкуа ");
		} while (true);
		sendString(sock, "end");
		setMark(0, _number, userId, project1Id, project2Id, _value);
	}

	void enterMarkAll(std::map<std::string, size_t> mpUsers, std::map<string, size_t> mpProjects) {
		size_t userId, project1Id, project2Id;
		std::string userIdStr, project1IdStr, project2IdStr, _valueStr;
		//float _value = 0;

		// ----- Выбор эксперта -----
		vector<string> vc = toVector(mpUsers);
		sendString(sock, "menu");
		sendString(sock, toString(vc));
		size_t ch = takeInt(sock);
		if (ch > 0) userId = mpUsers[vc[ch - 1]];
		else return;
		vc.clear();

		// ----- Выбор проекта -----
		vc = toVector(mpProjects);
		sendString(sock, "menu");
		sendString(sock, toString(vc));
		ch = takeInt(sock);
		if (ch > 0) project1Id = mpProjects[vc[ch - 1]];
		else return;
		auto iter = vc.cbegin(); // указатель на первый элемент
		vc.erase(iter + (ch - 1));

		sendString(sock, "menu");
		sendString(sock, toString(vc));
		ch = takeInt(sock);
		if (ch > 0) project2Id = mpProjects[vc[ch - 1]];
		else return;

		sendString(sock, "end");
		/*sendString(sock, "data");
		sendString(sock, "Номер ранжа: ");
		do {
			numberStr = takeString(sock);
			if (Checks::checkNoLetters(_valueStr)) {
				_number = std::stol(numberStr);
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\nНомер ранжа: ";
		} while (true);
		sendString(sock, "Оценку: ");
		do {
			_valueStr = takeString(sock);
			if (Checks::checkNoLetters(_valueStr)) {
				_value = std::stof(_valueStr);
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\nОценка: ";
		} while (true);
		sendString(sock, "end");*/

		enterMark(getNumber(), userId, project1Id, project2Id);
		//setMark(0, _number, userId, project1Id, project2Id, _value);
	}

	void enterMark(size_t _number, const std::pair<std::string, size_t>& userId, size_t project1Id, size_t project2Id) {
		std::string userIdStr, project1IdStr, project2IdStr, _valueStr;
		float _value;
		sendString(sock, "data");

		std::string str = "Эксперт " + userId.first;
		str += ". Оценкa(" + to_string(project1Id);
		str += " <-> " + to_string(project2Id);
		str += ") : ";
		sendString(sock, str);
		do {
			_valueStr = takeString(sock);
			if (Checks::checkNoLetters(_valueStr)) {
				_value = std::stof(_valueStr);
				if (_value < 0 || _value>1) {
					/*sendString(sock, "end");
					sendString(sock, "output");
					sendString(sock, "Некорректный ввод. Повторите попытку.\nОценка должна быть от 0 до 1.\n");
					sendString(sock, "data");
					sendString(sock, str);*/

					sendString(sock, "Некорректный ввод. Повторите попытку.\nОценка должна быть от 0 до 1.\n" + str);
					continue;
				}
				break;
			}
			else
				sendString(sock, "Некорректный ввод. Повторите попытку.\n" + str);
		} while (true);
		sendString(sock, "end");
		setMark(0, _number, userId.second, project1Id, project2Id, _value);
	}

	static Mark toMark(MarkSock& tmp) {
		Mark mrk;
		mrk.setMarkId(tmp.getMarkId());
		mrk.setNumber(tmp.getNumber());
		mrk.setUserId(tmp.getUserId());
		mrk.setProject1Id(tmp.getProject1Id());
		mrk.setProject2Id(tmp.getProject2Id());
		mrk.setValue(tmp.getValue1());
		return mrk;
	}

	friend std::ostream& operator<<(std::ostream& os, const MarkSock& mark) {
		os << "- - - - - - - - -  - - - - - - -" << std::endl;
		os << " mark_id: " << mark.getMarkId() << std::endl;
		os << " number: " << mark.getNumber() << std::endl;
		os << " user_id: " << mark.getUserId() << std::endl;
		os << " project1_id: " << mark.getProject1Id() << std::endl;
		os << " project2_id: " << mark.getProject2Id() << std::endl;
		os << " value1: " << mark.getValue1() << std::endl;
		os << " value2: " << mark.getValue2() << std::endl;
		return os;
	}
};

#endif //CURSSRV_MARK_H
