#include <iostream>
#include "jobs.hpp"

using namespace std;

ll GCD(ll a, ll b) {
    if(a == 0 || b == 0) {
        return max(a, b);
    }
    return GCD(max(a, b) % min(a, b), min(a, b));
}

ll FibNumberRemainderCount(ll n) {

    if(n == 0) {
        return 0;
    }
    ll f_prev = 0;
    ll f_cur = 1;
    for (ll i = 0; i < n - 1; ++i) {
        ll tmp = (f_cur + f_prev) % 10;
        f_prev = f_cur % 10;
        f_cur = tmp % 10;
    }
    return f_cur;
}

void HanoiTowers(int n, char from_rod, char to_rod, char aux_rod) { 
    if (n == 0) { 
        return; 
    } 
    HanoiTowers(n - 1, from_rod, aux_rod, to_rod); 
    cout << "We shift the ring from " << from_rod << " rod to " << to_rod << " rod..." << endl; 
    HanoiTowers(n - 1, aux_rod, to_rod, from_rod); 
} 

ll Sum(ll a, ll b) {
    return a + b;
}

ll DumbFib(ll n) {
    if(n <= 1) {
        return 1;
    }
    return DumbFib(n-1) + DumbFib(n-2);
}

void SOmanyHorses() {
    cout << "*VOID*" << endl;
    return;
}