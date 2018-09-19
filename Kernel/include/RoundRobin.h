#ifndef ROUNDROBIN_ROUNDROBIN_H
#define ROUNDROBIN_ROUNDROBIN_H

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

void round_robin();
void add_to_queue(tHeader *queue_header, tNode *node);
void free_queue_nodes(tNode* queue);


/* TESTS */
void testAddElementToHeader();
void testAddMultipleElementsToHeader();
void testAddALotOfElementsToQueue();
void testRoundRobin();
void testNotFinishedProcessGoesToTail();





#endif //ROUNDROBIN_ROUNDROBIN_H
