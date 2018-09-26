#ifndef ROUNDROBIN_ROUNDROBIN_H
#define ROUNDROBIN_ROUNDROBIN_H

#include "process.h"
#define QUANTUM 5

typedef struct node {
    struct node* next;
    /*
    int quantum_duration;
    int num;
    */
    struct process*  p;
}tNode;

typedef struct{
    tNode* last;
    tNode* first;


}tHeader;

extern tHeader * ready_queue;

proc round_robin(tHeader *process_queue);
void add_to_queue(tHeader *queue_header, tNode *node);
void add_proc_to_queue(tHeader *queue_header, proc p);
void free_queue_nodes(tNode* queue);
tNode *pop_queue_node(tHeader *queue_header);


/* TESTS */
void testAddElementToHeader();
void testAddMultipleElementsToHeader();
void testAddALotOfElementsToQueue();


void proc_cascade();





#endif //ROUNDROBIN_ROUNDROBIN_H
