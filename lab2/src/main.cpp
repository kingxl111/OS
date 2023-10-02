#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

#define FILTER_MATRIX_SIZE 3

long long counter = 0;
pthread_mutex_t mutex;

double new_matrix[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE];

void convolution(double matrix[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE],  int i, int j, double filter[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE]){

    int n = FILTER_MATRIX_SIZE;
    int m = FILTER_MATRIX_SIZE;

    double result = 0;
    int filter_x = 0;
    for(int x = i - FILTER_MATRIX_SIZE / 2; x <= i + FILTER_MATRIX_SIZE / 2; ++x) {
        int filter_y = 0;
        for (int y = j - FILTER_MATRIX_SIZE / 2; y <= j + FILTER_MATRIX_SIZE / 2; ++y) {
            if((x >= 0) && (x <= n) && (y >= 0) && (y <= m)) {
                result += (matrix[x][y] * filter[filter_x][filter_y]);
            }
            ++filter_y;
        }
        ++filter_x;
    }
    new_matrix[i][j] = result;

    return;
}


int main(int argc, char* argv[]) {
    
    clock_t start = clock();

    int n, m;
    cout << "Input the matrix size" << endl << "n: ";
    cin >> n;
    cout << "m: ";
    cin >> m;
    cout << "Input the matrix" << endl;

    n = FILTER_MATRIX_SIZE;
    m = FILTER_MATRIX_SIZE;

    double users_matrix[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE];
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> users_matrix[i][j];
        }
    }

    double filter1[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};
    // double filter2[FILTER_MATRIX_SIZE][FILTER_MATRIX_SIZE];

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            convolution(users_matrix, i, j, filter1); 
        } 
    }

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            cout << new_matrix[i][j] << " ";
        } cout << endl;
    }


    cout << (double)(clock() - start ) / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}