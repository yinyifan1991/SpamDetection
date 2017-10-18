//
// Created by yifan on 10/17/17.
//

#include <vector>
#include <queue>
#include "Decomposition.h"

Decomposition::Decomposition(unordered_map<string, unordered_set<string>> graph, int k) {
    this->graph = graph;
    this->K = k;
}

unordered_map<string, unordered_set<string>> Decomposition::k_core() {
    int k = this->K - 1;
    int counter = 0;
    queue<string> bfsqueue;
    unordered_set<string> in_queue;
    for(auto it = graph.begin();it != graph.end();it++) {
        //cout << "set size: " << it->second.size() << endl;
        if(it->second.size() < k) {
            bfsqueue.push(it->first);
            in_queue.insert(it->first);
        }
    }
    while(!bfsqueue.empty()) {
        string u = bfsqueue.front();
        unordered_set<string> neighbors = graph[u];
        for(auto it = neighbors.begin();it != neighbors.end();++it) {
            if(graph.find(*it) != graph.end()) {
                graph[*it].erase(u);
            }
            if(graph.find(*it) != graph.end() && graph[*it].size() < k && in_queue.find(*it) == in_queue.end()) {
                bfsqueue.push(*it);
                in_queue.insert(*it);
            }
        }
        counter++;
        graph.erase(u);
        bfsqueue.pop();
    }
    cout << "nodes to be erased: " << counter << endl;
    return graph;
}

unordered_map<string, unordered_set<string>> Decomposition::k_truss() {
    int k = this->K - 2;
    int counter = 0;
    queue<string> bfsqueue;
    unordered_set<string> in_queue;
    unordered_map<string, int> c_num;
    for(auto it = graph.begin();it != graph.end();++it) {
        unordered_set<string> u_neighbors = it->second;
        for(auto u_it = u_neighbors.begin();u_it != u_neighbors.end();++u_it) {
            int common_neighbor = 0;
            if(graph.find(*u_it) != graph.end()) {
                unordered_set<string> v_neighbors = graph[*u_it];
                for(auto v_it = v_neighbors.begin();v_it != v_neighbors.end();++v_it) {
                    if(u_neighbors.find(*v_it) != u_neighbors.end()) common_neighbor++;
                }
            }
            string _edge = it->first + "-" + *u_it;
            //cout << _edge << endl;
            if(common_neighbor < k) {
                bfsqueue.push(_edge);
                in_queue.insert(_edge);
                graph[it->first].erase(*u_it);
                if(graph.find(*u_it) != graph.end()) graph[*u_it].erase(it->first);
            }
            else {
                //cout << "common: " << common_neighbor << endl;
                c_num[_edge] = common_neighbor;
            }

        }
    }
    //BFS delete vertex pairs which has less common neighbors than k
    while(!bfsqueue.empty()) {
        string edge = bfsqueue.front();
        int pos = edge.find("-");
        string a = edge.substr(0, pos), b = edge.substr(pos + 1);
        //cout << a << " " << b << endl;
        unordered_set<string> a_neighbors;
        unordered_set<string> b_neighbors;
        unordered_set<string> common_temp;
        if(graph.find(a) != graph.end()) a_neighbors = graph[a];
        if(graph.find(b) != graph.end()) b_neighbors = graph[b];
        for(auto it = b_neighbors.begin();it != b_neighbors.end();++it) {
            //cout << a << " " << b << " " << *it << endl;
            if(a_neighbors.find(*it) != a_neighbors.end()) {
                //cout << a << " " << b << " " << *it << endl;
                common_temp.insert(*it);
            }
        }
        for(auto it = common_temp.begin();it != common_temp.end();++it) {
            string e_prime_a1 = a + "-" + *it;
            string e_prime_a2 = *it + "-" + a;
            string e_prime_b1 = b + "-" + *it;
            string e_prime_b2 = *it + "-" + b;
            //cout << a << " " << b << " " << *it << endl;
            if(c_num.find(e_prime_a1) != c_num.end()) {
                //cout << e_prime_a1 << " " << c_num[e_prime_a1] << endl;
                if(c_num[e_prime_a1]-- <= k) {
                    //cout << e_prime_a1 << " " << c_num[e_prime_a1] << endl;
                    bfsqueue.push(e_prime_a1);
                    in_queue.insert(e_prime_a1);
                    if(graph.find(a) != graph.end()) graph[a].erase(*it);
                    if(graph.find(b) != graph.end()) graph[b].erase(*it);
                    if(graph.find(*it) != graph.end()) {
                        graph[*it].erase(a);
                        graph[*it].erase(b);
                    }
                }

            }
            if(c_num.find(e_prime_a2) != c_num.end()) {
                //cout << e_prime_a2 << " " << c_num[e_prime_a2] << endl;
                if(c_num[e_prime_a2]-- <= k) {
                    //cout << e_prime_a2 << " " << c_num[e_prime_a2] << endl;
                    bfsqueue.push(e_prime_a2);
                    in_queue.insert(e_prime_a2);
                    if(graph.find(a) != graph.end()) graph[a].erase(*it);
                    if(graph.find(b) != graph.end()) graph[b].erase(*it);
                    if(graph.find(*it) != graph.end()) {
                        graph[*it].erase(a);
                        graph[*it].erase(b);
                    }
                }
            }
            if(c_num.find(e_prime_b1) != c_num.end()) {
                //cout << e_prime_b1 << " " << c_num[e_prime_b1] << endl;
                if(c_num[e_prime_b1]-- <= k) {
                    //cout << e_prime_b1 << " " << c_num[e_prime_b1] << endl;
                    bfsqueue.push(e_prime_b1);
                    in_queue.insert(e_prime_b1);
                    if(graph.find(a) != graph.end()) graph[a].erase(*it);
                    if(graph.find(b) != graph.end()) graph[b].erase(*it);
                    if(graph.find(*it) != graph.end()) {
                        graph[*it].erase(a);
                        graph[*it].erase(b);
                    }
                }
            }
            if(c_num.find(e_prime_b2) != c_num.end()) {
                //cout << e_prime_b2 << " " << c_num[e_prime_b2] << endl;
                if(c_num[e_prime_b2]-- <= k) {
                    //cout << e_prime_b2 << " " << c_num[e_prime_b2] << endl;
                    bfsqueue.push(e_prime_b2);
                    in_queue.insert(e_prime_b2);
                    if(graph.find(a) != graph.end()) graph[a].erase(*it);
                    if(graph.find(b) != graph.end()) graph[b].erase(*it);
                    if(graph.find(*it) != graph.end()) {
                        graph[*it].erase(a);
                        graph[*it].erase(b);
                    }
                }
            }
        }
        bfsqueue.pop();
    }
    return graph;
}

unordered_map<string, unordered_set<string>> Decomposition::remove_isolated_vertices() {
    vector<string> temp;
    for(auto it = graph.begin();it != graph.end();++it) {
        if(it->second.empty()) {
            cout << "empty " << it->first << endl;
            temp.push_back(it->first);
        }
        //cout << it->second.size() << endl;
    }
    for(auto it = temp.begin();it != temp.end();++it) graph.erase(*it);
    return graph;
}
