#ifndef SHELL_H
#define SHELL_H

  typedef enum {NONE, HELP, ECHO, DATE, CLOCK, DIV, INVOPC, CLEAR, EXIT, PS, PRINT_MEM, PROC_CASCADE, PRODCONS, END_PRODCONS} cmdID;

  void shell();
  void start_proc_user(char *name, void *function);
  void scroll();
  cmdID execute(char * cmd);
  void printHelpMsg();
  void echo(char*arg);
  void clear();
  void invOpcodeException();
  void zeroDivException();
  void print_process();


#endif
