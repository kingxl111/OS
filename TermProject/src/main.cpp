#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "jobs.hpp"
#include "jobs_validator.hpp"

using namespace std;
using graph = vector<vector<int> >;

const string CONFIG = "config.yaml";

int first_digit_in_string(string &s) {
    int idx = 0;
    while(idx < s.size()) {
        if(s[idx] >= '0' && s[idx] <= '9') {
            return idx;
        }
        ++idx;
    }
    if(idx == s.size()) {
        return -1;
    }
    return idx;
}

auto main() -> int {

    YAML::Node config = YAML::LoadFile(CONFIG);
    const YAML::Node& jobs = config["Jobs"];
    
    vector<vector<string> > tmp_graph;
    for (YAML::const_iterator it = jobs.begin(); it != jobs.end(); ++it) {
        const YAML::Node& job = *it;
        cout << "Name: " << job["Name"].as<string>() << endl;
        
        cout << "Vertices: ";
        vector<string> cur_vertices = job["Vertices"].as<vector<string> >();
        for (size_t i = 0; i < cur_vertices.size(); ++i) {
            cout << cur_vertices[i] << ", ";
        } cout << endl;
        tmp_graph.push_back(cur_vertices);

        cout << "Job: " << job["Job"].as<string>() << endl;
    }

    vector<vector<int> > g(tmp_graph.size());
    for (size_t i = 0; i < tmp_graph.size(); ++i) {
        
        vector<int> vts(tmp_graph[i].size());
        for(size_t j = 0; j < tmp_graph[i].size(); ++j) {
            
            int first_digit_idx = first_digit_in_string(tmp_graph[i][j]);
            string vtx_n = "";
            for(size_t k = first_digit_idx; k < tmp_graph[i][j].size(); ++k) {
                vtx_n += tmp_graph[i][j][k];
            }
            vts[j] = stoi(vtx_n);
        } 
        g[i] = vts;
    }

    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            cout << g[i][j] << " ";
        } 
        cout << endl;
    }

    try {
        if(!only_connectivity_component(g)) {
            throw std::logic_error("Graph' vertex hasn't the only one connectivity component");
            exit(-1);
        }
        if(!without_cycles(g)) {
            throw std::logic_error("Graph has cycle");
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    

    

    return 0;
}
