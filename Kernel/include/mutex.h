#ifndef MUTEX_H
#define MUTEX_H


#define MUTEX_MASTER_ID "__mutex_master_id__"
#define UNLOCKED	0
#define LOCKED		1
#define NULL_PID   -1
#include "listADT.h"


typedef struct proc{
  uint64_t pid;
}tproc;

typedef struct mutex{
  listADT waitingPIDs;
  char * id;
  uint32_t status;
  uint64_t ownerpid;
}tmutex;


//Implementado en asm/mutex.asm
/*
 *  Setea el estado a locked.
 *  En caso de que hubiese estado lockeado
 *  devuelve 1, devuelve 0 si no estaba lockeado
 */
uint32_t mutex_lock(uint32_t *status);

void initMutex();
int createMutex(char* mutexId, uint64_t processId);
// void addMutex(tmutex_node* node, tmutex* mutex);

int lock(char* mutexId, uint64_t processId);
int unlock(char* mutexId, uint64_t processId);

int terminateMutex(char* mutexId, uint64_t processId);
tmutex* containMutex(char* mutexId);



// TESTS
void initMutexTest();
void createMutexCreatesAMutexTest();
void lockofLockedMutexClaimsMutexTest();
void lockOfLockedMutexAddsToWaitingListTest();
void unlockOfLockedMutexChangesOwnerTest();
void unlockWithoutWaitingChangesStatusToUnlockTest();
void terminateMutexEliminatesTheMutexTest();


#endif
