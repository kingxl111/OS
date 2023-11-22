#include <iostream>
#include <vector>
#include "GCD.hpp"

int NaiveGCD(int a, int b) {
    
    std::vector<int> a_div;
    std::vector<int> b_div;

    a_div.push_back(1);
    b_div.push_back(1);

    for(int i = 2; i <= a; ++i) {
        if(a % i == 0) {
            a_div.push_back(i);
        }
    }
    for(int i = 2; i <= b; ++i) {
        if(b % i == 0) {
            b_div.push_back(i);
        }
    }

    std::vector<int> gcds;
    for (size_t i = 0; i < a_div.size(); ++i) {
        for (size_t j = 0; j < b_div.size(); j++) {
            if(a_div[i] == b_div[j]) {
                gcds.push_back(a_div[i]);
            }
        }
    }

    int answer = 1;
    for (size_t i = 0; i < gcds.size(); ++i) {
        answer = std::max(answer, gcds[i]);
    }

    std::cout << "Naive GCD has worked" << std::endl;
    return answer;
}

int GCD(int a, int b){
    if((a == 0) || (b == 0)) {
        std::cout << "GCD has worked" << std::endl;
        return std::max(a, b) > 0 ? std::max(a, b) : 1;
    }
    return GCD(std::max(a, b) % std::min(a, b), std::min(a, b));
}