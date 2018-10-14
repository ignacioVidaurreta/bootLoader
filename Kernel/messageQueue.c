#include <stdint.h>
#include "messageQueue.h"
#include "buddy.h"
#include "process.h"
#include "naiveConsole.h"
#include "mutex.h"
#include "lib.h"
#include "listADT.h"

listADT mailboxes;

int idFunctionMailbox(void *elem, void *id){
	return strcmp(((tmailbox*)elem)->mailboxId,(char*)id)==0;
}

int idFunctionMessageQueue(void *elem, void *id){
	return strcmp(((tmessage*)elem)->message,(char*)id)==0;
}

void initMessageQueue(){
  mailboxes = createListL(idFunctionMailbox, sizeof(tmailbox));
  createMutex(MUTEX_NAME, 0);
}

int createMailBox(char * mailboxId){
  lock(MUTEX_NAME, get_current_proc()->pid);
  if(!containsL(mailboxes, mailboxId)){
//      ncPrint(" -- No prior mailbox with this id exists --");
      addL(mailboxes,newMailbox(mailboxId));
//      ncPrint(" -- Mailbox was successfully added --");
    }
  unlock(MUTEX_NAME, get_current_proc()->pid);
  return 0;
}

void send(char *mailboxId, const void *message, const unsigned int messageSize) {
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	tmailbox * mailbox = getL(mailboxes, mailboxId);
	addL(mailbox->messageQueue,newMessage(message,messageSize));
	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
}

void * receive(char *mailboxId) {
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	tmailbox * mailbox = getL(mailboxes, mailboxId);
	tmessage * message = getFirstL(mailbox->messageQueue);
    removeFirstL(mailbox->messageQueue);
	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	return message->message;
}

void closeMailbox(char *mailboxId) {
	lock(MUTEX_NAME,get_current_proc()->pid);
	lock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);

	tmailbox * mailbox = getL(mailboxes, mailboxId);
	removeAndFreeAllL(mailbox->messageQueue);
	removeAndFreeL(mailboxes, mailboxId);

	unlock(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
	unlock(MUTEX_NAME,get_current_proc()->pid);
}

tmailbox * newMailbox(char *mailboxId) {
  tmailbox * newMailbox = mymalloc(sizeof(tmailbox));
  newMailbox->mailboxId = mymalloc(strlen(mailboxId) + 1);
  strcpy(newMailbox->mailboxId,mailboxId);
  newMailbox->messageQueue = createListL(idFunctionMessageQueue,sizeof(tmessage));
  createMutex(concat(MUTEX_NAME,mailboxId),get_current_proc()->pid);
  return newMailbox;
}

tmessage * newMessage(const void *message, const unsigned int messageSize){
  tmessage * newMessage = mymalloc(sizeof(tmessage));
  newMessage->message = mymalloc(messageSize);
  memcpy(newMessage->message,message,messageSize);
  return newMessage;
}


tmailbox * getMailbox(char *mailboxId) {

	return getL(mailboxes, mailboxId);

}

void * getMessage(listADT messageQueue) {
	return getFirstL(messageQueue);
}




//TESTS


void initMessageQueueCreatesMutexTest(){

    tmutex* mut = containMutex(MUTEX_NAME);

    if (mut != NULL){
        ncPrint("initMessageQueueCreatesMutexTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("initMessageQueueCreatesMutexTest: ");
        ncPrintTestFailed("FAILED!");
    }
}


void createMailBoxCreatesMailBoxTest(){
    createMailBox("__MAILBOXTEST__");
    if(strcmp(((tmailbox*)getFirstL(mailboxes))->mailboxId, "__MAILBOXTEST__") == 0){
        ncPrint("createMailBoxCreatesMailBoxTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("createMailBoxCreatesMailBoxTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void getMailboxFindsExistingMailboxTest(){
    tmailbox* mb = getL(mailboxes,"__MAILBOXTEST__");
    if (mb != NULL && strcmp(mb->mailboxId, "__MAILBOXTEST__") == 0){
        ncPrint("getMailboxFindsExistingMailboxTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("getMailboxFindsExistingMailboxTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void sendSendsMessageTest(){
    send("__MAILBOXTEST__", "a_test", strlen("a_test"));
    tmailbox* mb = getL(mailboxes, "__MAILBOXTEST__");
    if (strcmp(((tmessage*)getFirstL(mb->messageQueue))->message, "a_test") == 0){
        ncPrint("sendSendsMessageTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("sendSendsMessageTest: ");
        ncPrintTestFailed("FAILED!");
    }

}

void receiveReceivesMessageTest(){
    char * msg = receive("__MAILBOXTEST__");
    if (strcmp(msg, "a_test") == 0){
        ncPrint("receiveReceivesMessageTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("receiveReceivesMessageTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void closeMailboxClosesMailboxTest(){
    closeMailbox("__MAILBOXTEST__");

    if (getMailbox("__MAILBOXTEST__") == NULL){
        ncPrint("closeMailboxClosesMailboxTest: ");
        ncPrintTestPassed("PASSED!");
    }else{
        ncPrint("closeMailboxClosesMailboxTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void containsMailboxTest(){
    int failed = 1;

    if (containsL(mailboxes,"__MAILBOXTEST__") == FALSE){
        createMailBox("__MAILBOXTEST__");
        if (containsL(mailboxes,"__MAILBOXTEST__") == TRUE){
            failed = 0;
            ncPrint("containsMailboxTest: ");
            ncPrintTestPassed("PASSED!");
        }
    }

    if (failed){
        ncPrint("containsMailboxTest: ");
        ncPrintTestFailed("FAILED!");
    }
}
