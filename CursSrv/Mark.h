#ifndef CURSSRV_MARK_H
#define CURSSRV_MARK_H

#include <ostream>
#include <winsock.h>
#include "../Utils/stdafx.h"

using namespace std;
using namespace Checks;

class Mark {
private:
	size_t number_mark;
	size_t user_id;
	size_t project1_id;
	size_t project2_id;
	float value1;
	float value2;
public:
	Mark() {
		number_mark = 0;
		user_id = 0;
		project1_id = 0;
		project2_id = 0;
		value1 = 0;
		value2 = 0;
	}

	Mark(size_t numberMark, size_t userId, size_t project1Id, size_t project2Id, float _value) : number_mark(
		numberMark), user_id(userId), project1_id(project1Id), project2_id(project2Id), value1(_value), value2(1 - _value) {}

	size_t getNumberMark() const {
		return number_mark;
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

	void setNumberMark(size_t numberMark) {
		number_mark = numberMark;
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

	bool operator==(const Mark& rhs) const {
		return number_mark == rhs.number_mark &&
			user_id == rhs.user_id &&
			project1_id == rhs.project1_id &&
			project2_id == rhs.project2_id &&
			value1 == rhs.value1;
	}

	bool operator!=(const Mark& rhs) const {
		return !(rhs == *this);
	}

	void setMark(size_t numberMark, size_t userId, size_t project1Id, size_t project2Id, float _value) {
		number_mark = numberMark;
		user_id = userId;
		project1_id = project1Id;
		project2_id = project2Id;
		value1 = _value;
		value2 = 1 - _value;
	}

	void setMark(Mark& tmp) {
		number_mark = tmp.number_mark;
		user_id = tmp.user_id;
		project1_id = tmp.project1_id;
		project2_id = tmp.project2_id;
		value1 = tmp.value1;
		value2 = tmp.value2;
	}

	friend std::ostream& operator<<(std::ostream& os, const Mark& mark) {
		os << "number_mark: " << mark.number_mark << " user_id: " << mark.user_id << " project1_id: "
			<< mark.project1_id << " project2_id: " << mark.project2_id << " value1: " << mark.value1 << " value2: " << mark.value2;
		return os;
	}

	static void enterMark(Mark& tmp) {
		size_t numberMark, userId, project1Id, project2Id;
		std::string numberMarkStr, userIdStr, project1IdStr, project2IdStr, _valueStr;
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
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		tmp.setMark(numberMark, userId, project1Id, project2Id, _value);
	}
};

class MarkSock : public Mark {
public:
	SOCKET sock;

	explicit MarkSock(SOCKET _sock) {
		this->sock = _sock;
	}
	void enterMark() {
		size_t numberMark, userId, project1Id, project2Id;
		std::string numberMarkStr, userIdStr, project1IdStr, project2IdStr, _valueStr;
		float _value;

		// получение
		// NumberMark авто из БД
		// expert_Id из БД (текущий пользователь)
		// project1_id и project2_id выбор из БД
		sendString(sock, "data");
		do {
			sendString(sock, "Оценку: ");
			_valueStr = takeString(sock);
			if (Checks::checkNoLetters(_valueStr)) {
				_value = std::stof(_valueStr);
				break;
			}
			else
				cout << "Некорректный ввод. Повторите попытку.\n";
		} while (true);
		sendString(sock, "end");
		setMark(numberMark, userId, project1Id, project2Id, _value);
	}
};

#endif //CURSSRV_MARK_H
