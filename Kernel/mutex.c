#include <stdint.h>
#include "naiveConsole.h"
#include "mutex.h"
#include "buddy.h"
#include "lib.h"
#include "process.h"
#include <wait.h>
#include "listADT.h"
#include "wait.h"
#define NUM_OF_TESTS 7

//listADT mutexes;
static listADT mutexes;
static int test_inc = 0;

static int tests_passed = 0;

int idFunctionMutex(void *elem, void *id){
	return strcmp(((tmutex*)elem)->id,(char*)id)==0;
}


int idFunctionProcs(void *elem, void *id){
	return ((tproc*)elem)->pid == ((uint64_t)id);
}

void initMutex(){
  static int initialized;
  if(initialized==0){
    mutexes = createListL(idFunctionMutex,sizeof(tmutex));

    tmutex * mutex_master = mymalloc(sizeof(tmutex));
    mutex_master->id = MUTEX_MASTER_ID;
    mutex_master->status = UNLOCKED;
    mutex_master->ownerpid= NULL_PID;

    listADT waiting_proc =  createListL(idFunctionProcs,sizeof(uint64_t));

    mutex_master->waitingPIDs = waiting_proc;

    addL(mutexes, mutex_master);

    initialized++;
    return;
  }
}

int createMutex(char* mutexId, uint64_t processId){
    lock(MUTEX_MASTER_ID, processId);
    if (containsL(mutexes,mutexId)!= NULL){ //Ya existe el mutex
        unlock(MUTEX_MASTER_ID, processId);
        return -1;
    }


    tmutex* mutex = mymalloc(sizeof(tmutex));
    mutex->id = mutexId;
    mutex->status = UNLOCKED;
    mutex->ownerpid = NULL_PID;

    listADT waitList = createListL(idFunctionProcs,sizeof(tproc));

    mutex->waitingPIDs = waitList;

    addL(mutexes, mutex);
    unlock(MUTEX_MASTER_ID, processId);

    return 1;
}

tproc * newProc(uint64_t pid){
  tproc* newProc = mymalloc(sizeof(tproc));
  newProc->pid = pid;
  return newProc;
}

int lock(char* mutexId, uint64_t processId){
    if (mutexes == NULL || isEmptyL(mutexes)){
        return -1;
    }
    tmutex* mutex = getL(mutexes, mutexId);
    if (mutex == NULL){
        return -1;
    }

    int was_locked= mutex_lock(&mutex->status);

    if(was_locked){ //Si estaba bloqueado ==> agregarlo a la cola de procesos esperando
        addL(mutex->waitingPIDs,newProc(processId));
				wait(processId);

    }else{
        mutex->ownerpid = processId;
    }


    return 1;
}

int unlock(char* mutexId, uint64_t processId){
    if (mutexes == NULL || isEmptyL(mutexes)){
        return -1;
    }
    tmutex* mutex = getL(mutexes, mutexId);
    if (mutex == NULL){
        return -1;
    }
	 // ncScroll();
	 // ncPrint("unlock: ");
	 // ncPrintDec(processId);
	 // ncPrint(" , owner: ");
	 // ncPrintDec(mutex->ownerpid);
	 // ncScroll();

    if( mutex->ownerpid == processId){

        uint64_t new_pid;
        int blockedProcessFound = 0;

  		if (!isEmptyL(mutex->waitingPIDs)){
				new_pid = ((tproc*)getFirstL(mutex->waitingPIDs))->pid;
				removeFirstL(mutex->waitingPIDs);
				blockedProcessFound = 1;
				}

				// ncScroll();
				// ncPrint("unlock: ");
				// ncPrintDec(processId);
				// ncPrint(" blockedFound:");
				// ncPrintDec(blockedProcessFound);
				// ncPrint(" new_pid:");
				// ncPrintDec(new_pid);
				// ncScroll();
        /*
         *  No hace falta desbloquear el mutex si se
         *  lo voy a dar a otro proceso. Con simplemente
         *  cambiar el ownerpid le doy el control del
         *  mutex entonces sólo el lo puede desbloquearlo
         */
         if (blockedProcessFound) {
             mutex-> ownerpid = new_pid;
             signal();
         }else{ //Si no encontré proceso, librero el mutex
             mutex-> status = UNLOCKED;
         }

         return 1;
    }

    return -1;

}
int terminateMutex(char * mutexId, uint64_t processId){
    lock(MUTEX_MASTER_ID, processId); //Para que el borrado sea atómico

    tmutex* mutex = getL(mutexes, mutexId);
    if (mutex == NULL){ //No existe el mutex
        unlock(MUTEX_MASTER_ID, processId);
        return -1;
    }

    removeAndFreeAllL(mutex->waitingPIDs);
    removeAndFreeL(mutexes,mutexId);
    unlock(MUTEX_MASTER_ID, processId);
    return 1;

}

