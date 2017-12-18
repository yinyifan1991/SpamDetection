//
// Created by yifan on 11/28/17.
//

#ifndef SPAMDETECTION_REVIEWER_H
#define SPAMDETECTION_REVIEWER_H

#include <string>
#include <vector>
#include <iostream>
#include "main.h"

using namespace std;

class reviewer {
public:
    int id;
    string name;
    int triangle_num;
    int isolated_edge;
    int score;
    vector<string> isolated_edge_list;
    reviewer();
    reviewer(int id, string name, int t);
};


#endif //SPAMDETECTION_REVIEWER_H
