#include <stdint.h>
#include <stdlib.h>
#include "naiveConsole.h"
#include "mutex.h"
#include "buddy.h"
#include "lib.h"
#include "process.h"


static tmutex_list * mutexes;

void initMutex(){
  static int initialized;
  if(initialized==0){
    mutexes = mymalloc(sizeof(tmutex_list));

    tmutex * mutex_master = mymalloc(sizeof(tmutex));
    mutex_master->id = MUTEX_MASTER_ID;
    mutex_master->status = LOCKED;
    mutex_master->ownerpid= NULL_PID;

    tproc_list * waiting_proc =  mymalloc(sizeof(tproc_list));
    waiting_proc->head = NULL;

    mutex_master->waitingPIDs = waiting_proc;

    tmutex_node * node_master = mymalloc(sizeof(tmutex_node));
    node_master->mutex= mutex_master;
    node_master->next= NULL;
    mutexes->head = node_master;

    initialized++;
    return;
  }
}

int createMutex(char* mutexId, uint64_t processId){
    lock(MUTEX_MASTER_ID, processId);
    if (containMutex(mutexes->head, mutexId)!= NULL){ //Ya existe el mutex
        unlock(MUTEX_MASTER_ID, processId);
        return -1;
    }



    tmutex* mutex = mymalloc(sizeof(tmutex));
    mutex->id = mutexId;
    mutex->status = UNLOCKED;
    mutex->ownerpid = NULL_PID;

    tproc_list* waitList = mymalloc(sizeof(tproc_list));
    waitList->head = NULL;
    mutex->waitingPIDs = waitList;

    addMutex(mutexes->head ,mutex);
    unlock(MUTEX_MASTER_ID, processId);

    return 1;
}

void addMutex(tmutex_node* node,tmutex* mutex){
    if (node->next == NULL){
        tmutex_node * new_node = mymalloc(sizeof(tmutex_node));
        new_node->mutex = mutex;
        new_node-> next = NULL;
        node->next = new_node;
        return;
    }
    addMutex(node->next, mutex);
}

int lock(char* mutexId, uint64_t processId){
    if (mutexes == NULL || mutexes->head == NULL){
        return -1;
    }
    tmutex* mutex = containMutex(mutexes->head, mutexId);
    if (mutex == NULL){
        return -1;
    }

    int was_locked= mutex_lock(&mutex->status);


    if( was_locked){ //Si estaba bloqueado ==> agregarlo a la cola de procesos esperando
        if (mutex->waitingPIDs->head == NULL){
            tproc_node* newProc = mymalloc(sizeof(tproc_node));
            newProc->next = NULL;
            newProc->pid = processId;
            mutex->waitingPIDs->head = newProc;
            //sleep(processId)
        }else{
            addWaitingProc(processId, mutex->waitingPIDs->head);
            //sleep(processId)
        }

    }else{
        mutex->ownerpid = processId;
    }


    return 1;
}

int unlock(char* mutexId, uint64_t processId){
    if (mutexes == NULL || mutexes->head == NULL){
        return -1;
    }
    tmutex* mutex = containMutex(mutexes->head, mutexId);
    if (mutex == NULL){
        return -1;
    }

    if( mutex->ownerpid == processId){
        uint64_t new_pid;
        int blockedProcessFound = 0;

        //El blockedProcessFound=1 no debería ir, pero hasta que tengamos
        // la conexión a procesos lo dejo para que agarre el siguietne

        while(!blockedProcessFound && mutex->waitingPIDs->head != NULL){
            new_pid = mutex->waitingPIDs->head->pid;
            mutex->waitingPIDs->head = mutex->waitingPIDs->head-> next;
            //blockedProcessFound = isBlocked(pid);
            blockedProcessFound = 1; //
        }


        /*
         *  No hace falta desbloquear el mutex si se
         *  lo voy a dar a otro proceso. Con simplemente
         *  cambiar el ownerpid le doy el control del
         *  mutex entonces sólo el lo puede desbloquearlo
         */
         if (blockedProcessFound) {
             mutex-> ownerpid = new_pid;
             //wakeup(new_pid);
         }else{ //Si no encontré proceso, librero el mutex
             mutex-> status = UNLOCKED;
         }

         return 1;
    }

    return -1;

}

int lockIfUnlocked(char * mutexId, uint64_t processId){
    if (mutexes == NULL || mutexes->head == NULL){
        return -1;
    }
    tmutex* mutex = containMutex(mutexes->head, mutexId);
    if (mutex == NULL){
        return -1;
    }
    int was_locked= mutex_lock(&mutex->status);

    if (!was_locked){
        mutex->ownerpid = processId;
        return 1; //Puede ser lockeado
    }
    return 0; //No puede ser lockeado
}

int terminateMutex(char * mutexId, uint64_t processId){
    lock(MUTEX_MASTER_ID, processId); //Para que el borrado sea atómico

    tmutex* mutex = containMutex(mutexes->head, mutexId);
    if (mutex == NULL){ //No existe el mutex
        unlock(MUTEX_MASTER_ID, processId);
        return -1;
    }

    removeMutex(mutexId, mutex);
    unlock(MUTEX_MASTER_ID, processId);
    return 1;

}
tmutex* containMutex(tmutex_node* node, char* mutexId){
    if (strcmp(node->mutex->id, mutexId) == 0){
        return node->mutex;
    }
    if (node->next == NULL){
        return NULL;
    }
    return containMutex(node->next, mutexId);
}

