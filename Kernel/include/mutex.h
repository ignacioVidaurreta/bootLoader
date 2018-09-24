#ifndef MUTEX_H
#define MUTEX_H


#define MUTEX_MASTER_ID "__mutex_master_id__"
#define UNLOCKED	0
#define LOCKED		1
#define NULL_PID   -1

typedef struct proc_node{
  struct proc_node * next;
  struct process * p;
}tproc_node;

typedef struct proc_list{
  tproc_node * head;
}tproc_list;

typedef struct mutex{
  tproc_list * waiting_processes;
  char * id;
  uint32_t status;
  uint64_t ownerpid;
}tmutex;

typedef struct mutex_node{
  tmutex * mutex;
  tmutex * next;
}tmutex_node;

typedef struct motex_list{
  tmutex_node * head;
}tmutex_list;


void initMutex();


// TESTS
void initMutexTest();
void initMutipleTimesTest();




#endif
