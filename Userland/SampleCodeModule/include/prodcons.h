#ifndef PRODCONS_H
#define PRODCONS_H
#define MAX_MESSAGES 20
#define MAX_READERS 50
#define MAX_WRITERS 50
#define CONSUMERS 5
#define PRODUCERS 5
#define PRODCONS_MSG_QUEUE_ID "prodConsMsgQueue"
#define CREATION_MSG_QUEUE_ID "creationMsgQueue"
#define COMM_MSG_QUEUE_ID "commMsgQueue"
#define SHELL_PRODCONS_MUTEX "shellProdconsMutex"
#define PRODCONS_MUTEX "prodconsMutex"

void prodcons();
void consumer();
void producer();

#endif
