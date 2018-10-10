#include <stdlib.h>
#include <prodcons.h>
#include <date.h>

void changeInProcesses(char *processes, int number, int *prevNumber, int *pids);
void cleanup(int *writerPids, int *readerPids, int  currentWriters, int currentReaders);
void reader();
void writer();

void prodcons(int startingReaders, int startingWriters){
	int currentReaders = 0;
	int currentWriters = 0;
	int readerPids[MAX_READERS] = {0};
	int writerPids[MAX_WRITERS] = {0};
	createMessageQueue(PRODCONS_MSG_QUEUE_ID);
	createMessageQueue(CREATION_MSG_QUEUE_ID);
	createMessageQueue(COMM_MSG_QUEUE_ID);
	changeInProcesses("reader", startingReaders, &currentReaders, readerPids);
	changeInProcesses("writer", startingWriters, &currentWriters, writerPids);
	char *message = "continue";
	while(strcmp(message,"exit") != 0){
		message = (char*)receiveMessage(CREATION_MSG_QUEUE_ID);
		if(strcmp(message, "reader") == 0)
			changeInProcesses(message, *((int*)receiveMessage(CREATION_MSG_QUEUE_ID)), &currentReaders, readerPids);
		else if(strcmp(message, "writer") == 0)
			changeInProcesses(message, *((int*)receiveMessage(CREATION_MSG_QUEUE_ID)), &currentWriters, writerPids);
		else if(strcmp(message, "more messages") == 0)
			sendMessage(COMM_MSG_QUEUE_ID, "keep on keeping on", strlen("keep on keeping on"));
	}
	//cleanup(writerPids, readerPids, currentWriters, currentReaders);
}

void changeInProcesses(char* processes, int number, int *prevNumber, int *pids){
	if(strcmp(processes, "reader") == 0){
		if(number > 0 && number + *prevNumber < MAX_READERS)
			for(int i = 0; i < number; i++)
				pids[i + *prevNumber] = startProcUser("reader", (void*)reader);
		else if(number < 0 && *prevNumber + number > 0)
			for(int i = 0; i > number; i--)
				kill(pids[*prevNumber - i]);
	}
	else if(strcmp(processes, "writer") == 0){
		if(number > 0 && number + *prevNumber < MAX_WRITERS)
			for(int i = 0; i < number; i++)
				pids[i + *prevNumber] = startProcUser("writer", (void*)writer);
		else if(number < 0 && number + *prevNumber > 0)
			for(int i = 0; i > number; i--)
				kill(pids[*prevNumber + i]);
	}
	*prevNumber += number;
}

void cleanup(int *writerPids, int *readerPids, int  currentWriters, int currentReaders){
	for(int i = 0; i < currentWriters; i++)
		kill(writerPids[i]);
	for(int i = 0; i < currentReaders; i++)
		sendMessage(PRODCONS_MSG_QUEUE_ID, "exit", strlen("exit"));
	closeMessageQueue(CREATION_MSG_QUEUE_ID);
	closeMessageQueue(PRODCONS_MSG_QUEUE_ID);
}

void reader(){
	int messagesRead = 0;
	while(1){
		char *message = "continue";
		while(strcmp(message, "exit") != 0){
			message = (char*)receiveMessage(CREATION_MSG_QUEUE_ID);
			messagesRead++;
		}
	}
	printf("messages read: %d\n", messagesRead);
}

void writer(){
	int messagesSent = 0;
	while(1){
		if(messagesSent > MAX_MESSAGES){
			char *waitingMessage = (char*) receiveMessage(COMM_MSG_QUEUE_ID);
			if (strcmp(waitingMessage,"keep on keeping on") == 0)
				messagesSent = 0;
		}
		else{
			sendMessage(PRODCONS_MSG_QUEUE_ID, "lel this is fun message lel", strlen("lel this is fun message lel"));
			messagesSent++;
		}
	}
}