void addWaitingProc(int pid, tproc_node* node){
    if (node->next == NULL){
        tproc_node* newProc = mymalloc(sizeof(tproc_node));
        newProc->next = NULL;
        newProc->pid = pid;
        node->next = newProc;
        return;
    }
    addWaitingProc(pid, node->next);
    return;
}

void removeMutex(char * mutexId, tmutex * mutex){
    if (mutex->waitingPIDs->head != NULL){
            removeAndFreeWaitingProc(mutex->waitingPIDs->head);
    }
    myfree((void*)mutex->waitingPIDs, sizeof(tproc_list));

    removeAndFreeFromMutexes(mutexId, mutexes->head);
}

void removeAndFreeWaitingProc(tproc_node* node){
    if (node == NULL){
        return;
    }
    removeAndFreeWaitingProc(node->next);
    myfree(node, sizeof(tproc_node));
}

tmutex_node* removeAndFreeFromMutexes(char * mutexId, tmutex_node* node){
    if (node == NULL){
        /*
         *  NUNCA debería entrar acá, porque sólo entaría si el mutex
         *  no estuviese en la lista de mutexes, lo cual ya checkié que está
         *  pero bueno, por las dudas de que lo vaya a querer usar en otro
         *  contexto lo dejo
         */
        return NULL;
    }
    if (strcmp(node->mutex->id, mutexId) == 0){
        tmutex_node* aux_node = node->next;
        myfree(node->mutex, sizeof(tmutex));
        myfree(node, sizeof(tmutex_node));
        return aux_node;
    }

    node->next= removeAndFreeFromMutexes(mutexId, node->next);
    return node;
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
  else if(mutexes->head->mutex->waitingPIDs == NULL)
    ncPrint("initMutexTest FAILED! : mutexes->head->mutex->waitingPIDs = NULL");
  else
    ncPrint("initMutexTest PASSED!");
}
void createMutexCreatesAMutexTest(){
    int value = createMutex("HOLA_SOY_UN_TEST", 1);
    if (value == -1){
        ncPrint("ERROR: createMutex Failed");
        return;
    }
    tmutex_node* aux = mutexes->head;
    int found = 0;
    while(aux != NULL && !found){
        if (strcmp("HOLA_SOY_UN_TEST", aux->mutex->id) == 0){
            found =1;
            ncPrint("createMutexCreatesAMutexTest PASSED! ");
        }
        aux = aux->next;
    }
    if (!found){
        ncPrint("createMutexCreatesAMutexTest FAILED! ");
    }
}

void lockofLockedMutexClaimsMutexTest(){
    createMutex("LOCKEO_EL_MUTEX", 2);
    lock("LOCKEO_EL_MUTEX", 2);

    tmutex * mutex = containMutex(mutexes->head, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
    }
    if (mutex->ownerpid == 2){
        ncPrint("lockofLockedMutexClaimsMutexTest PASSED!");
    }else{
        ncPrint("lockofLockedMutexClaimsMutexTest FAILED!");
    }

    return;

}

void lockOfLockedMutexAddsToWaitingListTest(){

    lock("LOCKEO_EL_MUTEX", 3);

    tmutex * mutex = containMutex(mutexes->head, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
    }
    if (mutex->waitingPIDs->head != NULL && mutex->waitingPIDs->head->pid == 3){
        ncPrint("lockOfLockedMutexAddsToWaitingListTest: PASSED!");
    }else{
        ncPrint("lockOfLockedMutexAddsToWaitingListTest: FAILED!");
    }

}

void unlockOfLockedMutexChangesOwnerTest(){
    unlock("LOCKEO_EL_MUTEX", 2);
    tmutex * mutex = containMutex(mutexes->head, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
    }

    if (mutex->ownerpid == 3){
        ncPrint("unlockOfLockedMutexChangesOwnerTest: PASSED!");
    }else{
        ncPrint("unlockOfLockedMutexChangesOwnerTest: FAILED!");
    }
}

void unlockWithoutWaitingChangesStatusToUnlockTest(){
    unlock("LOCKEO_EL_MUTEX", 3);

    tmutex * mutex = containMutex(mutexes->head, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("TEST ERROR: Couldn't find mutex");
    }

    if( mutex->status == UNLOCKED){
        ncPrint("unlockWithoutWaitingChangesStatusToUnlockTest: PASSED!");
    }else{
        ncPrint("unlockWithoutWaitingChangesStatusToUnlockTest: FAILED!");
    }
}

void terminateMutexEliminatesTheMutexTest(){
    terminateMutex("LOCKEO_EL_MUTEX", 4);

    tmutex * mutex = containMutex(mutexes->head, "LOCKEO_EL_MUTEX");
    if(mutex == NULL){
        ncPrint("terminateMutexEliminatesTheMutexTest: PASSED!");
    }else{
        ncPrint("terminateMutexEliminatesTheMutexTest: FAILED!");
    }
}
