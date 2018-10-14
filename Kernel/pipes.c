#include <pipes.h>
#include <listADT.h>
#include <buddy.h>

void freeFds(int readfd, int writefd);
int findFreeWritefd();
int findFreeReadfd();

listADT pipes = 0;
int readfds[MAX_PIPES] = {0};
int writefds[MAX_PIPES] = {0};

int pipeId(void* pipe, void *id){
	return ((Pipe) pipe)->fds[0] == *((int*)id) 
			|| ((Pipe) pipe)->fds[1] == *((int*)id);
}

void initPipes(){
	pipes = createListL(pipeId, sizeof(pipe));
}

int *createPipe(){
	Pipe p = mymalloc(sizeof(pipe));
	int readfd = findFreeReadfd();
	int writefd = findFreeWritefd();
	if(readfd == -1 || writefd == -1) return 0;
	p->index = 0;
	p->fds[0] = readfd;
	p->fds[1] = writefd;
	addL(pipes, p);
	return p->fds;
}

void destroyPipe(int fd){
	Pipe p = getL(pipes, &fd);
	freeFds(p->fds[0], p->fds[1]);
	removeAndFreeL(pipes, &fd);
}

int writeToPipe(const char *message, int size, int fd){
	Pipe p = getL(pipes, &fd);
	if(p->index + size >= PIPE_SIZE)
		return 0;
	int i;
	for(i = 0; i < size; i++)
		p->buffer[p->index + i + 1] = message[i];
	p->index += i;
	return 1;
}

int readFromPipe(int fd, char *buffer){
	Pipe p = getL(pipes, &fd);
	for(int i = 0; i < p->index + 1; i++)
		buffer[i] = p->buffer[i];
	int aux = p->index;
	p->index = 0;
	return aux;
}

int isPipe(int fd){
	return(containsL(pipes, &fd));
}

int findFreeWritefd(){
	for(int i = 0; i < MAX_PIPES; i++)
		if(writefds[i] == 0)
			return i;
	return -1;
}

int findFreeReadfd(){
	for(int i = 0; i < MAX_PIPES; i++)
		if(readfds[i] == 0)
			return i + MAX_PIPES;
	return -1;
}

void freeFds(int readfd, int writefd){
	writefds[writefd] = 0;
	readfds[readfd - MAX_PIPES] = 0;
}
