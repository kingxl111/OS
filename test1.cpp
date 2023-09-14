#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE_SIZE 100
#define MAX_FD_LEN 10

int create_ps() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    return pid;
}

void int_to_str(int n, char result[]) {
    if(n < 0) {
        cout << "N < 0; ERROR!\n";
        exit(-1);
    }
    int i {0};
    // cout << "N: " << n << endl;
    while(n > 0) {
        result[i] = (char)((n % 10) + '0');
        n /= 10;
        ++i;
    }
    // cout << "promezh: " <<  result << endl;
    for(int j = 0; j < i / 2 + 1; ++j) {
        char tmp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = tmp;
    }
    result[i] = '\0'; 
}

int main(int argc, char* argv[]) {

    char str[MAX_LINE_SIZE] = {"Hello, child!!!"};

    int pipe_fd[2];
    int err = pipe(pipe_fd);
    if(err == -1) {
        perror("pipe");
        exit(-1);
    }
    pid_t pid = create_ps();
    if (pid == 0) {
        cout << "I'm a child" << endl;
        char fd_to_read[MAX_FD_LEN];
        int_to_str(pipe_fd[0], fd_to_read);
        
        execl("/home/vadim/Рабочий стол/OS subj/OS/child",
              "/home/vadim/Рабочий стол/OS subj/OS/child", fd_to_read, NULL);
        cout << "I'm a child one another time!" << endl; // Эта строка не выполнится!
    } else {
        cout << "I'm a parent" << endl;
        close(pipe_fd[0]);
        write(pipe_fd[1], str, MAX_LINE_SIZE);
        close(pipe_fd[1]);
        wait(NULL); 
        cout << "I have waited...\n";
    }
    cout << "Main programm has finished" << endl;
    return 0;
}