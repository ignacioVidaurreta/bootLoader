#include <listADT.h>
#include <buddy.h>

struct node{
	void *elem;
	struct node* next;
};

typedef struct node *Node;

struct listCDT{
	Node first;
	Node last;
	int (*idFunction)(void*, void*);
	int dataSize;
};

Node removeR(Node n, int (*idFunction)(void*, void*), void *elem, int free, int dataSize);
void removeFirstAndMaybeFree(listADT list, int free);
void removeAllAndMaybeFree(listADT list, int free);
void deleteNode(Node n, int free, int dataSize);
void *getR(Node current, int (*idFunction)(void*, void*), void *id);
int containsR(Node current, int (*idFunction)(void*, void*), void *id);

listADT createListL(int(idFunction)(void*,void*), int dataSize){
	listADT list = mymalloc(sizeof(struct listCDT));
	list->idFunction = idFunction;
	list->dataSize = dataSize;
	list->first = 0;
	list->last = 0;
	return list;
}

int addL(listADT list, void *elem){
	Node n = mymalloc(sizeof(struct node));
	if(n == 0)
		return FAILURE;
	n->elem = elem;
	n->next = 0;
	if(list->last == 0){
		list->last = n;
		list->first = n;
		return SUCCESS;
	}
	list->last->next = n;
	list->last = n;
	return SUCCESS;
}

void removeL(listADT list, void *elem){
	list->first = removeR(list->first, list->idFunction, elem, 0, list->dataSize);
}

void removeAndFreeL(listADT list, void *elem){
	list->first = removeR(list->first, list->idFunction, elem, 1, list->dataSize);
}

Node removeR(Node n, int (*idFunction)(void*, void*), void *elem, int free, int dataSize){
	if(n == 0)
		return FAILURE;
	if(idFunction(n->elem, elem) != 0){
		n->next = removeR(n->next, idFunction, elem, free, dataSize); 
		return n;
	}
	Node aux = n->next;
	deleteNode(n, free, dataSize);
	return aux;
}

void deleteNode(Node n, int free, int dataSize){
	if(free)
		myfree(n->elem, dataSize);
	myfree(n, sizeof(struct node));
}

void *getL(listADT list, void *id){
	return getR(list->first, list->idFunction, id);
}

void *getR(Node current, int (*idFunction)(void*, void*), void *id){
	if(current == 0)
		return FAILURE;
	if(idFunction(current->elem, id) != 0)
		return getR(current->next, idFunction, id);
	return current->elem;
}

void removeFirstL(listADT list){
	removeFirstAndMaybeFree(list, 0);
}

void removeAndFreeFirstL(listADT list){
	removeFirstAndMaybeFree(list, 1);
}

void removeFirstAndMaybeFree(listADT list, int free){
	Node aux = list->first->next;
	deleteNode(list->first, free, list->dataSize);
	list->first = aux;
}

void removeAllL(listADT list){
	removeAllAndMaybeFree(list, 0);
}

void removeAndFreeAllL(listADT list){
	removeFirstAndMaybeFree(list, 1);
}

void removeAllAndMaybeFree(listADT list, int free){
	while(list->first != 0)
		removeFirstAndMaybeFree(list, free);
}

void *getFirstL(listADT list){
	return list->first->elem;
}

int containsL(listADT list, void *id){
	return containsR(list->first, list->idFunction, id);
}

int containsR(Node current, int (*idFunction)(void*, void*), void *id){
	if(current == 0)
		return 0;
	if(idFunction(current->elem, id) != 0)
		return containsR(current->next, idFunction, id);
	return SUCCESS;
}
