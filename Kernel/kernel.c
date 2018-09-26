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
	loadIDT();

	return getStackBase();
}

void idle1() {
    ncPrint("Hola soy el uno\n");
    halt();
}

void idle2() {
    ncPrint("Hola soy el dos\n");
    halt();
}


//Test suites START
void roundRobinTestSuite(){
	testAddElementToHeader();
	ncScroll();
	testAddMultipleElementsToHeader();
	ncScroll();
	testAddALotOfElementsToQueue();
	ncScroll();
	testRoundRobin();
	ncScroll();
	testNotFinishedProcessGoesToTail();

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
	initializeScreen();
	ncResetPosition();

	int testing = 1;

	if(testing){

		//roundRobinTestSuite();
		ncPrintTest("Mutex Test Suite: ------------");
		mutexTestSuite();
		ncPrintTest("Message Queue Test Suite: ------------");
		messageQueueTestSuite();

	}else{
 	    start_proc("shell", sampleCodeModuleAddress);
	}


	halt();
	return 0;
}
