#include <iostream>
#include <unistd.h>

#define MAX_LINE_LENGTH 1000

using namespace std;

void to_lower(char s[], int len) {
    // cout << "Function has been called\n";
    for(int i = 0; i < len; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)((int)s[i] + ('a' - 'A'));//в таблице ASCII нижний регистр стоит позже
        }
    }
}

int main() {

    char str[MAX_LINE_LENGTH];
    read(STDIN_FILENO, str, MAX_LINE_LENGTH);
    to_lower(str, MAX_LINE_LENGTH);
    write(STDOUT_FILENO, str, MAX_LINE_LENGTH);


    return 0;
}