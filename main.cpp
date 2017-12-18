#include <iostream>
#include "json.hpp"
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "Decomposition.h"
#include "build_graph.h"
#include "main.h"
#include "evaluation.h"

using namespace std;
using json = nlohmann::json;

string folder;
char *folder_name;

vector<string> parse_bought(string line) ;
pair<string, unordered_set<string>> parse_category(string line);
string find_asin(string line);

void readFileJson();
pair<string, string> parse_review(string line);
unordered_map<string, unordered_set<string>> build_product_graph(unordered_map<string, unordered_set<string>>& map) ;
unordered_set<string> find_co_purchase(string line) ;

int main(int argc, char *argv[]) {
    //std::cout << "Hello, World!" << std::endl;
    string command ("eval");
    folder = "/home/yifan/CLionProjects/SpamDetection/reviews_whole/";
    if(command == "build_graph") {
        string rating_file = "/home/yifan/CLionProjects/SpamDetection/reviews_Clothing_Shoes_and_Jewelry.json";
        string meta_file = "/home/yifan/CLionProjects/SpamDetection/meta_Clothing_Shoes_and_Jewelry.json";
        build_graph bg(rating_file, meta_file);
        bg.build_graph_to_file();
        //readFileJson();
    }
    else if(command == "build_whole") {
        folder = "/home/yifan/CLionProjects/SpamDetection/reviews_whole/";
        vector<string> rating_files;
        vector<string> meta_files;
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Automotive.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Grocery_and_Gourmet_Food.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Video_Games.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Beauty.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Cell_Phones_and_Accessories.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Clothing_Shoes_and_Jewelry.json");
        rating_files.push_back("/home/yifan/CLionProjects/SpamDetection/reviews_Pet_Supplies.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Automotive.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Grocery_and_Gourmet_Food.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Video_Games.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Beauty.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Cell_Phones_and_Accessories.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Clothing_Shoes_and_Jewelry.json");
        meta_files.push_back("/home/yifan/CLionProjects/SpamDetection/meta_Pet_Supplies.json");
        build_graph bg(rating_files, meta_files);
        bg.build_whole_graph_to_file();
    }
    else if(command == "eval") {
        evaluation ev;
        ev.read_files();
        ev.sort_by_score();
        ev.write_score_rank(folder + "score_rank");
    }
    return 0;
}

