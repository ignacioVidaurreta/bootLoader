#include <stdint.h>
#include "messageQueue.h"
#include "buddy.h"
#include "process.h"
#include "naiveConsole.h"
#include "mutex.h"
#include "lib.h"

tmailbox_list * mailboxes;

void initMessageQueue(){
  mailboxes = mymalloc(sizeof(tmailbox_list));
  mailboxes->head = NULL;
  createMutex(MUTEX_NAME, 0);
}

int createMailBox(char * mailboxId){
  lock(MUTEX_NAME, get_current_proc()->pid);

  if(!containsMailbox(mailboxId))
    addMailbox(newMailbox(mailboxId));

  unlock(MUTEX_NAME, get_current_proc()->pid);
  return 0;
}

void send(const char *mailboxId, const void *message, const unsigned int messageSize) {
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	tmailbox * mailbox = getMailbox(mailboxId);
	addMessage(mailbox->messageQueue,message,messageSize);
	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
}

void * receive(const char *mailboxId) {
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	tmailbox * mailbox = getMailbox(mailboxId);
	void * message = getMessage(mailbox->messageQueue);
    removeFirst(mailbox->messageQueue);
	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	return message;
}

void closeMailbox(const char *mailboxId) {
	lock(MUTEX_NAME,get_current_proc()->pid);
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);

	tmailbox * mailbox = getMailbox(mailboxId);
	removeAndFreeAllMessages(mailbox->messageQueue);
	removeAndFreeMailbox(mailboxId);

	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	unlock(MUTEX_NAME,get_current_proc()->pid);
}



int containsMailbox(const char * mailboxId) {
	if(mailboxes == NULL)
    return FALSE;

	tmailbox_node * aux;
	if(mailboxes->head == NULL){
		return FALSE;
	}else {
		aux = mailboxes->head;
		while(aux != NULL){
			if(strcmp(mailboxId,aux->mailbox->mailboxId)==0){
                return TRUE;
            }
			aux = aux->next;
		}
	}
	return FALSE;
}

tmailbox * getMailbox(const char *mailboxId) {

	if(mailboxes == NULL) return NULL;

	tmailbox_node * aux = mailboxes->head;
	while (aux != NULL) {
		if(strcmp(mailboxId,aux->mailbox->mailboxId)==0) {
			return aux->mailbox;
		}
        aux = aux->next;
	}
	return NULL;
}

void * getMessage(tmessageQueue_list * messageQueue) {
	if(messageQueue == NULL) return NULL;
	if(messageQueue->head == NULL) return NULL;
	return messageQueue->head->message;
}

int removeFirst(tmessageQueue_list * messageQueue) {
	if(messageQueue == NULL) return NULL_LIST_ERROR;
	if(messageQueue->head == NULL) return EMPTY_LIST_ERROR;

	tmessageQueue_node * aux;
	aux = messageQueue->head;
	messageQueue->head = messageQueue->head->next;
  myfree(aux,sizeof(tmessageQueue_node));
	return REMOTION_OK;
}

int removeAndFreeAllMessages(tmessageQueue_list * messageQueue) {
	if(messageQueue == NULL) return NULL_LIST_ERROR;
	while (removeAndFreeFirstMessage(messageQueue) != EMPTY_LIST_ERROR);
	return REMOTION_OK;
}

int removeAndFreeFirstMessage(tmessageQueue_list * messageQueue) {
	if(messageQueue == NULL) return NULL_LIST_ERROR;
	if(messageQueue->head == NULL) return EMPTY_LIST_ERROR;

	tmessageQueue_node * aux;
	aux = messageQueue->head;
	messageQueue->head = messageQueue->head->next;
	myfree(aux->message,sizeof(aux->size));
	myfree(aux,sizeof(tmessageQueue_node));
	return REMOTION_OK;
}

int removeAndFreeMailbox(const char *mailboxId) {

	tmailbox_node * aux;
	tmailbox_node * auxPrev;
	tmailbox_node * aux2;
	int firstLoop = 1;
	if(mailboxes == NULL) return NULL_LIST_ERROR;
	if(mailboxes->head == NULL) {
		return ELEMENT_DOESNT_EXIST;
	}else {
		aux = mailboxes->head;
		auxPrev=NULL;

		while (aux != NULL) {
			if(strcmp(mailboxId,aux->mailbox->mailboxId)==0) {

				aux2 = aux->next;
				myfree(aux->mailbox,sizeof(tmailbox));
				myfree(aux,sizeof(tmailbox_node));
				if(firstLoop == 1){
					mailboxes->head = aux2;
				}else {
					auxPrev->next = aux2;
				}
				return REMOTION_OK;
			}
			auxPrev = aux;
			aux = aux->next;
			firstLoop = 0;
		}
		return ELEMENT_DOESNT_EXIST;
	}
}