tmutex* containMutex(char* mutexId){
    return getL(mutexes, mutexId);
}













// TESTS

void initMutexTest(){
    if(mutexes == NULL){
        ncPrint("initMutexTest ");
        ncPrintTestFailed("FAILED! --> mutexes = NULL");
    }else if(getFirstL(mutexes) == NULL){
        ncPrint("initMutexTest ");
        ncPrintTestFailed("FAILED! --> getFirstL(mutexes) = NULL");
    }else if(((tmutex*)getFirstL(mutexes))->waitingPIDs == NULL){
        ncPrint("initMutexTest ");
        ncPrintTestFailed("FAILED! --> getFirstL(mutexes)->waitingPIDs = NULL");
    }else{
        ncPrint("initMutexTest ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }
}


void createMutexCreatesAMutexTest(){
    int value = createMutex("HOLA_SOY_UN_TEST", 1);
    if (value == -1){
        ncPrint("ERROR: createMutex Failed");
        return;
    }
    if(containsL(mutexes,"HOLA_SOY_UN_TEST")){
      ncPrint("createMutexCreatesAMutexTest  ");
      ncPrintTestPassed("PASSED!");
			tests_passed++;
    }
    else{
      ncPrint("createMutexCreatesAMutexTest  ");
      ncPrintTestFailed("FAILED!");
    }

}

void increment_test(){
	for(int i=0;i<1000;i++){
		lock("__INC_GLOBAL__",get_current_proc()->pid);
		test_inc++;
		if(test_inc==5000)
			signal();
		unlock("__INC_GLOBAL__",get_current_proc()->pid);
	}
}


void multiProcessUsesMutexTest(){
	createMutex("__INC_GLOBAL__",get_current_proc()->pid);

	for(int i=0;i<5;i++){
		start_proc("test",(void*) increment_test,0,NULL, 0);
	}
	wait(get_current_proc()->pid);
	ncPrint("multiProcessUsesMutexTest: ");
	if(test_inc == 5000){
		ncPrintTestPassed("PASSED!");
	}else{
		ncPrintTestFailed("FAILED! ");
	}
}

void lockofLockedMutexClaimsMutexTest(){
    createMutex("LOCKEO_EL_MUTEX", 2);
    lock("LOCKEO_EL_MUTEX", 2);

    tmutex * mutex = getL(mutexes,"LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
    }
    if (mutex->ownerpid == 2){
        ncPrint("lockofLockedMutexClaimsMutexTest ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }else{
        ncPrint("lockofLockedMutexClaimsMutexTest ");
        ncPrintTestFailed("FAILED!");
    }

    return;

}


void lockOfLockedMutexAddsToWaitingListTest(){

    lock("LOCKEO_EL_MUTEX", 3);

    tmutex * mutex = getL(mutexes,"LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
        return;
    }
    if (((tproc*)getFirstL(mutex->waitingPIDs))->pid == 3){
        ncPrint("lockOfLockedMutexAddsToWaitingListTest: ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }else{
        ncPrint("lockOfLockedMutexAddsToWaitingListTest: ");
        ncPrintTestFailed("FAILED!");
    }

}

void unlockOfLockedMutexChangesOwnerTest(){
    unlock("LOCKEO_EL_MUTEX", 2);
    tmutex * mutex = getL(mutexes, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
        return;
    }

    if (mutex->ownerpid == 3){
        ncPrint("unlockOfLockedMutexChangesOwnerTest: ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }else{
        ncPrint("unlockOfLockedMutexChangesOwnerTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void unlockWithoutWaitingChangesStatusToUnlockTest(){
    unlock("LOCKEO_EL_MUTEX", 3);

    tmutex * mutex = getL(mutexes, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
        return;
    }

    if( mutex->status == UNLOCKED){
        ncPrint("unlockWithoutWaitingChangesStatusToUnlockTest: ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }else{
        ncPrint("unlockWithoutWaitingChangesStatusToUnlockTest: ");
        ncPrintTestFailed("FAILED!");
    }
}


void terminateMutexEliminatesTheMutexTest(){
    terminateMutex("LOCKEO_EL_MUTEX", 4);

    tmutex * mutex = getL(mutexes,"LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("terminateMutexEliminatesTheMutexTest: ");
        ncPrintTestPassed("PASSED!");
				tests_passed++;
    }else{
        ncPrint("terminateMutexEliminatesTheMutexTest: ");
        ncPrintTestFailed("FAILED!");
    }
}

void numOfTestsPassed(){
	if (tests_passed == NUM_OF_TESTS){
		ncPrintTestPassed("All Tests PASSED");
	}else{
		ncPrintDec(NUM_OF_TESTS - tests_passed);
		ncPrintTestFailed("Tests FAILED");
	}
}
