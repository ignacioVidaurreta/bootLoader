#ifndef SHELL_H
#define SHELL_H

  typedef enum {NONE, HELP, ECHO, DATE, CLOCK, DIV, INVOPC, CLEAR, EXIT, PS, PRINT_MEM, 
                PROC_CASCADE, PRODCONS, END_PRODCONS, ADD_READERS, ADD_WRITERS, PIPE_EXAMPLE,
                TEST} cmdID;
#include <stdint.h>

  void shell();
  void wait(uint64_t pid);
  void scroll();
  cmdID execute(char * cmd);
  void printHelpMsg();
  void echo(char*arg);
  void clear();
  void invOpcodeException();
  void zeroDivException();
  void print_process();
  void pipeExample();


#endif
