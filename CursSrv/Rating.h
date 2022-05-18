﻿#ifndef CURSSRV_RATING_H
#define CURSSRV_RATING_H

#include <ostream>
#include "../Utils/stdafx.h"
#include <vector>
#include "Project.h"
#include "Mark.h"

class Rating {
public:
	size_t size;
	size_t cntProjects;
	size_t cntExperts;
	std::vector<Project> vcProjects;
	std::map<std::string, size_t> mpExperts;
	size_t number;
	SOCKET sock;
	std::map<size_t, std::vector<Mark>> ranking;
	std::vector<Mark> rankingTotal;
	float total;

	Rating() {
		cntProjects = 0;
		cntExperts = 0;
		size = cntProjects * (cntProjects - 1) * cntExperts; // = 0;
		number = 0;
		sock = -1;
		total = 0;
	}

	explicit Rating(const SOCKET& _sock) {
		this->sock = _sock;
		cntProjects = 0;
		cntExperts = 0;
		size = cntProjects * (cntProjects - 1) * cntExperts; // = 0;
		number = 0;
	}

	//Rating(size_t _size, size_t _cntProjects, size_t _cntExperts, const vector <size_t>& _vcProjects, size_t _number) :
	Rating(SOCKET _sock, size_t _size, size_t _cntProjects, size_t _cntExperts, size_t _number) :
		size(_size),
		cntProjects(_cntProjects),
		cntExperts(_cntExperts),
		number(_number) {
		sock = _sock;
	}

	size_t getSize() const {
		return size;
	}

	void setSize(size_t _size) {
		Rating::size = _size;
	}

	size_t getCntProjects() const {
		return cntProjects;
	}

	void setCntProjects(size_t _cntProjects) {
		cntProjects = _cntProjects;
	}

	void setCntProjects() {
		cntProjects = vcProjects.size();
	}

	size_t getCntExperts() const {
		return cntExperts;
	}

	void setCntExperts(size_t _cntExperts) {
		cntExperts = _cntExperts;
	}

	void setCntExperts() {
		cntExperts = mpExperts.size();
	}

	size_t getNumber() const {
		return number;
	}

	void setNumber(size_t _number) {
		number = _number;
	}

	void addProject(Project tmp) {
		vcProjects.push_back(tmp);
	}


	void clear() {
		size = 0;
		cntExperts = 0;
		cntProjects = 0;
		vcProjects.clear();
		mpExperts.clear();
		number = 0;
		ranking.clear();
		rankingTotal.clear();
		total = 0;
	}

	// Выбор экспертов для ранжа
	void selectExperts(std::map<std::string, size_t> mpExp) {
		vector<string> vc = toVector(mpExp);
		cntExperts = 0;
		mpExperts.clear();
		std::cout << "--------------------------------------------" << std::endl;
		std::cout << "Список экспертов очищен (для ранжирования)!" << std::endl;
		do {
			sendString(sock, "menu2");
			sendString(sock, toString(vc));
			size_t ch = takeInt(sock);
			if (ch == 0) break;
			else if (ch != vc.size() + 1) {
				std::cout << "Добавить эксперта: " << vc[ch - 1] << ", id: " << mpExp[vc[ch - 1]] << std::endl;
				mpExperts.insert(make_pair(vc[ch - 1], mpExp[vc[ch - 1]]));
				auto iter = vc.cbegin(); // указатель на первый элемент
				vc.erase(iter + (ch - 1));
				if (vc.size() == 0) break;
			}
			else {
				std::cout << "Добавить всех экспертов:" << std::endl;
				for (auto it : vc) {
					std::cout << "Добавить эксперта: " << it << ", id: " << mpExp[it] << std::endl;
					mpExperts.insert(make_pair(it, mpExp[it]));
				}
				vc.clear();
				break;
			}
		} while (true);
		cntExperts = mpExperts.size();
		std::cout << "В список добавлено " << cntExperts << " экспертов(a)." << std::endl;
	}

