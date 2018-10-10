#ifndef PRODCONS_H
#define PRODCONS_H
#define MAX_MESSAGES 200
#define MAX_READERS 50
#define MAX_WRITERS 50
#define STARTING_READERS 5
#define STARTING_WRITERS 5
#define PRODCONS_MSG_QUEUE_ID "prodConsMsgQueue"
#define CREATION_MSG_QUEUE_ID "creationMsgQueue"
#define COMM_MSG_QUEUE_ID "commMsgQueue"
#define SHELL_PRODCONS_MUTEX "shellProdconsMutex"

void prodcons();

#endif