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

void spaces_replace(char s[], int len) {
    for(int i = 0; i < len; ++i) {
        if (s[i] == ' ') {
            s[i] = '_';
        }
    }
}

int main(int argc, char** argv) {

    if(argc != 2) {
        perror("Invalid arguments count!");
        exit(-1);
    }

    int shared_line_fd;
    if((shared_line_fd = shm_open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR)) == -1) {
        perror("shm openning error!");
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

    spaces_replace(shared_line, MAX_LINE_LENGTH);
    
    return 0;
}