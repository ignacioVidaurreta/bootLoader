#include <stdio.h>
#include "RoundRobin.h"
#include "buddy.h"
#include "include/naiveConsole.h"

proc round_robin(tHeader *process_queue) {
    proc temp = get_current_proc();
    if (process_queue->first != NULL) {
        tNode *node = pop_queue_node(process_queue);
        if (temp->pid && temp->state != DEAD) {
            temp = node->p;
            node->p = get_current_proc();
            add_to_queue(process_queue, node);
        } else {
            temp = node->p;
        }
    }
    return temp;
}


tNode *pop_queue_node(tHeader *queue_header) {
    tNode *node = queue_header->first;
    if (node != NULL) {
        if (node->next == NULL) {
            queue_header->last = NULL;
        }
        queue_header->first = node->next;
    }
    return node;
}

void add_proc_to_queue(tHeader *queue_header, proc p) {
    tNode *node = mymalloc(sizeof(tNode));
    node->p = p;
    add_to_queue(queue_header, node);
}


 void add_to_queue(tHeader *queue_header, tNode *node){
    node->next = NULL;
    if (queue_header-> last == NULL){
        queue_header->first = node;
    }else{
        queue_header->last->next = node;
    }

    queue_header->last = node;

 }


 void testAddElementToHeader(){
     tHeader* queue_header;
     tNode * node;
     node = mymalloc(sizeof(tNode));
     proc p1 = mymalloc(sizeof(struct process));
     p1->state = READY;
     node->p = p1;

     queue_header = mymalloc(sizeof(tHeader));
     queue_header->first = NULL;
     queue_header->last = NULL;

     node->p->pid = 5;
     add_to_queue(queue_header, node);
     if (node->p->pid == queue_header->first->p->pid){
         ncPrint("testAddElementToHeader: PASSED! ");
     }else{
         ncPrint("testAddElementToHeader: FAILED! ");
     }
     //no hacer doble free, ver que
     myfree(node, sizeof(*node));
     myfree(queue_header, sizeof(*queue_header));
     myfree(p1, sizeof(struct process));
}


void testAddMultipleElementsToHeader(){
    tHeader* queue_header;
    tNode * node1;
    tNode * node2;

    queue_header = mymalloc(sizeof(tHeader));

    queue_header->first = NULL;
    queue_header->last = NULL;

    node1 = mymalloc(sizeof(tNode));
    node2 = mymalloc(sizeof(tNode));

    proc p1 = mymalloc(sizeof(struct process));
    p1->state = READY;
    node1->p = p1;

    proc p2 = mymalloc(sizeof(struct process));
    p2->state = READY;
    node2->p = p2;


    node1->p->pid = 3;
    node2->p->pid= 5;
    add_to_queue(queue_header, node1);
    add_to_queue(queue_header, node2);
    if(node1->p->pid == queue_header->first->p->pid && node2->p->pid == queue_header->last->p->pid){
        ncPrint("testAddMultipleElementsToHeader: PASSED!");
    }else{
        ncPrint("testAddMultipleElementsToHeader: FAILED!");
    }
    myfree(node1, sizeof(*node1));
    myfree(node2, sizeof(*node2));
    myfree(queue_header, sizeof(*queue_header));

    myfree(p1, sizeof(struct process));
    myfree(p2, sizeof(struct process));
 }


 void testAddALotOfElementsToQueue(){

     //Create queue
     tHeader* queue;
     queue = mymalloc(sizeof(tHeader));
     queue->first = NULL; //sentinels
     queue->last = NULL;

     //Create nodes
     tNode* nodes[100];
     proc ps[100];
     for (int i =0; i<100; i++){
         nodes[i] = mymalloc(sizeof(tNode));
         ps[i] = mymalloc(sizeof(struct process));
         ps[i]->pid = i;
         nodes[i]->p = ps[i];
         add_to_queue(queue, nodes[i]);
     }

     int equals = 1;
     tNode* aux;
     aux = queue->first;
     int j =0;
     while(aux != NULL){
         if(aux->p->pid!= j++)
             equals = 0;
         aux = aux->next;
     }

     if(equals){
         ncPrint("testAddALotOfElementsToQueue: PASSED! ");
     }else{
         ncPrint("testAddALotOfElementsToQueue: FAILED! ");
     }



     for (int i=0; i<100; i++){
         myfree(nodes[i], sizeof(*nodes[i]));
     }

     for( int i=0; i<100; i++){
         myfree(ps[i], sizeof(*ps[i]));
     }

     myfree(queue, sizeof(*queue));
 }



void free_queue_nodes(tNode* node) {
    if (node == NULL){
        return;
    }
    free_queue_nodes(node->next);
    myfree(node, sizeof(*node));
}


void proc_cascade(){

       //Create queue
       tHeader* queue;
       queue = mymalloc(sizeof(tHeader));
       queue->first = NULL; //sentinels
       queue->last = NULL;

       //Create nodes
       tNode* nodes[100];
       proc ps[100];
       for (int i =0; i<100; i++){
           nodes[i] = mymalloc(sizeof(tNode));
           ps[i] = mymalloc(sizeof(struct process));

           for(int j=0;j<i;j++){
             ncPrint("+");
           }
           ncScroll();
           ps[i]->pid = i;
           nodes[i]->p = ps[i];
           add_to_queue(queue, nodes[i]);
       }

       for (int i=0; i<100; i++){
           for(int j=0;j<100-i;j++){
             ncPrint("-");
           }
           ncScroll();
           myfree(nodes[i], sizeof(*nodes[i]));
       }

       for( int i=0; i<100; i++){
           myfree(ps[i], sizeof(*ps[i]));
       }

       myfree(queue, sizeof(*queue));
   }
