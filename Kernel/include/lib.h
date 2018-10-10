#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#define NULL 0

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char* strcpy(char* dest,const char* src);
int strcmp(const char* str1,const char* str2);
int strlen(const char* str);
char* concat(const char* str1, const char* str2);
char * intToString(int num, char * numStr);
void swap(char* numStr, int index1, int index2);

char *cpuVendor(char *result);

#endif
