//
// Created by elodin on 05/09/18.
//

#ifndef ROUNDROBIN_BUDDY_H
#define ROUNDROBIN_BUDDY_H

typedef unsigned int mysize_t;

typedef struct buddyList BUDDYLIST;

    struct buddyList {
        /*
         * TAG(P) == 0 SI el bloque de dirección P está reservado
         * TAG(P) == 1 SI el bloque de dirección P está disponible
         */
        int available;
        /*
         * Size is 2**K KVAL sería el power
         */
        int order;

        /*
         *  FORWARD
         */
        BUDDYLIST *next;
        /*
         * BACK
         */
        BUDDYLIST *prev;
    };


    void initBuddySystem();
    void *mycalloc(mysize_t,mysize_t);
    void *mymalloc(mysize_t);
    void myfree(void *);
    int nextPower(mysize_t);
    int searchEmptyIndex(int);
    void printBuddyLists();
    int buddy(int, int);



#endif //ROUNDROBIN_BUDDY_H
