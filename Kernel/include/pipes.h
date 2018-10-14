#ifndef PIPES_H
#define PIPES_H
#define PIPE_SIZE 1000
#define MAX_PIPES 100
#define PIPE_FAILURE -1
#define PIPE_SUCCESS 1

// Please be aware that for functions that receive the
// file descriptor, either the read or the write fd can
// be used, as they both identify the pipe unequivocally.

typedef struct {
	int fds[2];
	char buffer[PIPE_SIZE];
	int index;
} pipe;

typedef pipe *Pipe;

int *createPipe();
int writeToPipe(const char *message, int size, int fd);
int readFromPipe(char *buffer, int count, int fd);
void destroyPipe(int fd);
int isPipe(int fd);

#endif