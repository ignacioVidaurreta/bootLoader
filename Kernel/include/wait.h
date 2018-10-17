#ifndef WAIT_H
#define WAIT_H
#include "RoundRobin.h"
void wait(uint64_t pid);
void delete_from_wait(proc p);
void init_wait_queue();
void signal();
#endif
