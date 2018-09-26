#include <stdint.h>
#include "buddy.h"
void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

char* strcpy(char* dest,const char* src){
  if (src == 0){
    return 0;
  }
  int i=0;
  for (; src[i] != 0; i++){
    dest[i] = src[i];
  }
  dest[i] = 0;
  return dest;
}



int strcmp(const char* str1,const char* str2){;
  int i=0;
  for (i = 0; str1[i] != 0; i++){
    if(str1[i] != str2[i]){
      return (str1[i]>str2[i])?1:-1;
    }
  }
  return str1[i] - str2[i];
}

int strlen(const char* str){

	int i = 0;
	while( str[i] != 0){
		i++;
	}

	return i;

}

char* concat(const char* str1, const char* str2){

	int s1 = strlen(str1);
	int s2 = strlen(str2);
	char * aux = (char*) mymalloc(s1 +s2 +1);
	int i;
	for (i=0; str1[i] != 0; i++)
	{
		aux[i] = str1[i];
	}
	for (int j = 0; str2[j] != 0; j++){
		aux[i++] = str2[j];
	}
	aux[i] = 0;

	return aux;
}
