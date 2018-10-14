#include <naiveConsole.h>
#include <listTest.h>
#include <buddy.h>

int idFunction(void *elem, void *id){
	return ((int*)elem) == ((int*)id);
}

void listTestSuite(){
	int *x1 = mymalloc(sizeof(int*));
	int *x2 = mymalloc(sizeof(int*));
	int *x3 = mymalloc(sizeof(int*));
	int *x4 = mymalloc(sizeof(int*));
	*x1 = 1;
	*x2 = 2;
	*x3 = 3;
	*x4 = 4;
	listADT l = createListL(idFunction, sizeof(int));
	ncPrint("The elements to test are: ");ncPrintDec(*x1);ncPrint(", ");ncPrintDec(*x2);ncPrint(", ");ncPrintDec(*x3);ncPrint(", ");ncPrintDec(*x4);
	ncScroll();
	listAdditionTest(l, x1, x2, x3);
	listRemoveFirstTest(l, x1, x2, x3);
	listRemoveTest(l, x1, x2, x3);
	removeAndFreeFirstL(l);
	listRemoveAndFreeFirstTest(l,x3);
	listRemoveAllTest(l, x1, x2, x3, x4);
	//listRemoveAndFreeAllTest(l, x1, x2, x3, x4);

}

void listAdditionTest(listADT l, int* x1, int* x2, int* x3){
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	ncPrint("listAdditionTest: ");
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3)){
		ncPrintTestPassed("PASSED!");
	}else{
		ncPrintTestFailed("FAILED!");
	}

}

void listRemoveFirstTest(listADT l, int* x1, int* x2, int* x3){
	removeFirstL(l);
	ncPrint("listRemoveFirstTest: ");
	if(!containsL(l, x1) && containsL(l, x2) && containsL(l, x3)){
		ncPrintTestPassed("PASSED!");
	}else{
		ncPrintTestFailed("FAILED!");
	}
}

void listRemoveTest(listADT l, int* x1, int* x2, int* x3){
	removeL(l, x2);
	ncPrint("listRemoveTest: ");
	if(!containsL(l, x2) && containsL(l, x3)){
		ncPrintTestPassed("PASSED!");
	}else{
		ncPrintTestFailed("FAILED!");
	}
}

void listRemoveAndFreeFirstTest(listADT l, int* x3){
	ncPrint("listRemoveAndFreeFirstTest: ");
	if(!containsL(l, x3) && isEmptyL(l)){
		ncPrintTestPassed("PASSED!");
	}else{
		ncPrintTestFailed("FAILED!");
	}
}

void listRemoveAllTest(listADT l, int* x1, int* x2, int* x3, int* x4){
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	addL(l, x4);
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3) && containsL(l, x4)){
		removeAllL(l);
		ncPrint("listRemoveAllTest: ");
		if(isEmptyL(l) && !containsL(l, x1)){
			ncPrintTestPassed("PASSED!");
		}else{
			ncPrintTestFailed("FAILED!");
		}
	}else{
		ncPrint("TEST ERROR: Test addition  ");
		ncPrintTestFailed("FAILED!");
	}
}

void listRemoveAndFreeAllTest(listADT l, int* x1, int* x2, int* x3, int* x4){
	addL(l, x1);
	addL(l, x2);
	addL(l, x3);
	addL(l, x4);
	if(containsL(l, x1) && containsL(l, x2) && containsL(l, x3) && containsL(l, x4)){
		removeAndFreeAllL(l);
		ncPrint("listRemoveAndFreeAllTest: ");
		if(isEmptyL(l) && !containsL(l, x1)){
			ncPrintTestPassed("PASSED!");
		}else{
			ncPrintTestFailed("FAILED!");
		}
	}else{
		ncPrint("TEST ERROR: Test addition  ");
		ncPrintTestFailed("FAILED!");
	}
}
