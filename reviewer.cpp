//
// Created by yifan on 11/28/17.
//

#include "reviewer.h"

using namespace std;

reviewer::reviewer() {
    this->id = -1;
    this->name = "NA";
    this->triangle_num = 0;
    this->isolated_edge = 0;
    this->score = 0;
}
reviewer::reviewer(int id, string name, int t) {
    this->id = id;
    this->name = name;
    this->triangle_num = t;
    this->isolated_edge = 0;
    this->score = 0;
}