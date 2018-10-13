#include <listADT.h>
#include <buddy.h>
#include <naiveConsole.h>

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
	int size;
};

Node removeR(listADT list, Node n, void *elem, int free);
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
	list->size = 0;
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
		list->size++;
		return SUCCESS;
	}
	list->last->next = n;
	list->last = n;
	list->size++;
	return SUCCESS;
}

void *getL(listADT list, void *id){
	return getR(list->first, list->idFunction, id);
}

void *getR(Node current, int (*idFunction)(void*, void*), void *id){
	if(current == 0)
		return FAILURE;
	if(!idFunction(current->elem, id))
		return getR(current->next, idFunction, id);
	return current->elem;
}

void *getFirstL(listADT list){
	if(list->first == 0)
		return 0;
	return list->first->elem;
}

void removeL(listADT list, void *elem){
	list->first = removeR(list, list->first, elem, 0);
	if(list->first == 0)
		list->last = 0;
}

void removeAndFreeL(listADT list, void *elem){
	list->first = removeR(list, list->first, elem, 1);
	if(list->first == 0)
		list->last = 0;
}

Node removeR(listADT list, Node n, void *elem, int free){
	if(n == 0)
		return 0;
	if(!list->idFunction(n->elem, elem)){
		n->next = removeR(list, n->next,elem, free);
		if(n->next == 0)
			list->last = n;
		return n;
	}
	Node aux = n->next;
	deleteNode(n, free, list->dataSize);
	list->size--;
	return aux;
}

void removeFirstL(listADT list){
	removeFirstAndMaybeFree(list, 0);
}

void removeAndFreeFirstL(listADT list){
	removeFirstAndMaybeFree(list, 1);
}

void removeFirstAndMaybeFree(listADT list, int free){
	if(list->first == 0)
		return;
	Node aux = list->first->next;
	deleteNode(list->first, free, list->dataSize);
	list->first = aux;
	list->size--;
	if(aux == 0)
		list->last = 0;
}

void removeAllL(listADT list){
	removeAllAndMaybeFree(list, 0);
}

void removeAndFreeAllL(listADT list){
	removeAllAndMaybeFree(list, 1);
}

void removeAllAndMaybeFree(listADT list, int free){
	while(list->first != 0)
		removeFirstAndMaybeFree(list, free);
	list->size = 0;
}

void deleteNode(Node n, int free, int dataSize){
	if(free)
		myfree(n->elem, dataSize);
	myfree(n, sizeof(struct node));
}

int containsL(listADT list, void *id){
	return containsR(list->first, list->idFunction, id);
}

int containsR(Node current, int (*idFunction)(void*, void*), void *id){
	if(current == 0)
		return 0;
	if(!idFunction(current->elem, id))
		return containsR(current->next, idFunction, id);
	return 1;
}

int sizeL(listADT list){
	return list->size;
}

int isEmptyL(listADT list){
	return list->size == 0;
}
