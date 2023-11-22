#include <iostream>
#include <vector>
#include "primeCount.hpp"

int NaivePrimeCount(int a, int b) {
    int counter = 0;
    for (int i = a; i <= b; ++i) {
        bool is_prime = true;
        if(i == 0 || i == 1) {
            is_prime = false;
        }
        for(int j = 2; j < i; ++j) {
            if(i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if(is_prime) {
            ++counter;
        }
    }
    std::cout << "Naive prime counter has worked" << std::endl;
    return counter;
}

int EratosthenesSieve(int a, int b) {

    std::vector<bool> resheto(b + 1, true);
    if(b <= 1) { // a, b - natural
        return 0;
    }
    resheto[0] = false;
    resheto[1] = false;

    for (int prime = 2; prime * prime <= b; ++prime) {
        if(resheto[prime]) {
            for(int j = prime * prime; j <= b; j += prime) {
                resheto[j] = false;
            }
        }
    }
    int counter = 0;
    for (int i = a; i <= b; ++i) {
        if(resheto[i]) {
            // std::cout << i << std::endl;
            ++counter;
        }
    }
    std::cout << "Eratosthenes sieve has worked" << std::endl;
    return counter;
}

// void hello_library() {
//     std::cout << "Hello, my first dynamic library!" << std::endl;
// }

// void hello_world() {
//     std::cout << "Hello, world!" << std::endl;
// }