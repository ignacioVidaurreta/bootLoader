#include <stdlib.h>
#include "wait.h"
#include "process.h"
#include "buddy.h"

extern void ctx_switch();
static tHeader *wait_queue;

void wait(uint64_t pid) {
    proc current_proc = get_current_proc();
    current_proc->waitpid = pid;
    add_proc_to_queue(wait_queue, current_proc);
    current_proc->state = WAIT;
    ctx_switch();
}

void delete_from_wait(proc p) {
    tNode *node = wait_queue->first;
    if (node == NULL) {
        return;
    }
    while (node->next != NULL && node->next->p != p) {
        node = node->next;
    }
    if (node->next != NULL) {
        tNode *temp = node->next;
        node->next = temp->next;
        free_queue_nodes(temp);
    }
}

void init_wait_queue() {
    wait_queue = mymalloc(sizeof(tHeader *));
}

