#include <stdlib.h>
#include <prodcons.h>
#include <shell.h>

static int end = PRODUCERS;

void producer()
{
  char i;
  for(i = 'a'; i <= 'z'; i++) {
    lockMutex("__PRODCONS_MUTEX__");
    printf("%c", i);
    unlockMutex("__PRODCONS_MUTEX__");
  }
	lockMutex("__PRODCONS_MUTEX__");
	end++;
	unlockMutex("__PRODCONS_MUTEX__");
}


void consumer()
{
  char* c = malloc(sizeof(char));
  while(1) {
    lockMutex("__PRODCONS_MUTEX__");
    int80(getStdin(), (uint64_t) c, 1, 0, 0, SYS_READ);
    unlockMutex("__PRODCONS_MUTEX__");
  }
}



void prodcons()
{
  createMutex("__PRODCONS_MUTEX__");
	int pids[PRODUCERS] = {0};
    int pids_consumers[CONSUMERS] = {0};
  int* pipe = createPipe();
  switchFd(STDOUT, pipe[1]);
  for(int i = 0; i < PRODUCERS; i++){
		//printf("created prod\n");
    char *n = malloc(sizeof(char)*2);
    intToString(i, n);
    pids[i] = start_proc_user(concat("producer",n), (void*)producer, 0,0, 10);
  }
  switchFd(STDOUT, STDOUT);
  switchFd(STDIN, pipe[0]);
  for(int i = 0; i < CONSUMERS; i++){
		printf("created cons\n");
    char *n = malloc(sizeof(char)*2);
    intToString(i, n);
    pids_consumers[i] = start_proc_user(concat("consumer", n), (void*)consumer, 0, 0, 10);
  }
  switchFd(STDIN, STDIN);
	//while(end){;}
	for(int i = 0; i < PRODUCERS; i++)
		kill(pids[i]);
    for (int i = 0; i < CONSUMERS; i++)
        kill(pids_consumers[i]);
  destroyPipe(pipe[0]);
}
