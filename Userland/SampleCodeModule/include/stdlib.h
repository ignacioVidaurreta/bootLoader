#ifndef _STDLIB_H
#define _STDLIB_H

  #include <stdint.h>
  #define BUFFER_SIZE 80
  #define MSG_QUEUE_CLOSE_SYS_CALL 1
  #define MSG_QUEUE_OPEN_SYS_CALL 2
  #define MSG_QUEUE_SEND_SYS_CALL 3
  #define MSG_QUEUE_RECEIVE_SYS_CALL 4
  #define MUTEX_OPEN_SYS_CALL 5
  #define MUTEX_CLOSE_SYS_CALL 6
  #define MUTEX_POST_SYS_CALL 7
  #define MUTEX_WAIT_SYS_CALL 8

  extern uint64_t int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID);

  void createMessageQueue(char *id);
  void closeMessageQueue(char *id);
  void sendMessage(char *id, void *msg, int *msgSize);
  void* receiveMessage(char *id);
  void startProcess(char *procName, void *procPointer);

  /*
  ** turns the given integer into a string and stores it in
  ** the given array
  */
  char* intToString(int num,char * ret);

  /*
  ** returns the length of the given string
  */
  int strlen(const char * str);

  int strcmp(char* str1, char* str2);

  /*
  ** prints the given string replacing %d with the integer
  ** parameter received and %s with the string parameter
  ** and %c with the char parameter received
  */
  void printf(const char * str,...);

  /*
  ** Uses Syscall Write to write on screen
  **
  */
   void putChar(char c);

   void swap(char* numStr, int index1, int index2);

   /*
   ** Copies the string from src to dest
    */
   char * strcpy(char * dest, char* src);

   int scanf(const char* format, ...);

   char* readBuffer();

   int isNum(char c);

   char readChar();

   void deleteCharacter();

   char* strncpy(char *destination, const char *source, int n);
   int strncmp(const char* str1, const char* str2, int n);


#endif