int addMessage(tmessageQueue_list * messageQueue, const void *message, const unsigned int size) {

	if(messageQueue == NULL) return NULL_LIST_ERROR;
    if(message == NULL) return NULL_ELEMENT_ERROR;
    if(size == 0)  return SIZE_ERROR;

	tmessageQueue_node * newNode = mymalloc(sizeof(tmessageQueue_node));
    newNode->message = mymalloc(size);
    newNode->size = size;
    newNode->next = NULL;
    memcpy(newNode->message,message,size);

	tmessageQueue_node *  aux;
	if(messageQueue->head == NULL) {
        messageQueue->head = newNode;
    }
    else {
		aux = messageQueue->head;
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = newNode;
    }
    return INSERTION_OK;
}

int addMailbox(tmailbox * mailbox){
  if(mailboxes == NULL) return NULL_LIST_ERROR;
    if(mailbox == NULL) return NULL_ELEMENT_ERROR;

	tmailbox_node * newNode = mymalloc(sizeof(tmailbox_node));
    newNode->mailbox = mymalloc(sizeof(tmailbox));
    newNode->next = NULL;
    memcpy(newNode->mailbox,mailbox,sizeof(tmailbox));

	tmailbox_node * aux;
	if(mailboxes->head == NULL) {
        mailboxes->head = newNode;
    }
    else {
		aux = mailboxes->head;
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = newNode;
    }
    return INSERTION_OK;
}

tmailbox * newMailbox(const char *mailboxId) {
	tmailbox * newMailbox = mymalloc(sizeof(tmailbox));
	newMailbox->mailboxId = mymalloc(strlen(mailboxId) + 1);
	strcpy(newMailbox->mailboxId,mailboxId);
	newMailbox->messageQueue = mymalloc(sizeof(tmessageQueue_list));
	createMutex(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	return newMailbox;
}



//TESTS


void initMessageQueueCreatesMutexTest(){

    tmutex* mut = containMutex(MUTEX_NAME);

    if (mut != NULL){
        ncPrint("initMessageQueueCreatesMutexTest: PASSED!");
    }else{
        ncPrint("initMessageQueueCreatesMutexTest: FAILED!");
    }
}


void createMailBoxCreatesMailBoxTest(){
    createMailBox("__MAILBOXTEST__");
    if(strcmp(mailboxes->head->mailbox->mailboxId, "__MAILBOXTEST__") == 0){
        ncPrint("createMailBoxCreatesMailBoxTest: PASSED");
    }else{
        ncPrint("createMailBoxCreatesMailBoxTest: FAILED");
    }
}

void getMailboxFindsExistingMailboxTest(){
    tmailbox* mb = getMailbox("__MAILBOXTEST__");
    if (mb != NULL && strcmp(mb->mailboxId, "__MAILBOXTEST__") == 0){
        ncPrint("getMailboxFindsExistingMailboxTest: PASSED!");
    }else{
        ncPrint("getMailboxFindsExistingMailboxTest: FAILED!");
    }
}

void sendSendsMessageTest(){
    send("__MAILBOXTEST__", "a_test", strlen("a_test"));
    tmailbox* mb = getMailbox("__MAILBOXTEST__");

    if (strcmp(mb->messageQueue->head->message, "a_test") == 0){
        ncPrint("sendSendsMessageTest: PASSED!");
    }else{
        ncPrint("sendSendsMessageTest: FAILED!");
    }

}

void receiveReceivesMessageTest(){
    char * msg = receive("__MAILBOXTEST__");

    if (strcmp(msg, "a_test") == 0){
        ncPrint("receiveReceivesMessageTest: PASSED!");
    }else{
        ncPrint("receiveReceivesMessageTest: FAILED!");
    }
}

void closeMailboxClosesMailboxTest(){
    closeMailbox("__MAILBOXTEST__");

    if (getMailbox("__MAILBOXTEST__") == NULL){
        ncPrint("closeMailboxClosesMailboxTest: PASSED!");
    }else{
        ncPrint("closeMailboxClosesMailboxTest: FAILED!");
    }
}

void containsMailboxTest(){
    int failed = 1;
    if (containsMailbox("__MAILBOXTEST__") == FALSE){
        createMailBox("__MAILBOXTEST__");
        if (containsMailbox("__MAILBOXTEST__") == TRUE){
            failed = 0;
            ncPrint("containsMailboxTest: PASSED!");
        }
    }

    if (failed){
        ncPrint("containsMailboxTest: FAILED!");
    }
}
