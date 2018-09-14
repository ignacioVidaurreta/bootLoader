#include <stdio.h>
#include <buddy.h>

#define MAX_MEM_SIZE 128*4096
#define MAX_MEM_POWER 19

static BUDDYLIST AVAIL[32];
void *pool = NULL;

static int flag=0; // To check if buddy system is initialized or not.

/*
 *initBuddy System initializes the buddy system.
 *
 */
void initBuddySystem(){

    int i;
    BUDDYLIST *myList;
    if(flag == 0)
    {
        pool = 0x1000000;

        for(i=0; i< MAX_MEM_POWER; i++)
        {
            AVAIL[i].next = AVAIL[i].prev = &AVAIL[i]; //Foward y Back apuntan a sí mismo
            AVAIL[i].available = 0; //Memoria reservada
            AVAIL[i].order = i; //Nivel
        }

        /* AVAILF[m] = AVAILB[m] = 0 */

        /*
         *  Genera un bloque de memoria maxima libre
         */
        AVAIL[i].next = AVAIL[i].prev = pool;
        AVAIL[i].available = 1; //Memoria Libre
        AVAIL[i].order = MAX_MEM_POWER;

        //printf(" Init Buddy  i = %d\n",i);

        /* LINKf(0) == prev(0) = LOC(AVAIL[m])
         * available(0) = 1, order (0) = m
         */
        myList = AVAIL[i].next;
        myList->next = myList->prev = &AVAIL[i];
        myList->available = 1;
        myList->order = i;
    }
    flag = 1;
}

/*
 *mymalloc allocates memory of size 'a'
 *similar to function malloc
 *
 */
void *mymalloc(mysize_t a){

    int j=0,k=0;

    if(!flag)
        initBuddySystem(MAX_MEM_SIZE); //Inicializa un bloque de memoria máxima

    BUDDYLIST *L=NULL,*P=NULL;

    // Size that needs to be malloce'd would also contain HEADER info
    int size = a + sizeof(BUDDYLIST);

    // Finding the next power of size to reserve a block of 2^k
    k = nextPower(size);
    if(k == 127){
        // give error ENOMEM
      //  printf("NO PROPER BLOCK SIZE");
        return (NULL);
    }
    else {

        // [Find Block]
        j = searchEmptyIndex(k);
        if(j != 127){

            /* [ Remove from list ]
             * L <-- AVAILF[j], ABAILF[j] <-- next(L),
             * prev(next(L)) <-- LOC(AVAIL[j]), and available(L) <-- 0
             */
            L = AVAIL[j].next;
            AVAIL[j].next = L->next;
            L->next->prev = &AVAIL[j];
            L->available = 0;
            L->order = k;

            /* [Split] Decrease j by 1
             *  P <-- L + 2^j, available(P) <-- 1, order(P) <-- j,
             * next(P) <-- LOC(AVAIL[j]), prev(P) <-- LOC (AVAIL[j])
             * AVAILF[j] <-- AVAILB[j] <-- P
             */
            while(j != k){
                j--;
                P = L + (1<<j)/sizeof(BUDDYLIST); // 1<<j == 2**j
                P->available = 1;
                P->order = j;
                P->next = P->prev = &AVAIL[j];
                AVAIL[j].next = AVAIL[j].prev = P;
                AVAIL[j].available = 1;
            }


        }
        else{
            // No Free block available
          //  printf("MEMORY FULLY ALLOCATED");

        }

    }
    //memset(L+1,0,a); Felt would remove garbage value !!
    return (void *)(L+1);
}

/*
 *myfree free's the memory of 'a'
 *similar to free
 */

void myfree(void *a){

    // Free up block
    BUDDYLIST *L=NULL,*P=NULL;
    if(a == NULL)
        return ;

    L = (BUDDYLIST *)(a) -1;

    int k = L->order;

    // s1 [Is Buddy availbale ?]
    P = (BUDDYLIST *)(buddy(k,(int)L));


    /* s2 [Combine with Buddy]
     * LINKf(prev(P) <-- next(P), prev(next(P)) <-- prev(P)
     * set k <-- k + 1 and if P < L set L <-- P
     */
    while(!((k == MAX_MEM_POWER) || (P->available == 0) || ((P->available == 1) && (P->order!=k)))) {
        P->prev->next = P->next;
        P->next->prev = P->prev;
        k++;
        if(P < L)
            L = P;
        P = (BUDDYLIST *)(buddy(k,(int)L));
    }

    /*s3 [Put on List] set
     *  available(L) <-- 1, next(L) <-- AVAILF[k], prev(AVAILF[k]) <-- L,
     *  order (L) <-- k, prev(L) <-- LOC (AVAIL[k]), AVAILF[k] <-- L.
     */
    L->available = 1;
    L->next = AVAIL[k].next;
    AVAIL[k].next->prev = L;
    L->order = k;
    L->prev = &AVAIL[k];
    AVAIL[k].next = L;

}

/*
 *mycalloc malloc's memory and
 *initializes it to 0. similar to calloc
 */
void *mycalloc(mysize_t a,mysize_t b){

    void *c = mymalloc(a*b);
    if(c!=NULL)
    {
        memset(c,0,a*b);
        return c;
    }
    else
        return NULL;

}

/*
 *next power is used to calculate
 *to next highest power to allocate a free block
 */
int nextPower(mysize_t a){

    int i=0,j=0;
    while(i < MAX_MEM_SIZE)
    {
        if(a < i){
            //printf(" power = %d\n",j);
            return j-1;
        }
        else{
            i = 1<<j;
            j++;
        }
    }
    return 127;
}

/*
 *searching for a free block index
 * [FIND BLOCK]
 */
int searchEmptyIndex(int k){
    int j=0;
    for(j = k; j <= MAX_MEM_POWER; j++)
    {
        /*
         * next apunta a AVAIL si no hay bloques libres en esa posición.
         */
        if(AVAIL[j].next != &AVAIL[j])
        {
            //printf("Empty Index = %d\n",j);
            return j;
        }
    }
    return 127;
}

/*
 *used to find the buddy's address
 *
 */
int buddy(int k,int x){

    return (((x - (int)pool)^(1<<k)) + (int)pool);

}
