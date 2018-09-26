#include "include/naiveConsole.h"
#include "include/messageQueue.h"
#include "include/mutex.h"
#include "include/RoundRobin.h"
#include "include/tests.h"


void runTests(){
    ncPrintTest("Round Robin Test Suite: ------------");
    roundRobinTestSuite();
    ncPrintTest("Mutex Test Suite: ------------");
    mutexTestSuite();
    ncPrintTest("Message Queue Test Suite: ------------");
    messageQueueTestSuite();
}


void roundRobinTestSuite(){
	ncScroll();
	testAddElementToHeader();
	ncScroll();
	testAddMultipleElementsToHeader();
	ncScroll();
	testAddALotOfElementsToQueue();
	ncScroll();
	testRoundRobin();
	ncScroll();
	testNotFinishedProcessGoesToTail();
	ncScroll();

}
void mutexTestSuite(){
	ncScroll();
	initMutexTest();
	ncScroll();
	createMutexCreatesAMutexTest();
	ncScroll();
	lockofLockedMutexClaimsMutexTest();
	ncScroll();
	lockOfLockedMutexAddsToWaitingListTest();
	ncScroll();
	unlockOfLockedMutexChangesOwnerTest();
	ncScroll();
	unlockWithoutWaitingChangesStatusToUnlockTest();
	ncScroll();
	terminateMutexEliminatesTheMutexTest();
	ncScroll();
}

void messageQueueTestSuite(){
	ncScroll();
	initMessageQueueCreatesMutexTest();
	ncScroll();
	createMailBoxCreatesMailBoxTest();
	ncScroll();
	getMailboxFindsExistingMailboxTest();
	ncScroll();
	sendSendsMessageTest();
	ncScroll();
	receiveReceivesMessageTest();
	ncScroll();
	closeMailboxClosesMailboxTest();
	ncScroll();
	containsMailboxTest();
	ncScroll();
}
