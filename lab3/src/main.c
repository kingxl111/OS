#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PAGESIZE 4096
#define MAX_LINE_LENGTH PAGESIZE


int getln(char line[]);
void to_lower(char s[], int len); // for child1
void spaces_replace(char s[], int len); // for child2

int main() {

    printf("Enter the input line:\n");

    char input_line[MAX_LINE_LENGTH];
    int line_size = getln(input_line);

    char* shared_line = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if(shared_line == MAP_FAILED) {
        fprintf(stderr, "%s\n", strerror(errno));
        printf("MMAP error!\n");
        exit(-1);
    }
    
    for (int i = 0; i < line_size; ++i) {
        shared_line[i] = input_line[i];
    }

    pid_t pid1 = fork();
    if(pid1 == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        printf("Fork creating error!\n");
        exit(-1);
    }

    if(pid1 == 0) {
        to_lower(shared_line, line_size);

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
            spaces_replace(shared_line, line_size);
        }
        else {
            wait(NULL);
            printf("Result line:\n");
            for(int i = 0; i < line_size; ++i) {
                printf("%c", shared_line[i]);
            } printf("\n");
        }
    }

    if(munmap(shared_line, PAGESIZE) != 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        printf("MUNMAP error!\n");
        exit(-1);
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

void to_lower(char s[], int len) {
    for(int i = 0; i < len; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)((int)s[i] + ('a' - 'A')); 
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
