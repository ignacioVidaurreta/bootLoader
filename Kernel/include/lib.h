#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char* strcpy(char* dest,const char* src);
int strcmp(const char* str1,const char* str2);
int strlen(const char* str);
char* concat(const char* str1, const char* str2);

char *cpuVendor(char *result);

#endif
