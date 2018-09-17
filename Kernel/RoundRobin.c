#include <stdio.h>
#include "RoundRobin.h"
#include "buddy.h"
#include "include/naiveConsole.h"

void round_robin(tHeader* process_queue, int max_rounds) {

   int ticks = 0;
   int i=0;

   for (int round = 0; round < max_rounds; ticks++) {

       //El agregado de nodos es para simular la llegada de procesos.
       tNode *node = mymalloc(sizeof(tNode));
       node->num = i++;
       add_to_queue(process_queue, node);

       if (ticks == QUANTUM) {
           ticks = 0;
           round++;
           if (process_queue->first != NULL) {
               process_queue->first->quantum_duration--;
               if (process_queue->first->quantum_duration >0){
                   process_queue->last = process_queue->first;
               }
               process_queue->first = process_queue->first->next;
           }

       }
   }

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

     queue_header = mymalloc(sizeof(tHeader));
     queue_header->first = NULL;
     queue_header->last = NULL;

     node->num=3;
     add_to_queue(queue_header, node);
     if (node->num == queue_header->first->num){
         ncPrint("Test01: PASSED! ");
     }else{
         ncPrint("Test01: FAILED! ");
     }

     myfree(node, sizeof(*node));
     myfree(queue_header, sizeof(*queue_header));
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


    node1->num = 3;
    node2->num = 5;
    add_to_queue(queue_header, node1);
    add_to_queue(queue_header, node2);
    if(node1->num == queue_header->first->num){
        ncPrint("IGUAL 1");
    }
    if (node2->num == queue_header->last->num){
        ncPrint("IGUAL 2");
    }
    if(node1->num == queue_header->first->num && node2->num == queue_header->last->num){
        ncPrint("Test02: PASSED! ");
    }else{
        ncPrint("Test02: FAILED! ");
    }
    myfree(node1, sizeof(*node1));
    myfree(node2, sizeof(*node2));
    myfree(queue_header, sizeof(*queue_header));

 }

 void testAddALotOfElementsToQueue(){

     //Create queue
     tHeader* queue;
     queue = mymalloc(sizeof(tHeader));
     queue->first = NULL; //sentinels
     queue->last = NULL;

     //Create nodes
     tNode* nodes[100];
     for (int i =0; i<100; i++){
         nodes[i] = mymalloc(sizeof(tNode));
         nodes[i]->num = i;
         add_to_queue(queue, nodes[i]);
     }

     int equals = 1;
     tNode* aux;
     aux = queue->first;
     int j =0;
     while(aux != NULL){
         if(j++ != aux->num)
             equals = 0;
         aux = aux->next;
     }

     if(equals){
         ncPrint("Test03: PASSED! ");
     }else{
         ncPrint("Test03: FAILED! ");
     }



     for (int i=0; i<100; i++){
         myfree(nodes[i], sizeof(*nodes[i]));
     }

     myfree(queue, sizeof(*queue));
 }


 void testRoundRobin(){
     //Create Queue
     int num=5;
     tHeader *process_queue = mymalloc(sizeof(tHeader));
     process_queue->first = NULL;
     process_queue->last = NULL;

     round_robin(process_queue, num);
     if( num == process_queue->first->num){
         ncPrint("Test04: PASSED! ");
     }else{
         ncPrint("Test04: FAILED! ");
     }

     free_queue_nodes(process_queue->first);
     myfree(process_queue,sizeof(*process_queue));
}

void testNotFinishedProcessGoesToTail(){

    tHeader *process_queue = mymalloc(sizeof(tHeader));
    process_queue->first = NULL;
    process_queue->last = NULL;

    tNode* node = mymalloc(sizeof(tNode));
    node->num = -1;
    node->quantum_duration = 2;

    add_to_queue(process_queue, node);

    round_robin(process_queue, 1);

    if(node->num == process_queue->last->num){
      ncPrint("Test05: PASSED");
        return 1;
    }else{
        ncPrint("Test05: FAILED!");
        return 0;
    }

    free_queue_nodes(process_queue->first);
    myfree(process_queue, sizeof(*process_queue));
    myfree(node, sizeof(*node));

}

void free_queue_nodes(tNode* node) {
    if (node == NULL){
        return;
    }
    free_queue_nodes(node->next);
    myfree(node, sizeof(*node));
}
