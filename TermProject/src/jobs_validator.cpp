#include "jobs_validator.hpp"
#include "jobs.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using graph = vector<vector<IJob*> >;

bool DFS_cycle(IJob* u, IJob* p, const graph & g, vector<int> & state, vector<IJob*> & path, vector<IJob*> & cycle) {
    
    if(u == nullptr || p == nullptr) {
        return false;
    }
    if(state[u->id] == 2) {
        return false;
    }

    path.push_back(u);
    state[u->id] = 1;
    for(auto v : g[u->id]) {
        if(p != v) {
            if(state[v->id] == 1) {
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
    state[u->id] = 2;
    path.pop_back();
    return false;
}

bool without_cycles(graph &g, vector<IJob*> Jobs) {
    
    vector<int> state(g.size(), 0);
    vector<IJob*> path;
    vector<IJob*> cycle;
    IJob* smt = new IJob(-1);
    for(size_t i = 0; i < g.size(); ++i) {
        
        bool has_cycle = DFS_cycle(Jobs[i], smt, g, state, path, cycle);
        if(has_cycle) {
            return false;
        }
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

vector<IJob*> start_component(graph &g, vector<IJob*> Jobs) {
    int max_vtx = 0;
    vector<IJob*> ans(0);
    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            max_vtx = max(max_vtx, g[i][j]->id);
        }
    }

    // Graph reverse 
    graph tmp(max_vtx + 1);
    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            tmp[g[i][j]->id].push_back(new IJob(i));
            // cout << i << " " << g[i][j] << " ";
        } cout << endl;
    }

    // Finish components for reversed graph
    vector<IJob*> fcs = finish_component(tmp, Jobs);
    return fcs;
}


vector<IJob*> finish_component(graph &g, vector<IJob*> Jobs) {
    vector<IJob*> ans;
    for (size_t i = 0; i < g.size(); ++i)  {
        // cout << "job vtc size: " << g[i].size() << endl;
        if(g[i].size() == 0) {
            ans.push_back(Jobs[i]);
            cout << "Finish component: " << i << endl;
        }
    }

    return ans;
}