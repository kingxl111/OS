#include <iostream>
#include <unistd.h>

using namespace std;

#define MAX_LINE_SIZE 100

int str_to_int(char ar[]){
    int i {0};
    // cout << "ar: " <<  ar << endl;
    int result = 0;
    while((ar[i] != '\0') && (i < MAX_LINE_SIZE)) {
        // cout << ar[i] << endl;
        if((ar[i] < '0') || (ar[i] > '9')) {
            cout << "Invalid format of string\n";
            exit(-1);
        }
        result *= 10;
        result += (ar[i] - '0');
        ++i;
    }
    return result;
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
        cout << "Invalid args count\n";
        exit(-1);
    }
    
    int fd = str_to_int(argv[1]);
    char str[MAX_LINE_SIZE];
    read(fd, str, MAX_LINE_SIZE);
    close(fd);
    cout << "Translation result: " << str << endl;

    return 0;
}