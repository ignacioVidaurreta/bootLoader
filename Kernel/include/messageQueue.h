#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H


typedef struct mailbox{
  char * mailboxId;
  void * messageQueue;
}tmailbox;

typedef struct mailbox_node{
  struct mailbox_node * next;
  tmailbox * mailbox;
}tmailbox_node;

typedef struct mailbox_list{
  tmailbox_node * head;

}tmailbox_list;

void initMessaageQueue();



#endif