//Read Json from file
void readFileJson() {
    ifstream review_in("/home/yifan/CLionProjects/SpamDetection/reviews_Baby.json");
    ifstream in("/home/yifan/CLionProjects/SpamDetection/meta_Baby.json");
    string line;
    unordered_map<string, unordered_set<string>> review_product_graph;
    if(review_in.is_open()) {
        while(getline(review_in, line)) {
            //cout << line << endl;
            pair<string, string> temp = parse_review(line);
            string reviewID = temp.first;
            string co_product = temp.second;
            if(review_product_graph.find(reviewID) == review_product_graph.end()) {
                unordered_set<string> neighbors;
                review_product_graph[reviewID] = neighbors;
            }
            if(review_product_graph.find(co_product) == review_product_graph.end()) {
                unordered_set<string> neighbors;
                review_product_graph[co_product] = neighbors;
            }
            review_product_graph[reviewID].insert(co_product);
            review_product_graph[co_product].insert(reviewID);
        }
    }
    if(in.is_open()) {
        //json j;
        vector<vector<string>> bought;
        //unordered_map<string, unordered_set<string>> asin_category;
        unordered_map<string, unordered_set<string>> product_network;
        //int product_counter = 0;
        while(getline(in, line)) {//read line and build user-product relations
            //cout << line << endl;
            //j = line;
            //string s = j.dump();
            string asin = find_asin(line);
            vector<string> sub_vector = parse_bought(line);
            pair<string, unordered_set<string>> pair = parse_category(line);
            bought.push_back(sub_vector);
            //asin_category.insert(pair);
            unordered_set<string> temp = find_co_purchase(line);
            unordered_set<string> em;
            if(review_product_graph.find(asin) == review_product_graph.end()) review_product_graph[asin] = em;
            //review_product_graph[asin] = find_co_purchase(line);
            for(auto it = temp.begin();it != temp.end();++it) {
                review_product_graph[asin].insert(*it);
                if(review_product_graph.find(*it) == review_product_graph.end()) review_product_graph[*it] = em;
                review_product_graph[*it].insert(asin);
            }
        }
        //build product-product relations
        //product_network = build_product_graph(asin_category);
        /*
        for(auto it = product_network.begin();it != product_network.end();++it) {
            cout << it->first << ": ";
            for(auto set_it = it->second.begin();set_it != it->second.end();++set_it) {
                cout << *set_it << ",";
            }
            cout << endl;
        }
         */
        /*
        for(auto it = review_product_graph.begin();it != review_product_graph.end();++it) {
            cout << it->first << ": ";
            for(auto set_it = it->second.begin();set_it != it->second.end();++set_it) {
                cout << *set_it << ",";
            }
            cout << endl;
        }
         */
    }

    cout << "original size: " << review_product_graph.size() << endl;
    int original_review_number = 0;
    for(auto it = review_product_graph.begin();it != review_product_graph.end();++it) {
        if(it->first.find("r_") != -1) original_review_number++;
    }
    cout << "original reviewer number: " << original_review_number << endl;
    int truss_num = 3;
    int truss_vertex_num = 1;
    unordered_map<string, unordered_set<string>> temp_graph = review_product_graph;
    Decomposition core(temp_graph, truss_num);
    unordered_map<string, unordered_set<string>> k_core_map = core.k_core();
    temp_graph = k_core_map;
    //cout << "core size: " << k_core_map.size() << endl;
    while(truss_vertex_num > 0) {

        Decomposition core(temp_graph, truss_num);
        //unordered_map<string, unordered_set<string>> k_core_map = core.k_core();
        //temp_graph = k_core_map;
        //Decomposition core(temp_graph, truss_num);
        cout << "truss number: " << truss_num << endl;
        //cout << "core size: " << k_core_map.size() << endl;

        unordered_map<string, unordered_set<string>> k_truss_map = core.k_truss();
        unordered_map<string, unordered_set<string>> dense_subgraph = core.remove_isolated_vertices();

        truss_vertex_num = dense_subgraph.size();
        cout << "final size: " << truss_vertex_num << endl;
        temp_graph = dense_subgraph;
        /*
        int review_num = 0;
        for(auto it = dense_subgraph.begin();it != dense_subgraph.end();++it) {
            if(it->first.find("r_") != -1) review_num++;
        }
        cout << "reviewer number contained in k-truss: " << review_num << endl;
         */
        truss_num++;
    }
    cout << "finished" << endl;

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
    /*
    if(line.find("also_viewed") != -1) {
        pos = line.find("also_viewed") + 16;
        //cout << "viewed: " << pos << " " << line.substr(pos, 10) << "\n";
        while (line[pos + 11] != ']') {
            sub_vector.push_back(line.substr(pos, 10));
            pos += 14;
        }
        sub_vector.push_back(line.substr(pos, 10));
    }
     */
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

unordered_set<string> find_co_purchase(string line) {
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
};
//Extract reviewerId
pair<string, string> parse_review(string line) {
    size_t pos = line.find("reviewerID");
    int i = pos + 14;
    string reviewer;
    while(line[i] != '"') {
        reviewer += line[i++];
    }
    reviewer = "r_" + reviewer;
    //cout << "ID: " << i << " " << reviewer << "\n";
    pos = line.find("asin");
    string asin = line.substr(pos + 8, 10);
    //cout << "asin:" << " " << asin << "\n";
    return make_pair(reviewer, asin);
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
 * time complexity is O(n^3)
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
                //cout << *set_it << ", ";
                if(v_set.count(*set_it) > 0) count++;
            }
            //cout << "\n";
            if(count > 1) {
                sub_set.insert(inner_asin);
            }
        }
        graph.insert(make_pair(asin, sub_set));
    }
    return graph;
}