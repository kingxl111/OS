#include "jobs_validator.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

using graph = vector<vector<int> >;

bool DFS_cycle(int u, int p, const graph & g, vector<int> & state, vector<int> & path, vector<int> & cycle) {
    
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
            if(DFS_cycle(v, u, g, state, path, cycle)) {
                return true;
            }
        }
    }
    state[u] = 2;
    path.pop_back();
    return false;
}

vector<int> num;
vector<int> state;
void dfs(int u, int p, const graph & g, int &c) {
    if(state[u])
    {
        return;
    }
    state[u] = true;
    num[u] = c;
    ++c;
    for(int v : g[u]) {
        if(v!= p) {
            dfs(v, u, g, c);
        }
    }
}

bool without_cycles(graph &g) {
    
    vector<int> state(g.size(), 0);
    vector<int> path;
    vector<int> cycle;
    bool has_loop = DFS_cycle(0, -1, g, state, path, cycle);

    if(has_loop) {
        return false;
    }
    cout << "Graph is without cycles" << endl;
    return true;
}

bool only_connectivity_component(graph& g) {
    for (size_t i = 0; i < g.size(); ++i) {
        if(g[i].size() > 1) {
            return false;
        }
    }
    cout << "Graph has only connectivity components!" << endl;
    return true;
}

vector<int> start_component(graph &g) {
    int max_vtx = 0;
    vector<int> ans(0);
    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            max_vtx = max(max_vtx, g[i][j]);
        }
    }

    // Graph reverse 
    graph tmp(max_vtx + 1);
    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            tmp[g[i][j]].push_back(i);
            // cout << i << " " << g[i][j] << " ";
        } cout << endl;
    }

    // Finish components for reversed graph
    vector<int> fcs = finish_component(tmp);
    return fcs;
}


vector<int> finish_component(graph &g) {
    vector<int> ans;
    for (size_t i = 0; i < g.size(); ++i){
        if(g[i].size() == 0) {
            ans.push_back(i);
            // cout << "Finish component: " << i << endl;
        }
    }
    return ans;
}