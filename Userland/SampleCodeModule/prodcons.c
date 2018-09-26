#include <stdlib.h>
#include <prodcons.h>
#define MUTEX_ID 1

static char *prodconsMsgQueueId = "prodConsMsgQueue";
static char *writerMsgQueueId = "creationMsgQueue";

int prodcons(int startingReaders, int startingWriters){
	createMessageQueue(prodconsMsgQueueId);
	createMessageQueue(creationMsgQueueId);
	createProcesses("reader", startingReaders);
	createProcesses("writer", startingWriters);
	while(message != "exit"){
		char *message = (char*)receiveMessage(creationMsgQueue);
		createProcesses(message, *((int*)receiveMessage(creationMsgQueue)));
	}
	cleanup();
}

void createProcesses(char* processes, int number){
	if(strcmp(processes, "reader") == 0)
		for(int i = 0; i < number; i++)
			startProcess("reader", &reader);
	else if(strcmp(processes, "writer") == 0)
		for(int i = 0; i < number; i++)
			startProcess("writer", &writer);
}

void cleanup(){}

int reader(){
	int messagesRead = 0;
	while(strcmp(message, "exit") != 0){

	}
}

int writer(){

}

// para recordar cosas que faltan:
// no creo poder hacer esto sin una manera de matar un proceso, por lo que necesito una syscall
// para eso.