#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include <yaml-cpp/yaml.h>
#include "jobs.hpp"
#include "jobs_validator.hpp"

using namespace std;
using graph = vector<vector<IJob*> >;

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

IJob* function_identifier(string fnc) {
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
    } else if(fnc == "SOmanyHorses") {
        return new VoidJob;
    } else {
        return nullptr;
    }

}

auto main(int argc, char** argv) -> int {

    YAML::Node config = YAML::LoadFile(CONFIG);
    const YAML::Node& jobs = config["Jobs"];


    // Config printing
    vector<vector<IJob*> > tmp_graph;
    vector<IJob*> Jobs;
    for (YAML::const_iterator it = jobs.begin(); it != jobs.end(); ++it) {
        const YAML::Node& job = *it;
        string cur_job_id = job["Job_id"].as<string>();
        cout << "Job_id: " << cur_job_id << endl;
        const int _id = stoi(cur_job_id);
        
        cout << "Vertices: ";
        vector<IJob*> cur_vertices;
        vector<string> cur_vertices_id = job["Vertices"].as<vector<string> >();
        for (size_t i = 0; i < cur_vertices_id.size(); ++i) {
            cout << cur_vertices_id[i] << ", ";
            IJob* vtx = new IJob(stoi(cur_vertices_id[i]) - 1);
            cur_vertices.push_back(vtx);
        } cout << endl;

        tmp_graph.push_back(cur_vertices);

        cout << "Job: " << job["Job"].as<string>() << endl;
        IJob* current_job = function_identifier(job["Job"].as<string>());
        if(!current_job) {
            cout << "Wrong job function name!" << endl;
            exit(-1);
        }
        current_job->id = _id - 1;
        Jobs.push_back(current_job);

    }

    for(size_t i = 0; i < tmp_graph.size(); ++i) {
        for (size_t j = 0; j < tmp_graph[i].size(); ++j) {
            tmp_graph[i][j] = Jobs[tmp_graph[i][j]->id];
        }
    }

    for (size_t i = 0; i < Jobs.size(); ++i) {
        if(tmp_graph[i].size() < 1) {
            continue;
        }
        Jobs[i]->next = tmp_graph[i][0];
    }

    // Graph validation
    vector<IJob*> start_jobs;
    vector<IJob*> finish_jobs;

    try {
        if(!only_connectivity_component(tmp_graph)) {
            throw std::logic_error("Graph' vertex hasn't the only one connectivity component");
        }
        if((finish_jobs = finish_component(tmp_graph, Jobs)).size() <= 0) {
            throw std::logic_error("There is no finish component!");
        }
        if(!without_cycles(tmp_graph, Jobs, finish_jobs)) {
            throw std::logic_error("Graph has cycle");
        }
        if((start_jobs = start_component(tmp_graph, Jobs)).size() <= 0) {
            throw std::logic_error("There is no start component!");
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    queue<IJob*> tasks;
    for (size_t i = 0; i < start_jobs.size(); ++i) {
        tasks.push(start_jobs[i]);
    }

    while(!tasks.empty()) {

        IJob* cur_job = tasks.front();
        tasks.pop();

        if(!cur_job) {
            cout << "Nullpointer in queue!" << endl;
            break;
        }
        if(cur_job->next) {
            tasks.push(cur_job->next);
        }
        if(cur_job->is_done) {
            continue;
        }
        cout << "Current job id: " << cur_job->id + 1 << endl;
        cur_job->do_job();
        sleep(1);
    }

    return 0;
}
