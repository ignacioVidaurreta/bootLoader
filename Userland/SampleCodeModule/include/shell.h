#ifndef SHELL_H
#define SHELL_H

  typedef enum {NONE, HELP, ECHO, DATE, CLOCK, DIV, INVOPC, CLEAR, EXIT, PS, TEST, PRINT_MEM, PROC_CASCADE} cmdID;

  void shell();
  void scroll();
  cmdID execute(char * cmd);
  void printHelpMsg();
  void echo(char*arg);
  void clear();
  void invOpcodeException();
  void zeroDivException();
  void print_process();


#endif
