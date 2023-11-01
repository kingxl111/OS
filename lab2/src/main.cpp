#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

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

void* convolution(void* data){

    pthread_data* pdata = (pthread_data*)data;
    
    if(pdata->n == 0) {
        return nullptr;
    }

    for(int i = pdata->i_start; i <= pdata->i_finish; ++i) {    
        for(int j = pdata->j_start; j <= pdata->j_finish; ++j) {    
            double result = 0;
            int filter_x = 0;
            for(int x = i - FILTER_MATRIX_SIZE / 2; x <= i + FILTER_MATRIX_SIZE / 2; ++x) {
                int filter_y = 0;
                for (int y = j - FILTER_MATRIX_SIZE / 2; y <= j + FILTER_MATRIX_SIZE / 2; ++y) {
                    if((x >= 0) && (x < pdata->n) && (y >= 0) && (y < pdata->m) 
                    && ((filter_x < FILTER_MATRIX_SIZE) && (filter_y < FILTER_MATRIX_SIZE))) {
                        result += (pdata->user_matrix[x][y] * pdata->filter[filter_x][filter_y]);
                    }
                    ++filter_y;
                }
                ++filter_x;
            }
            pdata->result[i][j] = result;
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

    double** result = (double**) malloc(n * sizeof(double*));
    for(int i = 0; i < n; ++i) {
        result[i] = (double*) malloc(m * sizeof(double));
    }

    const int thread_count = stoi(argv[1]);

    // data for threads 
    pthread_data pdata[thread_count];
    for(int i = 0; i < thread_count; ++i) {
        pdata[i].n = n;
        pdata[i].m = m;
        pdata[i].result = result;
        pdata[i].user_matrix = user_matrix;
    }

    pid_t pid = fork();
    if(pid == -1) {
        exit(0);
    }
    
    if(pid > 0) {

        // filter1 initialization
        double** filter1 = (double**) malloc(FILTER_MATRIX_SIZE * sizeof(double*));
        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            filter1[i] = (double*) malloc(FILTER_MATRIX_SIZE * sizeof(double));
        }
        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            for(int j = 0; j < FILTER_MATRIX_SIZE; ++j) {
                filter1[i][j] = 1;
            }
        }

        for(int i = 0; i < thread_count; ++i) {
            pdata[i].filter = filter1;
        }
        // threads array creating
        pthread_t tid[thread_count];

        // division task by threads
        int i_start = 0;
        int i_finish = (i_start + n / thread_count - 1) > 0 ? i_start + n / thread_count - 1 : 0;
        int j_start = 0;
        int j_finish = m - 1;
        for(int i = 0; i < thread_count; ++i) {
            pdata[i].i_start = i_start;
            pdata[i].i_finish = i_finish;
            pdata[i].j_start = j_start;
            pdata[i].j_finish = j_finish;

            i_start = (i_finish + 1);
            i_finish = (i_start + n / thread_count);
            if(i_start >= n) {
                i_start = n - 1;
            }
            if(i_finish >= n) {
                i_finish = n - 1;
            }
            
            if(pthread_create(&tid[i], NULL, convolution, &pdata[i]) != 0) {
                return 1;
            }
        }
        for(int i = 0; i < thread_count; ++i) {
            if(pthread_join(tid[i], NULL) != 0) {
                return 1;
            }
        }
            
        wait(nullptr);
        for (int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                cout << result[i][j] << " ";
            } cout << endl;
        }

        for(int i = 0; i < n; ++i) {
            free(user_matrix[i]);
            free(result[i]);
        }
        free(user_matrix);
        free(result);

        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            free(filter1[i]);
        }
        free(filter1);

    }
    else {
        // filter2 initialization
        double** filter2 = (double**) malloc(FILTER_MATRIX_SIZE * sizeof(double*));
        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            filter2[i] = (double*) malloc(FILTER_MATRIX_SIZE * sizeof(double));
        }
        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            for(int j = 0; j < FILTER_MATRIX_SIZE; ++j) {
                filter2[i][j] = 10;
            }
        }

        // threads array creating
        pthread_t tid[thread_count];

        for(int i = 0; i < thread_count; ++i) {
            pdata[i].filter = filter2;
        }

        // division task by threads
        int i_start = 0;
        int i_finish = (i_start + n / thread_count - 1) > 0 ? i_start + n / thread_count - 1 : 0;
        int j_start = 0;
        int j_finish = m - 1;
        for(int i = 0; i < thread_count; ++i) {
            pdata[i].i_start = i_start;
            pdata[i].i_finish = i_finish;
            pdata[i].j_start = j_start;
            pdata[i].j_finish = j_finish;

            i_start = (i_finish + 1);
            i_finish = (i_start + n / thread_count);
            if(i_start >= n) {
                i_start = n - 1;
            }
            if(i_finish >= n) {
                i_finish = n - 1;
            }
            
            if(pthread_create(&tid[i], NULL, convolution, &pdata[i]) != 0) {
                return 1;
            }
        }
        for(int i = 0; i < thread_count; ++i) {
            if(pthread_join(tid[i], NULL) != 0) {
                return 1;
            }
        }

        for (int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                cout << result[i][j] << " ";
            } cout << endl;
        } cout << endl;

        for(int i = 0; i < n; ++i) {
            free(user_matrix[i]);
            free(result[i]);
        }

        for(int i = 0; i < FILTER_MATRIX_SIZE; ++i) {
            free(filter2[i]);
        }

        free(user_matrix);
        free(result);
        free(filter2);

    }
    return 0;
}