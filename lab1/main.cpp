#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#define MAX_LINE_LENGTH 100

using namespace std;

int create_ps() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    return pid;
}

void to_lower(char s[], int len) {
    // cout << "Function has been called\n";
    for(int i = 0; i < len; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)((int)s[i] + ('a' - 'A'));//в таблице ASCII нижний регистр стоит позже
        }
    }
}

void spaces_replace(char s[], int len) {
    for(int i = 0; i < len; ++i) {
        if (s[i] == ' ') {
            s[i] = '_';
        }
    }
}

void get_line(char line[]){
    char c;
    int i {0};
    while(((c = getchar()) != '\n') && (i < MAX_LINE_LENGTH)) {
        line[i] = c;
        ++i;
    }
} 

int main() {
    
    int pipe1_fd[2], pipe2_fd[2], btw_childs_pipe_fd[2];
    //pipe_fd[0] - for reading, pipe_fd[1] - for writing
    int err1 = pipe(pipe1_fd); 
    if (err1 == -1) {
        perror("pipe");
        exit(-1);
    }
    int err2 = pipe(btw_childs_pipe_fd); 
    if (err2 == -1) {
        perror("pipe");
        exit(-1);
    }
    int err3 = pipe(pipe2_fd); 
    if (err3 == -1) {
        perror("pipe");
        exit(-1);
    }
    char str_in[100];
    get_line(str_in);
    pid_t pid1 = create_ps(); //Создаем первого ребенка 
    if (pid1 == 0) { //child ps
        close(pipe1_fd[1]);
        char str[MAX_LINE_LENGTH];
        // cout << "I'm child\n";
        read(pipe1_fd[0], &str, MAX_LINE_LENGTH);
        close(pipe1_fd[0]);
        to_lower(str, MAX_LINE_LENGTH);
        // cout << "First child's result: " << str  << endl;
        //Sending data to the second child
        close(btw_childs_pipe_fd[0]);
        write(btw_childs_pipe_fd[1], str, MAX_LINE_LENGTH);
        close(btw_childs_pipe_fd[1]);
    } else { // parent ps
        close(pipe1_fd[0]);
        write(pipe1_fd[1], &str_in, sizeof(str_in) / sizeof(char));
        close(pipe1_fd[1]);
        //Creating new child
        pid_t pid2 = create_ps();
        if (pid2 == 0) { //child ps
            char str[MAX_LINE_LENGTH];

            close(btw_childs_pipe_fd[1]);
            read(btw_childs_pipe_fd[0], str, MAX_LINE_LENGTH);
            close(btw_childs_pipe_fd[0]);

            spaces_replace(str, MAX_LINE_LENGTH);

            close(pipe2_fd[0]);
            write(pipe2_fd[1], str, MAX_LINE_LENGTH);
            close(pipe2_fd[1]);
            // cout << "Second child's result: " << str << endl;
        } else { //parent ps
            char str_out[MAX_LINE_LENGTH];
            close(pipe2_fd[1]);
            read(pipe2_fd[0], str_out, MAX_LINE_LENGTH);
            close(pipe2_fd[0]);
            cout << "RESULT: " << str_out << endl;
        }
    }
    



    return 0;
}