#ifndef CURSSRV_RATING_H
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

	size_t getCntExperts() const {
		return cntExperts;
	}

	void setCntExperts(size_t _cntExperts) {
		cntExperts = _cntExperts;
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
		number = 0;
	}

	void selectExperts(std::map<std::string, size_t> mpExp) {
		vector<string> vc = toVector(mpExp);
		cntExperts = 0;
		mpExperts.clear();
		do {
			sendString(sock, "menu2");
			sendString(sock, toString(vc));
			size_t ch = takeInt(sock);
			if (ch == 0) break;
			else if (ch != vc.size()) { 
				mpExperts.insert(make_pair(vc[ch - 1], mpExp[vc[ch - 1]]));
				auto iter = vc.cbegin(); // указатель на первый элемент
				vc.erase(iter + (ch - 1));
				if (vc.size() == 0) break;
			}
			else {
				for (auto it : vc) {
					mpExperts.insert(make_pair(it, mpExp[it]));
				}
				vc.clear();
				break;
			}
		} while (true);
		cntExperts = mpExperts.size();
	}

	void selectProjects(std::map<std::string, Project> mpPro) {
		vector<string> vc = toVector(mpPro);
		cntProjects = 0;
		vcProjects.clear();
		do {
			sendString(sock, "menu2");
			sendString(sock, toString(vc));
			size_t ch = takeInt(sock);
			if (ch == 0) break;
			else if (ch != vc.size()) {
				vcProjects.push_back(mpPro[vc[ch - 1]]);
				auto iter = vc.cbegin(); // указатель на первый элемент
				vc.erase(iter + (ch - 1));
				if (vc.size() == 0) break;
			}
			else {
				for (auto& it : vc) {
					vcProjects.push_back(mpPro[it]);
				}
				vc.clear();
				break;
			}
		} while (true);
		cntProjects = vcProjects.size();
	}

	//Ввод оценок
	void enterRank() {
		MarkSock tmpMark(sock);
		std::vector<Mark> mrk;
		for (auto& it : mpExperts) {
			mrk.clear();
			for (size_t j = 0; j < cntProjects; j++) {
				for (size_t k = j + 1; k < cntProjects; k++) {
					std::cout << it.second << " -- " << j << " : " << k << std::endl;
					tmpMark.enterMark(number, it.second, vcProjects[j].getProjectId(), vcProjects[k].getProjectId());
					mrk.push_back(MarkSock::toMark(tmpMark));
				}
			}
			ranking.insert(make_pair(it.second, mrk));
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
			vcProjects[i].setWeight(total / vcProjects[i].getWeight());
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

	//const vector <size_t> &getVcProjects() const {
	//    return vcProjects;
	//}

	//void setVcProjects(const vector <size_t> &vcProjects) {
	//    Rating::vcProjects = vcProjects;
	//}

//    friend std::ostream &operator<<(std::ostream &os, const Rating &rating) {
//        os << "width: " << rating.width << " number_projects: " << rating.number_projects << " number_experts: "
//           << rating.number_experts << " vcProjects: " << rating.vcProjects;
//        return os;
//    }
};
#endif //CURSSRV_RATING_H
