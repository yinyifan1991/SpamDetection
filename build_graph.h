//
// Created by yifan on 10/29/17.
//

#ifndef SPAMDETECTION_BUILD_GRAPH_H
#define SPAMDETECTION_BUILD_GRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class build_graph {
private:
    string rating_file_name;
    string meta_file_name;
    int reviewer_num;
    int total_num;
    int edge;
    int rating_edge;
    vector<string> rating_files;
    vector<string> meta_files;
    unordered_map<string, int> id_to_int;
    //vector<unordered_set<int>> graph;
    vector<vector<int>> graph;
    void build_map();
    void build_review_graph();
    void build_review_whole();
    void build_meta_graph();
    pair<string, string> parse_review(string line);
    string find_asin(string line);
    unordered_set<string> find_co_purchase(string line);
    void write_graph_file(string write_file_name);
public:
    build_graph(string rating_file, string meta_file);
    build_graph(vector<string> rating_files, vector<string> meta_files);
    vector<unordered_set<int>> build_graph_to_file();
    vector<unordered_set<int>> build_whole_graph_to_file();
    vector<vector<int>> get_graph();
};


#endif //SPAMDETECTION_BUILD_GRAPH_H