	// Выбор проектов для ранжа
	void selectProjects(std::map<std::string, Project> mpPro) {
		vector<string> vc = toVector(mpPro);
		cntProjects = 0;
		vcProjects.clear();
		std::cout << "--------------------------------------------" << std::endl;
		std::cout << "Список проектов очищен (для ранжирования)!" << std::endl;
		do {
			sendString(sock, "menu2");
			sendString(sock, toString(vc));
			size_t ch = takeInt(sock);
			if (ch == 0) break;
			else if (ch != vc.size() + 1) {
				std::cout << "Добавить проект: " << vc[ch - 1] << ", id: " << mpPro[vc[ch - 1]].getProjectId() << std::endl;
				vcProjects.push_back(mpPro[vc[ch - 1]]);
				auto iter = vc.cbegin(); // указатель на первый элемент
				vc.erase(iter + (ch - 1));
				if (vc.size() == 0) break;
			}
			else {
				std::cout << "Добавить все проекты:" << std::endl;
				for (auto& it : vc) {
					std::cout << "Добавить проект: " << it << ", id: " << mpPro[it].getProjectId() << std::endl;
					vcProjects.push_back(mpPro[it]);
				}
				vc.clear();
				break;
			}
		} while (true);
		cntProjects = vcProjects.size();
		std::cout << "В список добавлено " << cntProjects << " проекта(ов)." << std::endl;
	}

	// Выбор номера для ранжа
	void selectNumber(std::vector<std::string> vc) {
		std::cout << "--------------------------------------------" << std::endl;
		sendString(sock, "menu303");
		sendString(sock, toString(vc, "Выберите номер ранжирования:"));
		size_t ch = takeInt(sock);
		if (ch == 0) return;
		else if (ch != vc.size() + 1) {
			number = stoi(vc[ch - 1]);
		}
		else {
			number = stoi(vc[vc.size() - 1]) + 1;
		}
		std::cout << "Выбран номер: " << number << std::endl;
	}

	//Ввод оценок одним экспертом
	std::vector<Mark> enterRank(std::pair<std::string, size_t> expert, size_t flag = 0) {
		MarkSock tmpMark(sock);
		std::vector<Mark> mrk;
		if (flag == 1) {
			std::cout << "--------------------------------------------" << std::endl;
			std::cout << "Ввод оценок (для ранжирования) одним экспертом!" << std::endl;
			printVcProjects();
		}
		for (size_t j = 0; j < cntProjects; j++) {
			for (size_t k = j + 1; k < cntProjects; k++) {
				std::cout << "Эксперт " << expert.first << "(" << expert.second << ")" << " -- " << j + 1 <<
					"(" << vcProjects[j].getProjectId() << ") : " << k + 1 << "(" << vcProjects[k].getProjectId() << "): ";
				tmpMark.enterMark(number, expert, vcProjects[j].getProjectId(), vcProjects[k].getProjectId());
				std::cout << tmpMark.getValue1() << " :: " << tmpMark.getValue2() << std::endl;
				mrk.push_back(MarkSock::toMark(tmpMark));
			}
		}
		ranking.insert(make_pair(expert.second, mrk));
	}

	//Ввод оценок всеми экспертами
	void enterRanks(size_t _number) {
		MarkSock tmpMark(sock);
		std::vector<Mark> mrk;
		std::cout << "--------------------------------------------" << std::endl;
		printVcProjects();
		std::cout << "Ввод оценок (для ранжирования) от всех экспертов!" << std::endl;
		number = _number;
		for (auto& it : mpExperts) {
			mrk.clear();
			/*for (size_t j = 0; j < cntProjects; j++) {
				for (size_t k = j + 1; k < cntProjects; k++) {
					std::cout << "Эксперт " << it.first << "(" << it.second << ")" << " -- " << j + 1 <<
						"(" << vcProjects[j].getProjectId() << ") : " << k + 1 << "(" << vcProjects[k].getProjectId() << "): ";
					tmpMark.enterMark(number, it, vcProjects[j].getProjectId(), vcProjects[k].getProjectId());
					std::cout << tmpMark.getValue1() << " :: " << tmpMark.getValue2() << std::endl;
					mrk.push_back(MarkSock::toMark(tmpMark));
				}
			}
			ranking.insert(make_pair(it.second, mrk));*/
			mrk = enterRank(it);
		}
	}



