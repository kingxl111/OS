#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

long long counter = 0;
pthread_mutex_t mtx;

void* hello(void* prm) {

    long long local_sum = 0;
    for (long long i = 0; i < 100000000; ++i) {
        ++local_sum;
    }
    pthread_mutex_lock(&mtx);  
    counter += local_sum;  
    pthread_mutex_unlock(&mtx);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    
    clock_t start = clock();
    pthread_t t1, t2;
    pthread_mutex_init(&mtx, NULL);
    if (pthread_create(&t1, NULL, hello, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&t2, NULL, hello, NULL) != 0) {
        return 2;
    }
    if(pthread_join(t1, NULL) != 0) {
        return 3;
    }
    if(pthread_join(t2, NULL) != 0) {
        return 4;
    }
    pthread_mutex_destroy(&mtx);
    cout << counter << endl;
    cout << (double)(clock() - start ) / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}