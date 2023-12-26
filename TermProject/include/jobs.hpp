#pragma once
#include <iostream>

using namespace std;
using ll = int64_t;

ll GCD(ll a, ll b);
ll FibNumberRemainderCount(ll n);
void HanoiTowers(int n, char from_rod, char to_rod, char aux_rod);
ll Sum(ll a, ll b);
ll DumbFib(ll n);
void Void();


class IJob {
public:
    int id;
    bool is_done;
    IJob* next;
    virtual void do_job() {
        cout << "Job interface" << endl;
    }
    IJob() : id(0), is_done(false), next(nullptr) {};
    IJob(int _id) : id(_id), is_done(false), next(nullptr) {};
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