	//Расчет суммарных значений
	void calcTotal() {
		if (cntExperts == 0 || cntProjects == 0 || ranking.size() == 0) return;
		rankingTotal.clear();
		Mark tmp{};
		size_t cntTotal = 0;
		total = 0;
		for (auto& it : ranking) {
			tmp.clear();
			rankingTotal.push_back(tmp);
			for (auto iit : it.second) {
				rankingTotal[cntTotal] += iit;
				total = total + iit.getValue1() + iit.getValue2();
			}
			cntTotal++;
		}
	}

	//Расчет весов
	void calcWeights() {
		size_t cnt = 0;
		for (size_t i = 0; i < vcProjects.size(); i++) {
			for (size_t j = 0; j < rankingTotal.size(); j++) {
				if (rankingTotal[j].getProject1Id() == vcProjects[i].getProjectId()) vcProjects[i].setWeight(vcProjects[i].getWeight() + rankingTotal[j].getValue1());
				if (rankingTotal[j].getProject2Id() == vcProjects[i].getProjectId()) vcProjects[i].setWeight(vcProjects[i].getWeight() + rankingTotal[j].getValue2());
				if (rankingTotal[j].getProject1Id() == vcProjects[i].getProjectId() || rankingTotal[j].getProject2Id() == vcProjects[i].getProjectId()) cnt++;
				if (cnt == vcProjects.size()) break; //прерываем чтобы лишние разы не крутить
			}
			vcProjects[i].setWeight(vcProjects[i].getWeight() / total);
		}
		sort(vcProjects.begin(), vcProjects.end(), compareWeight); //Сортируем по весам
	}

	void createRank() {
		for (size_t i = 0; i < cntExperts; i++) {
			for (size_t j = 0; j < cntProjects; j++) {
				for (size_t k = j + 1; k < cntProjects; k++) {
					std::cout << i << " -- " << j << " : " << k << std::endl;
					//rank.insert(make_pair())
				}
			}
		}
	}

	// Вывод пояснений по проектам в ранже (в серверную консоль)
	void printVcProjects() {
		std::cout << "- - - - - - - - - - - - - - -" << std::endl;
		std::cout << "Проекты в ранже:" << std::endl;
		for (auto& it : vcProjects) {
			std::cout << it.getProjectId() << " - " << it.getProjectName() << std::endl;
		}
		std::cout << "- - - - - - - - - - - - - - -" << std::endl;
	}

	// Вывод пояснений по проектам в ранже (в клиентскую консоль или в file)
	void printVcProjectsSock(std::string fout = "") {
		sendString(sock, "output" + fout);
		sendString(sock, "- - - - - - - - - - - - - - -\n");
		sendString(sock, "Проекты в ранже:\n");
		for (auto& it : vcProjects) {
			sendString(sock, std::to_string(it.getProjectId()) + " - " + it.getProjectName() + "\n");
		}
		sendString(sock, "- - - - - - - - - - - - - - -\n");
		sendString(sock, "end");
	}

	// Вывод результата ранжирования проектов (в серверную консоль)
	void printRating(size_t flag = 0) {
		if (flag == 1) {
			// Заголовок
		}
		for (auto& it : vcProjects) {
			if (it == vcProjects[0]) it.printProjectWeight(2, 1);
			else it.printProjectWeight(2);
		}
	}

	// Вывод результата ранжирования проектов (в клиентскую консоль или в file)
	void printRatingSock(std::string fout = "", size_t flag = 0) {
		sendString(sock, "output" + fout);
		if (flag == 1) {
			// Заголовок
		}
		for (auto& it : vcProjects) {
			if (it == vcProjects[0]) it.printProjectWeightSock(sock, 2, 1);
			else it.printProjectWeightSock(sock, 2);
		}
		sendString(sock, "end");
	}

	// Поиск эксперта по id
	std::string findExpert(size_t id) {
		for (auto& it : mpExperts) {
			if (it.second == id) return it.first;
		}
	}

