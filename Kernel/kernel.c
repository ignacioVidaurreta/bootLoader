#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <math.h>
#include <time.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <RoundRobin.h>
#include "process.h"
#include "mutex.h"
#include "include/messageQueue.h"
#include "buddy.h"
#include <philosophers.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

    init_process();
    initMutex();
    initMessageQueue();
	loadIDT();

	return getStackBase();
}

uint64_t idle1() {
    ncPrint("Hola soy el uno\n");
    return 0;
}


void idle2() {
    ncPrint("Hola soy el dos\n");
}


void roundRobinTestSuite(){
	ncScroll();
	testAddElementToHeader();
	ncScroll();
	testAddMultipleElementsToHeader();
	ncScroll();
	testAddALotOfElementsToQueue();
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



int main(){

	//initializeScreen();
	initMutex();
	ncResetPosition();
	int test=0;
	if (test){

		ncPrintTestHeader("------------ Round Robin Test Suite: ------------");
	    roundRobinTestSuite();
	    ncPrintTestHeader("------------ Mutex Test Suite: ------------");
	    mutexTestSuite();
	    ncPrintTestHeader("------------ Message Queue Test Suite: ------------");
	    messageQueueTestSuite();

		//testPhilosophers();
	}else{
		start_proc("shell", sampleCodeModuleAddress, 0, NULL);
	}
	halt();
	return 0;
}
