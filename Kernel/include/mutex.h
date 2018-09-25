#ifndef MUTEX_H
#define MUTEX_H


#define MUTEX_MASTER_ID "__mutex_master_id__"
#define UNLOCKED	0
#define LOCKED		1
#define NULL_PID   -1

typedef struct proc_node{
  struct proc_node * next;
  uint64_t pid;
}tproc_node;

typedef struct proc_list{
  tproc_node * head;
}tproc_list;

typedef struct mutex{
  tproc_list * waitingPIDs;
  char * id;
  uint32_t status;
  uint64_t ownerpid;
}tmutex;

typedef struct mutex_node{
  tmutex * mutex;
  struct mutex_node * next;
}tmutex_node;

typedef struct motex_list{
  tmutex_node * head;
}tmutex_list;

//Implementado en asm/mutex.asm
/*
 *  Setea el estado a locked.
 *  En caso de que hubiese estado lockeado
 *  devuelve 1, devuelve 0 si no estaba lockeado
 */
uint32_t mutex_lock(uint32_t *status);

void initMutex();
int createMutex(char* mutexId, uint64_t processId);
void addMutex(tmutex_node* node, tmutex* mutex);

int lock(char* mutexId, uint64_t processId);
int unlock(char* mutexId, uint64_t processId);
int lockIfUnlocked(char* mutexId, uint64_t processId);

int terminateMutex(char* mutexId, uint64_t processId);
tmutex* containMutex(tmutex_node* node, char* mutexId);
void addWaitingProc(int pid, tproc_node* node);
void removeMutex(char* mutexId, tmutex* mutex);
void removeAndFreeWaitingProc(tproc_node* node);
tmutex_node * removeAndFreeFromMutexes(char* mutexId, tmutex_node* node);


// TESTS
void initMutexTest();
void createMutexCreatesAMutexTest();
void lockofLockedMutexClaimsMutexTest();
void lockOfLockedMutexAddsToWaitingListTest();
void unlockOfLockedMutexChangesOwnerTest();
void unlockWithoutWaitingChangesStatusToUnlockTest();
void terminateMutexEliminatesTheMutexTest();


#endif
