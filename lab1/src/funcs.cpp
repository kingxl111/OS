#include <iostream>
#include <unistd.h>

#define MAX_LINE_LENGTH 1000

using namespace std;

int create_ps() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    return pid;
}

void create_pipe(int pipe_fd[2]) {
    if(pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(-1);
    }
}

void get_line(char line[]){
    read(STDIN_FILENO, line, MAX_LINE_LENGTH);
} 