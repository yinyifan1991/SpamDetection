//
// Created by yifan on 11/26/17.
//

#include "evaluation.h"
#include <fstream>
#include <iostream>
#include "main.h"
#include "reviewer.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#define MAXLINE 1000000

using namespace std;

bool comparator(reviewer r1, reviewer r2) {return r1.score < r2.score;}

evaluation::evaluation() {
    nVtx = -1;
    nEdge = -1;
    reviewer_num = -1;
}

void evaluation::read_files() {
    string id_map_file = folder + "id_to_int.txt";
    string triangle_file = folder + "triangles_reviewer_involved.txt";
    string edge_file = folder + "isolated_edge.txt";
    //string group_file = folder + "check_group.txt";

    char* line = (char*) malloc (sizeof (char) * MAXLINE);
    //cout << id_map_file << endl;
    FILE* matfp = fopen(id_map_file.c_str(), "r");
    // skip comments
    do {
        fgets(line, 1000000, matfp);
    } while (line[0] == '%');
    stringstream ss (line);
    ss >> nVtx >> reviewer_num;
    id_map.resize(nVtx);
    string name;
    int id;
    for (int i = 0; i < nVtx; i++) {
        fgets(line, MAXLINE, matfp);
        stringstream ss (line);
        ss >> name >> id;
        if(name.find("r_") != -1) name = name.substr(name.find("r_") + 2);
        id_map[id] = name;
    }
    fclose(matfp);

    int triangles;
    reviewer_list.resize(reviewer_num);
    matfp = fopen(triangle_file.c_str(), "r");
    while(fgets(line, MAXLINE, matfp)) {
        triangles = 0;
        stringstream ss (line);
        ss >> id >> triangles;
        string name = id_map[id];
        reviewer r(id, name, triangles);
        r.score += triangles;
        reviewer_list[id] = r;
    }
    fclose(matfp);

    int ie;
    int ie_num;
    matfp = fopen(edge_file.c_str(), "r");
    while(fgets(line, MAXLINE, matfp)) {
        ie_num = 0;
        int loop = 1;
        string ignore = "none";
        stringstream ss (line);
        ss >> id >> ie_num >> ignore;
        if(ignore != "none") {
            loop = ie_num;
        }
        reviewer_list[id].isolated_edge = ie_num;
        if(reviewer_list[id].name == "NA") reviewer_list[id].name = id_map[id];
        for(int i = 0;i < loop;i++) {
            fgets(line, MAXLINE, matfp);
            stringstream ss (line);
            ss >> id >> ie;
            string edge = id_map[ie];
            reviewer_list[id].isolated_edge_list.push_back(edge);
        }
        reviewer_list[id].score -= ie_num;
    }

    string tmp_line;
    /*
    int subcore_id;
    matfp = fopen(group_file.c_str(), "r");
    ofstream gf (folder + "group_feature.txt");
    while(fgets(line, MAXLINE, matfp)) {
        int r = 0;
        tmp_line = line;
        gf << tmp_line << endl;
        subcore_id = stoi(tmp_line.substr(0, tmp_line.find_first_of(":")));
        //cout << tmp_line.find("reviewer number:") << endl;
        //cout << tmp_line.substr(tmp_line.find("reviewer number:") + 17) << endl;
        r = stoi(tmp_line.substr(tmp_line.find("reviewer number:") + 17));
        for(int i = 0;i < r;i++) {
            fgets(line, MAXLINE, matfp);
            stringstream ss (line);
            ss >> id;
            gf << id << " " << reviewer_list[id].triangle_num << " " << reviewer_list[id].isolated_edge << endl;
        }
    }
    fclose(matfp);
    gf.close();
     */
}

void evaluation::sort_by_score() {
    sort(reviewer_list.begin(), reviewer_list.end(), comparator);
}

void evaluation::write_score_rank(string write_file) {
    ofstream out (write_file);
    out << "ID Triangles Isolated Edges Score" << endl;
    for(int i = 0;i < reviewer_list.size();i++) {
        out << reviewer_list[i].name << " " << reviewer_list[i].triangle_num << " " << reviewer_list[i].isolated_edge << " " << reviewer_list[i].score << endl;
    }
    out.close();
}
