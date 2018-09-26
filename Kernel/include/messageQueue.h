#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#define TRUE 1
#define FALSE 0
#define INSERTION_OK 0
#define NULL_ELEMENT_ERROR 1
#define NULL_LIST_ERROR -3
#define EMPTY_LIST_ERROR -2
#define REMOTION_OK 0
#define ELEMENT_DOESNT_EXIST -1
#define SIZE_ERROR 2



typedef struct messageQueue_node{
  void * message;
  int size;
  struct messageQueue_node * next;
}tmessageQueue_node;

typedef struct messageQueue_list{
  tmessageQueue_node * head;
}tmessageQueue_list;

typedef struct mailbox{
  char * mailboxId;
  tmessageQueue_list * messageQueue;
}tmailbox;

typedef struct mailbox_node{
  struct mailbox_node * next;
  tmailbox * mailbox;
}tmailbox_node;

typedef struct mailbox_list{
  tmailbox_node * head;

}tmailbox_list;

void initMessaageQueue();
int createMailBox(char * mailboxId);
void send(const char *mailboxId, const void *message, const unsigned int messageSize);
void * receive(const char *mailboxId);
void closeMailbox(const char *mailboxId);
int containsMailbox(const char * mailboxId);
tmailbox * getMailbox(const char *mailboxId);
void *getMessage(tmessageQueue_list * messageQueue);
int removeFirst(tmessageQueue_list * messageQueue);
int removeAndFreeAllMessages(tmessageQueue_list * messageQueue);
int removeAndFreeFirstMessage(tmessageQueue_list * messageQueue);
int removeAndFreeMailbox(const char *mailboxId);
int addMessage(tmessageQueue_list * messageQueue, const void *message, const unsigned int size);
int addMailbox(tmailbox * mailbox);
static tmailbox * newMailbox(const char *mailboxId);
int strlen(const char * str);


#endif
