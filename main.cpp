#include <iostream>
#include "json.hpp"
#include <string>
#include <fstream>
//#include <unordered_set.h>
#include <unordered_set>
//#include <bits/unordered_map.h>

using namespace std;
using json = nlohmann::json;
void readFileJson();

vector<string> parse_bought(string line) ;
unordered_set<string> parse_category(string line);

void parse_review(string line);

int main() {
    //std::cout << "Hello, World!" << std::endl;
    readFileJson();
    return 0;
}

//Read Json from file
void readFileJson() {
    ifstream in("/home/yifan/CLionProjects/SpamDetection/test.json");
    string line;
    if(in.is_open()) {
        json j;
        vector<vector<string>> bought;
        vector<unordered_set<string>> category;
        while(getline(in, line)) {
            j = line;
            string s = j.dump();
            vector<string> sub_vector = parse_bought(line);
            unordered_set<string> sub_set = parse_category(line);
            for(unordered_set<string>::iterator it = sub_set.begin();it != sub_set.end();++it) cout << " " << *it;
            cout << "\n";
            cout << line << "\n";
            bought.push_back(sub_vector);
        }
    }

}

vector<string> parse_bought(string line) {
    vector<string> sub_vector;
    size_t pos = line.find("asin");
    sub_vector.push_back(line.substr(pos + 8, 10));
    if(line.find("also_bought") != -1) {
        pos = line.find("also_bought") + 16;
        //cout << "bought: " << pos << " " << line.substr(pos, 10) << "\n";
        while(line[pos + 11] != ']') {
            sub_vector.push_back(line.substr(pos, 10));
            pos += 14;
        }
        sub_vector.push_back(line.substr(pos, 10));
    }
    if(line.find("also_viewed") != -1) {
        pos = line.find("also_viewed") + 16;
        //cout << "viewed: " << pos << " " << line.substr(pos, 10) << "\n";
        while (line[pos + 11] != ']') {
            sub_vector.push_back(line.substr(pos, 10));
            pos += 14;
        }
        sub_vector.push_back(line.substr(pos, 10));
    }
    /*
    if(line.find("buy_after_viewing") != -1) {
        pos = line.find("buy_after_viewing") + 22;
        //cout << "viewed: " << pos << " " << line.substr(pos, 10) << "\n";
        while (line[pos + 11] != ']') {
            sub_vector.push_back(line.substr(pos, 10));
            pos += 14;
        }
        sub_vector.push_back(line.substr(pos, 10));
    }
     */
    return sub_vector;
}

void parse_review(string line) {
    size_t pos = line.find("reviewerID");
    int i = pos + 14;
    string reviewer;
    while(line[i] != '"') {
        reviewer += line[i++];
    }
    cout << "ID: " << i << " " << reviewer << "\n";
    pos = line.find("asin");
    string asin = line.substr(pos + 8, 10);
    cout << "asin:" << " " << asin << "\n";
}

unordered_set<string> parse_category(string line) {
    unordered_set<string> category;
    size_t pos = line.find("categories");
    if(pos == -1) return category;
    pos += 13;
    int brackets = 0;
    while(line[pos++] == '[') brackets++;
    while(brackets > 0) {
        if(line[pos] == '[') brackets++;
        else if(line[pos] == ']') brackets--;
        else {
            if(!isalpha(line[pos])) {
                pos++;
                continue;
            }
            else {
                int end = pos;
                while(line[end++] != '\'');
                category.insert(line.substr(pos, end - pos - 1));
                pos = end;
                continue;
            }
        }
    }
    return category;
}