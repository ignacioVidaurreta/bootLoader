#include <stdlib.h>
#include <prodcons.h>
#define MUTEX_ID 1

static char *prodconsMsgQueueId = "prodConsMsgQueue";
static char *prodconsMutexId = "prodConsMutex";

int prodcons(int startingReaders, int startingWriters){
	char *creationMsgQueueId = "creationMsgQueue";
	int currentReaders = 0;
	int currentWriters = 0;
	int readerPids[MAX_READERS] = {0};
	int writerPids[MAX_WRITERS] = {0};
	createMessageQueue(prodconsMsgQueueId);
	createMessageQueue(creationMsgQueueId);
	createProcesses("reader", startingReaders, &currentReaders, readerPids);
	createProcesses("writer", startingWriters, &currentWriters, writerPids);
	while(message != "exit"){
		char *message = (char*)receiveMessage(creationMsgQueueId);
		int *agent = strcmp(message,"reader") == 0 ? &currentReaders:&currentWriters;
		int *pids = strcmp(message,"reader") == 0 ? readerPids:writerPids;
		createProcesses(message, *((int*)receiveMessage(creationMsgQueueId)), agent, pids);
	}
	cleanup(writerPids, readerPids, currentWriters, currentReaders);
}

void changeInProcesses(char* processes, int number, int *prevNumber, int *pids){
	if(strcmp(processes, "reader") == 0){
		if(number > 0 && number + *prevNumber < MAX_READERS)
			for(int i = 0; i < number; i++)
				startProcess("reader", &reader);
		else if(number < 0 && *prevNumber + number > 0)
			for(int i = 0; i < -number; i++)
				//killProcess(pids[*prevNumber - i])
	}
	else if(strcmp(processes, "writer") == 0){
		if(number > 0 && number + *prevNumber < MAX_WRITERS)
			for(int i = 0; i < number; i++)
				startProcess("writer", &writer);
		else if(number < 0 && number + *prevNumber > 0)
			for(int i = 0; i > number; i--)
				//killProcess(pids[*prevNumber + i]);
	}
	*prevNumber += number;
}

void cleanup(int *writerPids, int *readerPids, int  currentWriters, int currentReaders, char* creationMsgQueueId){
	for(int i = 0; i < currentWriters; i++)
		//killProcess(writerPids[i]);
	for(int i = 0; i < currentReaders; i++)
		//killProcess(readerPids[i]);
	closeMessageQueue(creationMsgQueueId);
	closeMessageQueue(prodconsMsgQueueId);
}

int reader(){
	int messagesRead = 0;
	while(strcmp(message, "exit") != 0){
		char *message = (char*)receiveMessage(creationMsgQueue);
		messagesRead++;
	}
	printf("messages read: %d\n", messagesRead);
}

int writer(){
	int messagesSent = 0;
	while(1){
		char * msg = "lel this is fun message lel"
		sendMessage(prodconsMsgQueueId, msg, strlen(msg));
	}
}