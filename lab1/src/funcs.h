#pragma once

#include <iostream>

int create_ps();
void create_pipe(int pipe_fd[2]);
void get_line(char line[]);
