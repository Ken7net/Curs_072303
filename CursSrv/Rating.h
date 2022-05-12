#ifndef CURSSRV_RATING_H
#define CURSSRV_RATING_H

#include <ostream>
#include "../Utils/stdafx.h"
#include <vector>
#include "Project.h"

class Rating {
public:
    size_t size;
    size_t cntProjects;
    size_t cntExperts;
    vector<Project> vcProjects;
    size_t number;

    Rating() {
        cntProjects = 0;
        cntExperts = 0;
        size = cntProjects * (cntProjects - 1) * cntExperts; // = 0;
        number = 0;
    }

    Rating(size_t _size, size_t _cntProjects, size_t _cntExperts, const vector <size_t> &_vcProjects, size_t _number) : 
        size(_size),
        cntProjects(_cntProjects),
        cntExperts(_cntExperts),
        vcProjects(_vcProjects),
        number(_number) {}

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
