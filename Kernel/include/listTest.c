#include <listADT.h>
#include <naiveConsole.h>

int idFunction(void *elem, void *id){
	return ((int*)elem) == ((int*)id);
}

void start(){
	int *x1 = mymalloc(sizeof(int));
	int *x2 = mymalloc(sizeof(int));
	int *x3 = mymalloc(sizeof(int));
	int *x4 = mymalloc(sizeof(int));
	*x1 = 1;
	*x2 = 2;
	*x3 = 3;
	*x4 = 4;

	listADT l = createListL(idFunction, sizeof(int));
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	if(!containsL(l, x4)){
		ncPrint("1: l does not contain 4");
		ncScroll();
	}
	addL(l, x4);
	if(containsL(l, x4)){
		ncPrint("2: l contains 4");
		ncScroll();
	}
	removeL(l, x4);
	if(!containsL(l, x4)){
		ncPrint("3: l does not contain 4");
		ncScroll();
	}
	removeFirstL(l);
	if(!containsL(l, x1)){
		ncPrint("4: l does not contain 1");
		ncScroll();
	}
	removeAllL(l);
	if(!containsL(l, x2) && !containsL(l, x3)){
		ncPrint("5: l contains neither 3 nor 4");
		ncScroll();
	}
}