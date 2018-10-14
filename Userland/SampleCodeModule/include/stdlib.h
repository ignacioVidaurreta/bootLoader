#ifndef _STDLIB_H
#define _STDLIB_H

  #include <stdint.h>
  #define STDIN 0
  #define STDOUT 1
  #define BUFFER_SIZE 80
  #define SYS_NEW_PROC 13
  #define SEND_MAILBOX  15
  #define RECEIVE_MAILBOX 16
  #define CREATE_MAILBOX  17
  #define DESTROY_MAILBOX 18
  #define CREATE_MUTEX  19
  #define LOCK_MUTEX  20
  #define UNLOCK_MUTEX  21
  #define SYS_FD 22
  #define TERMINATE_MUTEX 23
  #define ALLOCATE_MEMORY 24
  #define FREE 25
  #define KILL 29
  

  extern uint64_t int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID);

  void createMessageQueue(char *id);
  void closeMessageQueue(char *id);
  void sendMessage(char *id, void *msg, int msgSize);
  void* receiveMessage(char *id);
  int start_proc_user(char *procName, void *procPointer);
  void createMutex(char *id);
  void closeMutex(char *id);
  void lockMutex(char *id);
  void unlockMutex(char *id);
  void kill(int pid);

  /*
  ** turns the given integer into a string and stores it in
  ** the given array
  */
  char* intToString(int num,char * ret);

  int stringToInt(char *string);

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

   int getStdout();
   int getStdin();


#endif
