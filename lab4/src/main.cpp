#include <iostream>
#include <dlfcn.h>
#include "primeCount.hpp"
#include "GCD.hpp"

using namespace std;

char dl2_name[] = "libGCDLib.so";
char dl1_name[] = "libPrimeCountLib.so";

char naive_alg_lib1[] = "NaivePrimeCount";
char efficient_alg_lib1[] = "EratosthenesSieve";
char naive_alg_lib2[] = "NaiveGCD";
char efficient_alg_lib2[] = "GCD";


int main() {

    // dynamic libraries openning 
    void* lib1_handle = dlopen(dl1_name, RTLD_LAZY);
    void* lib2_handle = dlopen(dl2_name, RTLD_LAZY);

    //error checking
    if (!lib1_handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    if (!lib2_handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    typedef int (*sym_t)(int, int); // function args after pointer

    // reset errors
    dlerror();

    int algorithm_switcher = 1;

    while(true) { // study lab moment
        cout << "Enter command(0, 1, 2): ";
        int command, a, b, result;

        if (!(cin >> command)) {
            cout << endl;
            cout << "===================Completion of work===================" << endl;
            dlclose(lib1_handle);
            dlclose(lib2_handle);
            break;
        }

        const char* dlsym_error;

        switch (command) {
        case 0:
            algorithm_switcher = algorithm_switcher == 1 ? 2 : 1;
            cout << "Algorithm mode changed" << endl;
            break;

        case 1:
            sym_t lib1_function;
            if(algorithm_switcher == 1) {
                lib1_function = (sym_t) dlsym(lib1_handle, naive_alg_lib1);
            }
            else {
                lib1_function = (sym_t) dlsym(lib1_handle, efficient_alg_lib1);
            }
            dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol from library: " << dl1_name << dlsym_error << endl;
                dlclose(lib1_handle);

                return 1;
            }
            cout << "Enter two numbers separated by space: ";
            cin >> a >> b;
            result = (*lib1_function)(a, b); 
            cout << "Lib1 algorithm result: " << result << endl;
            break;
        
        case 2:
            sym_t lib2_function;
            if(algorithm_switcher == 1) {
                lib2_function = (sym_t) dlsym(lib2_handle, naive_alg_lib2);
            }
            else {
                lib2_function = (sym_t) dlsym(lib2_handle, efficient_alg_lib2);
            }
            dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol from library: " << dl2_name << dlsym_error << endl;
                dlclose(lib2_handle);
                return 1;
            }

            cout << "Enter two numbers separated by space: ";
            cin >> a >> b;
            result = (*lib2_function)(a, b); 
            cout << "Lib2 algorithm result: " << result << endl;
            
            break;

        default:
            break;
        }

        cout << "--------------------------------------------------------" << endl;
    }

    return 0;
}

