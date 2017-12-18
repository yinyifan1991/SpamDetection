//
// Created by yifan on 11/26/17.
//

#ifndef SPAMDETECTION_EVALUATION_H
#define SPAMDETECTION_EVALUATION_H

#include <string>
#include <vector>
#include "main.h"
#include "reviewer.h"

using namespace std;

class evaluation {
private:
    int nVtx;
    int nEdge;
    int reviewer_num;
    vector<string> id_map;
    vector<int> reviewer_triangles;
    vector<vector<int>> isolated_edges;
    vector<reviewer> reviewer_list;
public:
    evaluation();
    void read_files();
    void sort_by_score();
    void write_score_rank(string write_file);
};


#endif //SPAMDETECTION_EVALUATION_H
