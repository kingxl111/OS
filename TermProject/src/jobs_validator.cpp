#include "jobs_validator.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

using graph = vector<vector<int> >;

bool DFS(int u, int p, const graph & g, vector<int> & state, vector<int> & path, vector<int> & cycle) {
    
    if(state[u]==2) {
        return false;
    }

    path.push_back(u);
    state[u] = 1;
    for(int v : g[u]) {
        if(p != v) {
            if(state[v] == 1) {
                cycle.push_back(v);
                int idx = path.size() - 1;

                while(path[idx] != v) {
                    cycle.push_back(path[idx]);
                    --idx;
                }
                reverse(cycle.begin(), cycle.end());
                return true;
            }
            if(DFS(v, u, g, state, path, cycle)) {
                return true;
            }
        }
    }
    state[u] = 2;
    path.pop_back();
    return false;
}

bool without_cycles(graph &g) {
    
    vector<int> state(g.size(), 0);
    vector<int> path;
    vector<int> cycle;
    bool has_loop = DFS(0, -1, g, state, path, cycle);

    if(has_loop) {
        return false;
    }
    return true;
}

bool only_connectivity_component(graph& g) {
    for (size_t i = 0; i < g.size(); ++i) {
        if(g[i].size() > 1) {
            return false;
        }
    }
    return true;
}

bool has_start_component(graph &g) {

}


bool has_finish_component(graph &g) {

    for (size_t i = 0; i < g.size(); ++i){
        if(g[i].size() == 0) {
            return true;
        }
    }
    return false;
}