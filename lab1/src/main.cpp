#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funcs.h"

#define MAX_LINE_LENGTH 1000

using namespace std;

int main() {

    int pipe1_fd[2], pipe2_fd[2], btw_childs_pipe_fd[2];
    //pipe_fd[0] - for reading, pipe_fd[1] - for writing
    create_pipe(pipe1_fd);
    create_pipe(btw_childs_pipe_fd);
    create_pipe(pipe2_fd);

    char str_in[MAX_LINE_LENGTH];
    write(STDOUT_FILENO, "Enter input string: ", 21);
    read(STDIN_FILENO, str_in, MAX_LINE_LENGTH);

    pid_t pid1 = create_ps(); //Создаем первого ребенка 
    if (pid1 == 0) { //child ps

        close(pipe1_fd[1]);
        close(btw_childs_pipe_fd[0]);

        if (dup2(pipe1_fd[0], STDIN_FILENO) == -1) {
            perror("dup2 error");
            exit(-1);
        }
        if (dup2(btw_childs_pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 error");
            exit(-1);
        }

        if (execl("child1", "child1", NULL) == -1) {
            perror("exec error!");
            exit(-1);
        }
        
    } else { // parent ps
        close(pipe1_fd[0]);
        write(pipe1_fd[1], str_in, MAX_LINE_LENGTH);
        close(pipe1_fd[1]);
        
        //Creating new child
        pid_t pid2 = create_ps();
        if (pid2 == 0) { //child ps
            close(btw_childs_pipe_fd[1]);

            if (dup2(btw_childs_pipe_fd[0], STDIN_FILENO) == -1) {
                perror("dup2 error");
                exit(-1);    
            }
            if (dup2(pipe2_fd[1], STDOUT_FILENO) == -1) {
                perror("dup2 error");
                exit(-1);
            }   
            if(execl("child2", "child2", NULL) == -1) {
                perror("exec error!");
                exit(-1);
            }
        }
        else { //parent ps
            char str_out[MAX_LINE_LENGTH];
            close(pipe2_fd[1]);
            read(pipe2_fd[0], str_out, MAX_LINE_LENGTH);
            close(pipe2_fd[0]);
            cout << "RESULT: " << str_out ;
        }
    }

    return 0;
}