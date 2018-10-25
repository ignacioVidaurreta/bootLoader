#include <stdlib.h>
#include <prodcons.h>
#include <shell.h>
#define MAX 10000

static int keep = 1;
static int cant = 0;

void producer()
{
	while(keep){
		lockMutex("__PRODCONS_MUTEX__");
	  char i='x';
	  if(cant < MAX) {
	    printf("%c", i);
			cant++;
	  }
		unlockMutex("__PRODCONS_MUTEX__");
	}
}


void consumer()
{
  char* c = malloc(sizeof(char));
  while(keep) {
    lockMutex("__PRODCONS_MUTEX__");
		if(cant>0){
			int80(getStdin(), (uint64_t) c, 1, 0, 0, SYS_READ);
			cant--;
		}
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
    char *n = malloc(sizeof(char)*2);
    intToString(i, n);
    pids[i] = start_proc_user(concat("producer",n), (void*)producer, 0,0, 10);
  }
  switchFd(STDOUT, STDOUT);


  switchFd(STDIN, pipe[0]);
  for(int i = 0; i < CONSUMERS; i++){
    char *n = malloc(sizeof(char)*2);
    intToString(i, n);
    pids_consumers[i] = start_proc_user(concat("consumer", n), (void*)consumer, 0, 0, 10);
  }
  switchFd(STDIN, STDIN);


	while(keep){
		lockMutex("__PRODCONS_MUTEX__");
		int q = readChar();
		if(q == 'q')
			keep = 0;
		unlockMutex("__PRODCONS_MUTEX__");
	}


	for(int i = 0; i < PRODUCERS; i++)
		kill(pids[i]);
  for (int i = 0; i < CONSUMERS; i++)
    kill(pids_consumers[i]);
  destroyPipe(pipe[0]);
}
