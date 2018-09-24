#include <stdint.h>
#include <stdlib.h>
#include "naiveConsole.h"
#include "mutex.h"
#include "buddy.h"
#include "process.h"


static tmutex_list * mutexes;
static int initialized;
void initMutex(){

  if(initialized==0){
    mutexes = mymalloc(sizeof(tmutex_list));
    mutexes->head = NULL;

    tmutex * mutex_master = mymalloc(sizeof(tmutex));
    mutex_master->id = MUTEX_MASTER_ID;
    mutex_master->status = LOCKED;
    mutex_master->ownerpid= NULL_PID;

    tproc_list * waiting_proc =  mymalloc(sizeof(tproc_list));
    waiting_proc->head = NULL;

    mutex_master->waiting_processes = waiting_proc;

    tmutex_node * node_master = mymalloc(sizeof(tmutex_node));
    node_master->mutex= mutex_master;
    node_master->next= NULL;
    mutexes->head = node_master;

    initialized++;
    return;
  }
}















// TESTS

void initMutexTest(){
  initMutex();
  if(mutexes == NULL)
    ncPrint("initMutexTest FAILED! : mutexes = NULL");
  else if(mutexes->head == NULL)
    ncPrint("initMutexTest FAILED! : mutexes->head = NULL");
  else if(mutexes->head->mutex == NULL)
    ncPrint("initMutexTest FAILED! : mutexes->head->mutex = NULL");
  else if(mutexes->head->mutex->waiting_processes == NULL)
    ncPrint("initMutexTest FAILED! : mutexes->head->mutex->waiting_processes = NULL");
  else
    ncPrint("initMutexTest PASSED!");
}
/*
void initMutipleTimesTest(){
  if ( initialized != 0){
    ncPrint("initMutipleTimesTest FAILED: Initialized = 0");
  }
  else{
    initMutex();
    if (initialized >1){
      ncPrint("initMutipleTimesTest FAILED: Initialized >1");
    }else{
      ncPrint("initMutipleTimesTest PASSED!");
    }
  }
}
*/
