#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>

#define MAX_LINE_LENGTH 1024
#define SHARED_LINE_NAME "shared_line"

int getln(char line[]);

int main() {

    printf("Enter the input line:\n");

    char input_line[MAX_LINE_LENGTH];
    int line_size = getln(input_line);

    int shared_line_fd;
    if((shared_line_fd = shm_open(SHARED_LINE_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR)) == -1) {
        perror("shm openning error!");
        exit(-1);
    }
    
    if (ftruncate(shared_line_fd, MAX_LINE_LENGTH) == -1 ) {
        perror("ftruncate error!");
        exit(-1);
    }

    char* shared_line = static_cast<char*>(mmap(NULL, 
                        MAX_LINE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, 
                        shared_line_fd, 0));

    if(shared_line == MAP_FAILED) {
        fprintf(stderr, "%s\n", strerror(errno));
        printf("MMAP error!\n");
        exit(-1);   
    }
    
    for (int i = 0; i < line_size; ++i) {
        shared_line[i] = input_line[i];
    } shared_line[line_size] = '\n';

    // printf("I'm here!\n");

    pid_t pid1 = fork();
    if(pid1 == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        printf("Fork creating error!\n");
        exit(-1);
    }

    if(pid1 == 0) {
        
        if(dup2(shared_line_fd, STDIN_FILENO) == -1) {
            perror("dup2 error");
            exit(-1);
        }
        
        if (execl("child1", "child1", SHARED_LINE_NAME, NULL) == -1) {
            perror("exec error!");
            exit(-1);
        }
    }
    else {
        wait(NULL);

        pid_t pid2 = fork();
        if(pid2 == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            printf("Fork creating error!\n");
            exit(-1);
        }

        if(pid2 == 0) {
            if(dup2(shared_line_fd, STDIN_FILENO) == -1) {
                perror("dup2 error");
                exit(-1);
            }

            if (execl("child2", "child2", SHARED_LINE_NAME, NULL) == -1) {
                perror("exec error!");
                exit(-1);
            }
        }
        else {
            wait(NULL);

            printf("Result line:\n");
            
            char output_line[MAX_LINE_LENGTH];
            if(read(shared_line_fd, output_line, line_size) != line_size) {
                perror("read error!");
                exit(-1);
            }

            for(int i = 0; i < line_size; ++i) {
                printf("%c", output_line[i]);
            } printf("\n");
        
            if(munmap(shared_line, MAX_LINE_LENGTH) != 0) {
                fprintf(stderr, "%s\n", strerror(errno));
                printf("MUNMAP error!\n");
                exit(-1);
            }
            shm_unlink(SHARED_LINE_NAME);    
        }
    }
    return 0;
}

int getln(char line[]) {
    char c;
    int counter = 0;
    while(((c = getchar()) != '\n') && (counter < MAX_LINE_LENGTH)) {
        line[counter] = c;
        ++counter;
    }
    return counter;
}
