//
// Created by yifan on 10/29/17.
//

#include <fstream>
#include <iostream>
#include "build_graph.h"

build_graph::build_graph(string rating_file, string meta_file) {
    this->rating_file_name = rating_file;
    this->meta_file_name = meta_file;
    this->total_num = 0;
    this->edge = 0;
}

vector<unordered_set<int>> build_graph::build_graph_to_file() {
    build_review_graph();
    build_meta_graph();
    /*
    for(int i = 0;i < graph.size();i++) {
        if(i >= 100) break;
        for(int j = 0;j < graph[i].size();j++) {
            cout << i << " " << graph[i][j] << endl;
        }
    }
    */
    string write_file_name = "/home/yifan/CLionProjects/SpamDetection/reviews_video_games.mtx.txt";
    write_graph_file(write_file_name);
}

void build_graph::build_review_graph() {
    unordered_map<string, unordered_set<string>> map;
    ifstream review_in(rating_file_name);
    string line;
    if(review_in.is_open()) {
        while(getline(review_in, line)) {
            pair<string, string> temp = parse_review(line);
            string review_id = temp.first;
            string asin = temp.second;
            if(id_to_int.find(review_id) == id_to_int.end()) {
                id_to_int[review_id] = total_num++;
                /*
                unordered_set<int> em;
                graph.push_back(em);
                 */
                vector<int> em;
                graph.push_back(em);
            }
            if(map.find(review_id) == map.end()) {
                unordered_set<string> temp_set;
                map[review_id] = temp_set;
            }
            map[review_id].insert(asin);
        }
    }
    reviewer_num = total_num - 1;
    for(auto it = map.begin();it != map.end();++it) {
        int index = id_to_int[it->first];
        for(auto set_it = it->second.begin();set_it != it->second.end();++set_it) {
            if(id_to_int.find(*set_it) == id_to_int.end()) {
                id_to_int[*set_it] = total_num++;
                /*
                unordered_set<int> em;
                graph.push_back(em);
                 */
                vector<int> em;
                graph.push_back(em);
            }
            int set_index = id_to_int[*set_it];
            bool flag = false, same = false;

            if(graph[index].empty()) {
                graph[index].push_back(set_index);
                edge++;
                flag = true;
            }
            else {
                for(auto v = graph[index].begin();v != graph[index].end();++v) {
                    if(*v > set_index) {
                        graph[index].insert(v, set_index);
                        edge++;
                        flag = true;
                        break;
                    }
                    if(*v == set_index) {
                        same = true;
                        break;
                    }
                }
            }
            if(!flag && !same) {
                graph[index].push_back(set_index);
                edge++;
            }
            //graph[index].insert(set_index);
            //edge++;
        }
    }
}

void build_graph::build_meta_graph() {
    ifstream meta_in(meta_file_name);
    string line;
    if(meta_in.is_open()) {
        while(getline(meta_in, line)) {
            string asin = find_asin(line);
            unordered_set<string> temp = find_co_purchase(line);
            if(id_to_int.find(asin) == id_to_int.end()) {
                id_to_int[asin] = total_num++;
                //unordered_set<int> em;
                //graph.push_back(em);
                vector<int> em;
                graph.push_back(em);
                //cout << "first: " << asin << endl;
            }
            //cout << "temp: " << endl;
            int index = id_to_int[asin];
            for(auto it = temp.begin();it != temp.end();++it) {
                if(id_to_int.find(*it) == id_to_int.end()) {
                    id_to_int[*it] = total_num++;
                    //unordered_set<int> tm;
                    //graph.push_back(tm);
                    //cout << "second: " << *it << endl;
                    vector<int> em;
                    graph.push_back(em);
                }
                int set_index = id_to_int[*it];
                int x, y;
                if(index < set_index) {
                    //graph[index].insert(set_index);
                    //edge++;
                    x = index;
                    y = set_index;
                }
                else if(index > set_index) {
                    //graph[set_index].insert(index);
                    //edge++;
                    x = set_index;
                    y = index;
                }
                else continue;

                bool flag = false, same = false;
                if(graph[x].empty()) {
                    graph[x].push_back(y);
                    //cout << x << " " << y << endl;
                    edge++;
                    flag = true;
                    //cout << "third: " << x << " " << y << endl;
                }
                else {
                    for(auto v = graph[x].begin();v != graph[x].end();++v) {
                        if(*v > y) {
                            //cout << "forth: " << *v << " " << y << endl;
                            graph[x].insert(v, y);
                            edge++;
                            flag = true;
                            break;
                        }
                        if(*v == y) {
                            //cout << "same: " << *v << " " << y << endl;
                            same = true;
                            break;
                        }
                    }
                }
                if(!flag && !same) {
                    graph[x].push_back(y);
                    //cout << "fifth: " << x << " " << y << endl;
                    edge++;
                }
            }
        }
    }
}

pair<string, string> build_graph::parse_review(string line) {
    size_t pos = line.find("reviewerID");
    int i = pos + 14;
    string reviewer;
    while(line[i] != '"') {
        reviewer += line[i++];
    }
    reviewer = "r_" + reviewer;
    pos = line.find("asin");
    string asin = line.substr(pos + 8, 10);
    return make_pair(reviewer, asin);
}

string build_graph::find_asin(string line) {
    size_t pos = line.find("asin");
    if(pos == -1) return "no asin";
    return line.substr(pos + 8, 10);
}

unordered_set<string> build_graph::find_co_purchase(string line) {
    unordered_set<string> sub_set;
    size_t pos = line.find("asin");
    if(line.find("also_bought") != -1) {
        pos = line.find("also_bought") + 16;
        //cout << "bought: " << pos << " " << line.substr(pos, 10) << "\n";
        while(line[pos + 11] != ']') {
            sub_set.insert(line.substr(pos, 10));
            pos += 14;
        }
        sub_set.insert(line.substr(pos, 10));
    }
    return sub_set;
}

void build_graph::write_graph_file(string file_name) {
    ofstream myfile (file_name);
    if(!myfile.is_open()) {
        cout << "Unable to open file" << endl;
        myfile.close();
        return;
    }

    myfile << graph.size() << ' ' << edge << endl;
    for(int i = 1;i <= graph.size();i++) {
        for(auto it = graph[i-1].begin();it != graph[i-1].end();++it) {
            myfile << i << ' ' << *it + 1 << endl;
            if(i == *it + 1) cout << i << ' ' << *it + 1 << endl;
        }
    }
    myfile.close();
}

