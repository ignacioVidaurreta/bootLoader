#ifndef LIST_ADT_H
#define LIST_ADT_H
#define SUCCESS 1
#define FAILURE 0

// the following handles a list of elements each of which must have an id
// the addition operation will take the actual element but all following
// operations such as remove or get will take the id.
// Thus, the identifying function will take the element and the id and
// return true (different than cero) if the element corresponds to that id.
//
// The list does accept repeats, if this is not desired the user must control
// additions with the contains function.

typedef struct listCDT *listADT;

listADT createListL(int (*idFunction)(void*, void*), int datasize);
int addL(listADT list, void *elem);
void removeL(listADT list, void *id);
void removeAndFreeL(listADT list, void *id);
void removeFirstL(listADT list);
void removeAndFreeFirstL(listADT list);
void removeAllL(listADT list);
void removeAndFreeAllL(listADT list);
void *getL(listADT list, void *id); 
void *getFirstL(listADT list);
int containsL(listADT list, void *id);
void destroyListL(listADT list);
int sizeL(listADT list);
int isEmptyL(listADT list);

#endif