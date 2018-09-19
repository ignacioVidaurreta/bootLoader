#ifndef PROCESS_H
#define PROCESS_H
#define NUM_PROCESS 100
#define STACK_SIZE 512

typedef struct process *proc;

enum p_state {RUN, WAIT, READY, ZOMBIE, DEAD};

struct process {
    uint64_t pid;
    uint64_t priority;
    enum p_state state;
    proc parent;
    uint64_t rsp;
    uint64_t stack[STACK_SIZE];
    uint8_t occupied;
};

extern struct process process_table[NUM_PROCESS];

void start_proc();
int get_new_index();
int get_new_pid();
proc get_current_proc();
void scheduler_test();
#endif
