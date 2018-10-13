#include <stdio.h>
#include <buddy.h>
#include "naiveConsole.h"

#define sizes 20
/* pointers to the free space lists */
void * freelists[sizes];

/* blocks in freelists[i] are of size 2**i. */
#define BLOCKSIZE(i) (1 << (i))

/* the address of the buddy of a block from freelists[i]. */
#define BUDDYOF(b,i) ((void *)( ((long)b) ^ (1 << (i)) ))


void * mymalloc( int size )
{
    int i;

    /* compute i as the least integer such that i >= log2(size) */
    for (i = 0; BLOCKSIZE( i ) < size; i++);

    if (i > sizes) {
        return NULL;
    /*
     *  Saqué el && freelists[i] == NULL porque freelists no puede
     *  acceder a la posición 20
     */
    } else if (i == sizes ) {
        return (void*)0x1000000; //Caso inicial
    } else if (freelists[i] != NULL) {

            /* we already have the right size block on hand */
            void * block;
            block = freelists[i];
            freelists[i] = *(void * *)freelists[i];
            return block;

    } else {

        /* we need to split a bigger block */
        void * block,*buddy;
        block = mymalloc( BLOCKSIZE( i + 1 ) );

        if (block != NULL) {
            /* split and put extra on a free list */
            buddy = BUDDYOF( block, i );
            *(void * *)buddy = freelists[i];
            freelists[i] = buddy;
        }

        return block;
  }
}

void myfree( void * block, int size )
{
    int i;
    void * * p;
    void * buddy;

    /* compute i as the least integer such that i >= log2(size) */
    for (i = 0; BLOCKSIZE( i ) < size; i++);

    /* see if this block's buddy is free */
    buddy = BUDDYOF( block, i );
    p = &freelists[i];
    while ((*p != NULL) && (*p != buddy)) p = (void * *)*p;

    if (*p != buddy) {

        /* buddy not free, put block on its free list */
        *(void * *)block = freelists[i];
        freelists[i] = block;

    } else {

        /* buddy found, remove it from its free list */
        *p = *(void * *)buddy;

        /* deallocate the block and its buddy as one block */
        if (block > buddy) {
            myfree( buddy, BLOCKSIZE( i + 1 ));
        } else {
            myfree( block, BLOCKSIZE( i + 1 ));
        }
    }
}

void print_free_memory(){
  ncPrint("Free memory blocks of order 2^N available, starting address: 0x1000000");
  ncScroll();
  ncPrint("-----------------------------");
  ncScroll();
  ncPrint("N");
  ncPrint("          ");
  ncPrint("first available at");
  ncScroll();
  ncPrint("-----------------------------");
  ncScroll();
  int i=0;
  while(i < sizes){
    if(freelists[i]!=NULL){
      ncPrintDec(i);
      ncPrint(":         ");
      ncPrintDec((uint64_t)freelists[i]);
      ncScroll();
    }
    i++;
  }
}
