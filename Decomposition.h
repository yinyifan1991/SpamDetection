//
// Created by yifan on 10/17/17.
//


#ifndef SPAMDETECTION_DECOMPOSITION_H
#define SPAMDETECTION_DECOMPOSITION_H
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Decomposition {
private:
    unordered_map<string, unordered_set<string>> graph;
    int K;
public:
    Decomposition(unordered_map<string, unordered_set<string>> graph, int k);
    unordered_map<string, unordered_set<string>> k_core();
    unordered_map<string, unordered_set<string>> k_truss();
    unordered_map<string, unordered_set<string>> remove_isolated_vertices();
};


#endif //SPAMDETECTION_DECOMPOSITION_H
