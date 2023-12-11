#include <iostream>
#include <map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "jobs.hpp"
#include "jobs_validator.hpp"
#include <thread>

using namespace std;
using graph = vector<vector<int> >;

const string CONFIG = "config.yaml";

// enum Functions {
//     null,
//     _GCD,
//     _FibNumberRemainderCount,
//     _HanoiTower,
//     _Sum, 
//     _DumbFib,
//     _Void
// };


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

class IJob {
public:
    virtual void do_job() = 0;
};

class GCDJob: public IJob {
public:
    void do_job() override {
        cout << "GCD(1245324, 74152356) result: " << GCD(1245324, 74152356) << endl;
    }
};

class FibNumberRemainderCountJob: public IJob {
public:
    void do_job() override {
        cout << "FibNumberRemainderCount(100000) result: " << FibNumberRemainderCount(100000) << endl;
    }
};

class HanoiTowersJob: public IJob {
public:
    void do_job() override {
        cout << "HanoiTowers(10, 1, 3, 2) result: " << endl;
        HanoiTowers(10, '1', '3', '2');
    }
};

class SumJob: public IJob {
public:
    void do_job() override {
        cout << "Sum(51232425, 81241671) result: " << Sum(51232425, 81241671) << endl;
    }
};

class DumbFibJob: public IJob {
public:
    void do_job() override {
        cout << "DumbFib(30) result: " << DumbFib(30) << endl;
    }
};

class VoidJob: public IJob {
public:
    void do_job() override {
        cout << "Void result: " << endl;
    }
};

IJob *function_identifier(string fnc) {
    if(fnc == "GCD") {
        return new GCDJob;
    } else if (fnc == "FibNumberRemainderCount") {
        return new FibNumberRemainderCountJob;
    } else if (fnc == "HanoiTower") {
        return new HanoiTowersJob;
    } else if (fnc == "Sum") {
        return new SumJob;
    } else if(fnc == "DumbFib") {
        return new DumbFibJob;
    } else if(fnc == "Void") {
        return new VoidJob;
    } else {
        return nullptr;
    }

}

ll fact(ll n) {
    if(n <= 1) {
        return 1;
    }
    return fact(n - 1) * n;
}

auto main(int argc, char** argv) -> int {

    YAML::Node config = YAML::LoadFile(CONFIG);
    const YAML::Node& jobs = config["Jobs"];
    
    vector<string> jobs_tasks;

    // Config printing
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
        jobs_tasks.push_back(job["Job"].as<string>());
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
            vts[j] = (stoi(vtx_n) - 1);
        } 
        g[i] = vts;
    }

    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g[i].size(); ++j) {
            cout << g[i][j] << " ";
        } 
        cout << endl;
    }

    vector<int> start_jobs;
    vector<int> finish_jobs;

    try {
        if(!only_connectivity_component(g)) {
            throw std::logic_error("Graph' vertex hasn't the only one connectivity component");
            exit(-1);
        }
        if(!without_cycles(g)) {
            throw std::logic_error("Graph has cycle");
        }
        if((finish_jobs = finish_component(g)).size() <= 0) {
            throw std::logic_error("There is no finish component!");
        }
        if((start_jobs = start_component(g)).size() <= 0) {
            throw std::logic_error("There is no start component!");
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    // Multithreading beginning 
    int THREAD_COUNT = argc;

    cout << "Jobs' count: " << jobs_tasks.size() << endl;
    
    // vector<thread> t;
    try {
    
        for (size_t i = 0; (i < jobs_tasks.size()) /*&& (i < THREAD_COUNT)*/; ++i) {
            // cout << jobs_tasks[i] << endl;
            IJob* cur_job = function_identifier(jobs_tasks[i]);

            if(!cur_job) {
                throw std::logic_error("Invalid function name in the configure file!");
            }

            std::thread t1(&IJob::do_job, cur_job);

            t1.join();
            // t.push_back(t2);
            delete cur_job; 
        }
    } 
    catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }

    

    // for (size_t i = 0; i < t.size(); ++i) {
    //     t[i].join();
    // }
    

    return 0;
}
