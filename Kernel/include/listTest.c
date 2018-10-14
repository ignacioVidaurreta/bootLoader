#include <listADT.h>
#include <naiveConsole.h>

int idFunction(void *elem, void *id){
	return ((int*)elem) == ((int*)id);
}

void startListTest(){
	int *x1 = mymalloc(sizeof(int));
	int *x2 = mymalloc(sizeof(int));
	int *x3 = mymalloc(sizeof(int));
	int *x4 = mymalloc(sizeof(int));
	*x1 = 1;
	*x2 = 2;
	*x3 = 3;
	*x4 = 4;
	listADT l = createListL(idFunction, sizeof(int));
	ncPrint("1: the elements to test are: ");ncPrintDec(*x1);ncPrint(", ");ncPrintDec(*x2);ncPrint(", ");ncPrintDec(*x3);ncPrint(", ");ncPrintDec(*x4);
	ncScroll();
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3))
		ncPrintLn("2: addition works");
	removeFirstL(l);
	if(!containsL(l, x1) && containsL(l, x2) && containsL(l, x3))
		ncPrintLn("3: removal of first works");
	removeL(l, x2);
	if(!containsL(l, x2) && containsL(l, x3))
		ncPrintLn("4: removal works");
	removeAndFreeFirstL(l);
	if(!containsL(l, x3) && isEmptyL(l))
		ncPrintLn("5: removal and free of first works");
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	addL(l, x4);
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3) && containsL(l, x4)){
		removeAllL(l);
		if(isEmptyL(l) && !containsL(l, x1))
			ncPrintLn("6: removal of all works");
	}
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	addL(l, x4);
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3) && containsL(l, x4)){
		removeAndFreeAllL(l);
		if(isEmptyL(l) && !containsL(l, x1))
			ncPrintLn("7: removal and free of all works");
	}
}
