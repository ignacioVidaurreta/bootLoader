#ifndef _STDLIB_H
#define _STDLIB_H

  #include <stdint.h>
  #define STDIN 0
  #define STDOUT 1
  #define BUFFER_SIZE 80
  #define SYS_READ 1
  #define SYS_WRITE 2
  #define SYS_CLR_SCRN 3
  #define SYS_READ_KEYA 4
  #define SYS_TIME 5
  #define SYS_BEEP 6
  #define SYS_NO_BEEP 7
  #define SYS_SCRN_INFO 8
  #define SYS_DEL_CHAR 9
  #define SYS_DRAW_PXL 10
  #define SYS_SCRL 11
  #define SYS_DRAW_NUM 12
  #define SYS_NEW_PROC 13
  #define SYS_PRINT_PROC 14
  #define SEND_MAILBOX  15
  #define RECEIVE_MAILBOX 16
  #define CREATE_MAILBOX  17
  #define DESTROY_MAILBOX 18
  #define CREATE_MUTEX  19
  #define LOCK_MUTEX  20
  #define UNLOCK_MUTEX  21
  #define SYS_FDS 22
  #define TERMINATE_MUTEX 23
  #define ALLOCATE_MEMORY 24
  #define FREE 25
  #define SYS_PRINT_MEM 26
  #define SYS_PROC_CASCADE 27
  #define SYS_KILL 28
  #define SYS_WAIT 29
  #define SYS_SWITCH_FD 30
  #define SYS_CREATE_PIPE 31
  #define SYS_DESTROY_PIPE 32

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
  int *createPipe();
  void destroyPipe(int fd);
  void writeToFd(int fd, char *buffer, int size);
  void readFromFd(int fd, char *buffer, int size);
  int getStdout();
  int getStdin();
  void switchFd(int fdType, int newFd);

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

   void wait(uint64_t pid);

   int *joinByPipe(char *procName1, void *procPointer1, char *procName2, void *procPointer2, int *readerPid, int *writerPid);

#endif
