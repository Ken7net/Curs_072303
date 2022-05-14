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

	Rating() {
		cntProjects = 0;
		cntExperts = 0;
		size = cntProjects * (cntProjects - 1) * cntExperts; // = 0;
		number = 0;
	}

	explicit Rating(const SOCKET& _sock) {
		this->sock = _sock;
		cntProjects = 0;
		cntExperts = 0;
		size = cntProjects * (cntProjects - 1) * cntExperts; // = 0;
		number = 0;
	}

	//Rating(size_t _size, size_t _cntProjects, size_t _cntExperts, const vector <size_t>& _vcProjects, size_t _number) :
	Rating(size_t _size, size_t _cntProjects, size_t _cntExperts, size_t _number) :
		size(_size),
		cntProjects(_cntProjects),
		cntExperts(_cntExperts),
		number(_number) {
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
		size_t cntVc = 0;
		cntExperts = 0;
		mpExperts.clear();
		do {
			cntVc = vc.size();
			sendString(sock, "menu5");
			sendString(sock, toString(vc));
			size_t ch = takeInt(sock);
			if (ch == 0) break;
			else if (ch != vc.size()+1) { 
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
