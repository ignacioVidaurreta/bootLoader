#ifndef PIPES_H
#define PIPES_H
#include <buddy.h>
#define PIPE_SIZE 1000
#define MAX_PIPES 100

typedef struct {
	int fds[2];
	char buffer[PIPE_SIZE];
	int index;
} pipe;

typedef pipe *Pipe;

#endif