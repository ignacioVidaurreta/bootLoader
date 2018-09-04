#ifndef PROCESS_H
#define PROCESS_H

typedef struct process *proc;

enum p_state {RUN, WAIT, READY, ZOMBIE, DEAD};

struct process {
    uint64_t pid;
    uint64_t priority;
    enum p_state state;
    proc parent;
    uint64_t regs[7];
}


#endif
