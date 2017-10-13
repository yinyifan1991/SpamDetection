#include <iostream>
#include "json.hpp"
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;
void readFileJson();

vector<string> parse_bought(string line) ;
pair<string, unordered_set<string>> parse_category(string line);
string find_asin(string line);

void parse_review(string line);

unordered_map<string, unordered_set<string>> build_product_graph(unordered_map<string, unordered_set<string>>& map) ;

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
        //json j;
        vector<vector<string>> bought;
        unordered_map<string, unordered_set<string>> asin_category;
        unordered_map<string, unordered_set<string>> product_network;
        //int product_counter = 0;
        while(getline(in, line)) {//read line and build user-product relations
            //j = line;
            //string s = j.dump();
            string asin = find_asin(line);
            vector<string> sub_vector = parse_bought(line);
            pair<string, unordered_set<string>> pair = parse_category(line);
            bought.push_back(sub_vector);
            asin_category.insert(pair);
        }
        //build product-product relations
        product_network = build_product_graph(asin_category);
        for(auto it = product_network.begin();it != product_network.end();++it) {
            cout << it->first << ": ";
            for(auto set_it = it->second.begin();set_it != it->second.end();++set_it) {
                cout << *set_it << ",";
            }
            cout << endl;
        }
    }

}
//Extract products which are also_bought and also_viewed
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
//Extract reviewerId
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
//Extract categories one product belongs to
pair<string, unordered_set<string>> parse_category(string line) {
    unordered_set<string> category;
    string asin = find_asin(line);
    size_t pos = line.find("categories");
    if(pos == -1) {
        return make_pair(asin, category);
    }
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
    return make_pair(asin, category);
    //return category;
}
//Extrac asin ID
string find_asin(string line) {
    size_t pos = line.find("asin");
    if(pos == -1) return "no asin";
    return line.substr(pos + 8, 10);
}
/*Construct product-product graph
 * compare the categories one product belongs to with the categories another product belongs to
 * if the number of matches is more than a threshold
 * identify they are connected
 * time complexity is O(kn^2)
 * */
unordered_map<string, unordered_set<string>> build_product_graph(unordered_map<string, unordered_set<string>>& map) {
    unordered_map<string, unordered_set<string>> graph;
    for(unordered_map<string, unordered_set<string>>::iterator it = map.begin();it != map.end();++it) {
        unordered_set<string> sub_set;
        unordered_set<string> u_set = it->second;
        string asin = it->first;
        int count = 0;
        for(unordered_map<string, unordered_set<string>>::iterator inner_it = map.begin();inner_it != map.end();++inner_it) {
            string inner_asin = inner_it->first;
            if(asin == inner_asin) continue;
            unordered_set<string> v_set = inner_it->second;
            for(unordered_set<string>::iterator set_it = u_set.begin();set_it != u_set.end();++set_it) {
                cout << *set_it << ", ";
                if(v_set.count(*set_it) > 0) count++;
            }
            cout << "\n";
            if(count > 1) {
                sub_set.insert(inner_asin);
            }
        }
        graph.insert(make_pair(asin, sub_set));
    }
    return graph;
}