#ifndef LIST_TEST_H
#define LIST_TEST_H

#include <listADT.h>
void listTestSuite();
void listAdditionTest(listADT l, int* x1, int* x2, int* x3);
void listRemoveFirstTest(listADT l, int* x1, int* x2, int* x3);
void listRemoveTest(listADT l, int* x1, int* x2, int* x3);
void listRemoveAndFreeFirstTest(listADT l, int* x3);
void listRemoveAllTest(listADT l, int* x1, int* x2, int* x3, int* x4);
void listRemoveAndFreeAllTest(listADT l, int* x1, int* x2, int* x3, int* x4);

#endif
