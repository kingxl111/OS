#include <iostream>
#include <map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "jobs.hpp"
#include "jobs_validator.hpp"
#include <thread>
#include <queue>

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
    
    // map<int, IJob*> jobs_tasks;
    graph g;

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
            IJob* vtx = new IJob(stoi(cur_vertices_id[i]));
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


        // IJob* next_job = nullptr;
        // current_job->id = first_digit_in_string(_id) - 1;
        // jobs_tasks[current_job->id] = current_job;

    }
    
    vector<IJob*> start_jobs;
    vector<IJob*> finish_jobs;

    try {
        if(!only_connectivity_component(tmp_graph)) {
            throw std::logic_error("Graph' vertex hasn't the only one connectivity component");
        }
        if(!without_cycles(tmp_graph, Jobs)) {
            throw std::logic_error("Graph has cycle");
        }
        if((finish_jobs = finish_component(tmp_graph, Jobs)).size() <= 0) {
            throw std::logic_error("There is no finish component!");
        }
        // if((start_jobs = start_component(tmp_graph, Jobs)).size() <= 0) {
        //     throw std::logic_error("There is no start component!");
        // }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    // // Multithreading beginning 
    // int THREAD_COUNT = argc;

    // // cout << "Jobs' count: " << jobs_tasks.size() << endl;

    // for (size_t i = 0; i < start_jobs.size(); ++i) {
    //     queue<IJob*> cur_thread_tasks;
        
    // }
    
    try {
    
        // for (size_t i = 0; (i < jobs_tasks.size()) /*&& (i < THREAD_COUNT); ++i) {

            // cout << jobs_tasks[i] << endl;
            // IJob* cur_job = function_identifier(jobs_tasks[i], id);

            // if(!cur_job) {
            //     throw std::logic_error("Invalid function Job_id in the configure file!");
            // }

            // std::thread t1(&IJob::do_job, cur_job);

            // t1.join();
            // // t.push_back(t2);
            // delete cur_job; 
        // }
    } 
    catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }

    

    
    

    return 0;
}
