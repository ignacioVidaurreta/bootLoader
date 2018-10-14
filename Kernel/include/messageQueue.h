#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include "listADT.h"
#define MUTEX_NAME "__MessegeQueueMutex__"

#define TRUE 1
#define FALSE 0
#define INSERTION_OK 0
#define NULL_ELEMENT_ERROR 1
#define NULL_LIST_ERROR -3
#define EMPTY_LIST_ERROR -2
#define REMOTION_OK 0
#define ELEMENT_DOESNT_EXIST -1
#define SIZE_ERROR 2

typedef struct mailbox{
  char * mailboxId;
  listADT messageQueue;
}tmailbox;

typedef struct message{
  void * message;
  int size;
}tmessage;

// typedef struct messageQueue_node{
//   void * message;
//   int size;
//   struct messageQueue_node * next;
// }tmessageQueue_node;
//
// typedef struct messageQueue_list{
//   tmessageQueue_node * head;
// }tmessageQueue_list;
//
// typedef struct mailbox{
//   char * mailboxId;
//   tmessageQueue_list * messageQueue;
// }tmailbox;
//
// typedef struct mailbox_node{
//   struct mailbox_node * next;
//   tmailbox * mailbox;
// }tmailbox_node;
//
// typedef struct mailbox_list{
//   tmailbox_node * head;
//
// }tmailbox_list;

void initMessageQueue();
int createMailBox(char * mailboxId);
void send(char *mailboxId, const void *message, const unsigned int messageSize);
void * receive(char *mailboxId);
void closeMailbox(char *mailboxId);
tmailbox * getMailbox(char *mailboxId);
void *getMessage(listADT messageQueue);
tmailbox * newMailbox(char *mailboxId);
tmessage * newMessage(const void *message, const unsigned int messageSize);

//TESTS
void initMessageQueueCreatesMutexTest();
void createMailBoxCreatesMailBoxTest();
void getMailboxFindsExistingMailboxTest();
void sendSendsMessageTest();
void receiveReceivesMessageTest();
void closeMailboxClosesMailboxTest();
void containsMailboxTest();

#endif
