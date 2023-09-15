#include <iostream>
#include <unistd.h>

#define MAX_LINE_LENGTH 1000

using namespace std;

void spaces_replace(char s[], int len) {
    for(int i = 0; i < len; ++i) {
        if (s[i] == ' ') {
            s[i] = '_';
        }
    }
}

int main() {

    char str[MAX_LINE_LENGTH];
    read(STDIN_FILENO, str, MAX_LINE_LENGTH);
    spaces_replace(str, MAX_LINE_LENGTH);
    write(STDOUT_FILENO, str, MAX_LINE_LENGTH);

    return 0;
}