	// Вывод парных сравнений проектов в ранже (в серверную консоль)
	void printRatingTable(size_t flag = 0) {
		printVcProjects();
		std::cout << "+------------------------------+";
		for (size_t i = 0; i < cntProjects; i++) {
			std::cout << "---------+";
		}
		std::cout << std::endl;
		std::cout << "| Эксперт     \\ Пары проектов  |";
		for (auto& it : ranking.begin()->second) {
			std::cout << std::right << std::setw(2) << it.getProject1Id() << " <-> " << std::left << std::setw(2) << it.getProject2Id() << "|";
		}
		std::cout << std::endl;
		std::cout << "+------------------------------+";
		for (size_t i = 0; i < cntProjects; i++) {
			std::cout << "----+----+";
		}
		std::cout << std::endl;
		for (auto& it : ranking) {
			std::cout << "|" << std::left << std::setw(30) << findExpert(it.first) << "|";
			for (auto& iit : it.second) {
				std::cout << std::setw(4) << iit.getValue1() << "|" << std::setw(4) << iit.getValue2() << "|";
			}
			std::cout << std::endl;
			std::cout << "+------------------------------+";
			for (size_t i = 0; i < cntProjects; i++) {
				std::cout << "----+----+";
			}
			std::cout << std::endl;
		}

		std::cout << "|                     Итого:   |";
		for (auto& it : rankingTotal) {

			std::cout << std::setw(4) << it.getValue1() << "|" << std::setw(4) << it.getValue2() << "|";
		}
		std::cout << std::endl;
		std::cout << "+------------------------------+";
		for (size_t i = 0; i < cntProjects; i++) {
			std::cout << "----+----+";
		}
		std::cout << std::endl;
	}

	// Вывод парных сравнений проектов в ранже (в клиентскую консоль или в file)
	void printRatingTableSock(std::string fout = "", size_t flag = 0) {
		printVcProjectsSock(fout);
		sendString(sock, "output" + fout);
		std::stringstream ss;
		sendString(sock, "+------------------------------+");
		for (size_t i = 0; i < cntProjects; i++) {
			sendString(sock, "---------+");
		}
		sendString(sock, "\n| Эксперт     \\ Пары проектов  |");
		for (auto& it : ranking.begin()->second) {
			ss << std::right << std::setw(2) << it.getProject1Id() << " <-> " << std::left << std::setw(2) << it.getProject2Id() << "|";
		}
		sendString(sock, ss.str());
		ss.str("");
		sendString(sock, "\n+------------------------------+");
		for (size_t i = 0; i < cntProjects; i++) {
			sendString(sock, "----+----+");
		}
		for (auto& it : ranking) {
			ss << "\n|" << std::left << std::setw(30) << findExpert(it.first) << "|";
			//sendString(sock, ss.str());
			for (auto& iit : it.second) {
				ss << std::setw(4) << iit.getValue1() << "|" << std::setw(4) << iit.getValue2() << "|";
			}
			sendString(sock, ss.str());
			ss.str("");
			sendString(sock, "\n+------------------------------+");
			for (size_t i = 0; i < cntProjects; i++) {
				sendString(sock, "----+----+");
			}
		}
		sendString(sock, "\n|                     Итого:   |");
		for (auto& it : rankingTotal) {
			ss << std::setw(4) << it.getValue1() << "|" << std::setw(4) << it.getValue2() << "|";
		}
		sendString(sock, ss.str());
		ss.str("");
		sendString(sock, "\n+------------------------------+");
		for (size_t i = 0; i < cntProjects; i++) {
			sendString(sock, "----+----+");
		}
		sendString(sock, "\n");
		sendString(sock, "end");
	}


	//    friend std::ostream &operator<<(std::ostream &os, const Rating &rating) {
	//        os << "width: " << rating.width << " number_projects: " << rating.number_projects << " number_experts: "
	//           << rating.number_experts << " vcProjects: " << rating.vcProjects;
	//        return os;
	//    }
};
#endif //CURSSRV_RATING_H
