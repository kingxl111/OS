#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <vector>

using namespace std;

#define FILTER_MATRIX_SIZE 3

struct pthread_data {
    int n;
    int m;
    double** user_matrix; 
    double** filter;
    double** result;
    int i_start;
    int j_start;
    int i_finish;
    int j_finish;
};


pthread_mutex_t mutex;

void* convolution(void* data){

    pthread_data* pdata = (pthread_data*)data;

    int n = pdata->n;
    int m = pdata->m;
    double** mtx = pdata->user_matrix;
    double** filter = pdata->filter;
    double** new_matrix = pdata->result;
    int i_start = pdata->i_start;
    int j_start = pdata->j_start;
    int i_finish = pdata->i_finish;
    int j_finish = pdata->j_finish;

    
    if(n == 0) {
        return nullptr;
    }

    for(int i = i_start; i <= i_finish; ++i) {    
        for(int j = j_start; j <= j_finish; ++j) {    
            double result = 0;
            int filter_x = 0;
            for(int x = i - FILTER_MATRIX_SIZE / 2; x <= i + FILTER_MATRIX_SIZE / 2; ++x) {
                int filter_y = 0;
                for (int y = j - FILTER_MATRIX_SIZE / 2; y <= j + FILTER_MATRIX_SIZE / 2; ++y) {
                    if((x >= 0) && (x < n) && (y >= 0) && (y < m) 
                    && ((filter_x < FILTER_MATRIX_SIZE) && (filter_y < FILTER_MATRIX_SIZE))) {
                        // pthread_mutex_lock(&mutex);
                        result += (mtx[x][y] * filter[filter_x][filter_y]);
                        // pthread_mutex_unlock(&mutex);
                    }
                    ++filter_y;
                }
                ++filter_x;
            }
            // pthread_mutex_lock(&mutex);
            new_matrix[i][j] = result;
            // pthread_mutex_unlock(&mutex);
            
        }
    }
    pthread_exit(0);
    return nullptr;
}


int main(int argc, char* argv[]) {
    
    if(argc != 2) { 
        cout << "wrong arguments count" << endl;
        return 1;
    }

    int k;
    cout << "Input the K number: ";
    cin >> k;

    int n, m;
    cout << "Input the matrix size" << endl << "n: ";
    cin >> n;
    cout << "m: ";
    cin >> m;
    cout << "Input the matrix" << endl;

    double** user_matrix = (double**) malloc(n * sizeof(double*));
    for(int i = 0; i < n; ++i) {
        user_matrix[i] = (double*) malloc(m * sizeof(double));
    }
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> user_matrix[i][j];
        }
    }
    cout << endl;
    clock_t start = clock();

    double** result = (double**) malloc(n * sizeof(double*));
    for(int i = 0; i < n; ++i) {
        result[i] = (double*) malloc(m * sizeof(double));
    }

    double** filter1 = (double**) malloc(FILTER_MATRIX_SIZE * sizeof(double*));
    for(int i = 0; i < n; ++i) {
        filter1[i] = (double*) malloc(m * sizeof(double));
    }
    for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
        for(int j = 0; j < FILTER_MATRIX_SIZE; ++j) {
            filter1[i][j] = 1;
        }
    }

    pthread_data pdata;
    pdata.n = n;
    pdata.m = m;
    pdata.filter = filter1;
    pdata.result = result;
    pdata.user_matrix = user_matrix;


    
    const int thread_count = stoi(argv[1]);
    // cout << "Thread count: " << thread_count << endl;
    pthread_t tid[thread_count];
    pthread_mutex_init(&mutex, nullptr);

    int i_start = 0;
    int i_finish = i_start + n / thread_count - 1;
    int j_start = 0;
    int j_finish = m - 1;
    for(int i = 0; i < thread_count; ++i) {
        pdata.i_start = i_start;
        pdata.i_finish = i_finish;
        pdata.j_start = j_start;
        pdata.j_finish = j_finish;
        cout << "i_start: " << i_start << ", i_finish: " << i_finish << ", j_start: " << j_start << ", j_finish: " << j_finish << endl;
        i_start = (i_finish + 1);
        i_finish = (i_start + n / thread_count);
        if(i_start >= n) {
            i_start = n - 1;
        }
        if(i_finish >= n) {
            i_finish = n - 1;
        }
        if(pthread_create(&tid[i], nullptr, convolution, &pdata) != 0) {
            return 1;
        }
    }
    for(int i = 0; i < thread_count; ++i) {
        if(pthread_join(tid[i], nullptr) != 0) {
            return 1;
        }
    }
    

    //The first arg is a number of threads

    // cout << "OK" << endl;
    // for (int i = 0; i < n; ++i) {
    //     for(int j = 0; j < m; ++j) {
    //         convolution(users_matrix, i, j, filter1, result); 
    //     } 
    // }

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            cout << result[i][j] << " ";
        } cout << endl;
    }

    pthread_mutex_destroy(&mutex);

    free(user_matrix);
    free(filter1);
    free(result);

    cout << (double)(clock() - start ) / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}