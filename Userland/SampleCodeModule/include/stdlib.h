#ifndef _STDLIB_H
#define _STDLIB_H


#define BUFFER_SIZE 256

/*turns the given integer into a string and stores it in
** the given array
*/
char* intToString(int num,char * ret);

/*returns the length of the given string*/
int strlen(const char * str);

/*prints the given string replacing %d with the integer
** parameter received and %s with the string parameter
** and %c with the char parameter received
*/
void printf(const char * str,...);

/*
 *  Uses Syscall Write to write on screen
 *
 */
 void putChar(char c);

 void swap(char* numStr, int index1, int index2);

 /*
  * Copies the string from src to dest
  */
 char * strcpy(char * dest, char* src);




#endif
