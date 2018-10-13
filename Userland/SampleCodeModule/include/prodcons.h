#ifndef PRODCONS_H
#define PRODCONS_H
#define MAX_MESSAGES 20
#define MAX_READERS 50
#define MAX_WRITERS 50
#define STARTING_READERS 1
#define STARTING_WRITERS 1
#define PRODCONS_MSG_QUEUE_ID "prodConsMsgQueue"
#define CREATION_MSG_QUEUE_ID "creationMsgQueue"
#define COMM_MSG_QUEUE_ID "commMsgQueue"
#define SHELL_PRODCONS_MUTEX "shellProdconsMutex"
#define PRODCONS_MUTEX "prodconsMutex"

void prodcons();

#endif