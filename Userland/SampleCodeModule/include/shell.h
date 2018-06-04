#ifndef SHELL_H
#define SHELL_H

  typedef enum {NONE, HELP, ECHO, DATE, CLOCK, DIV, CLEAR, EXIT} cmdID;

  void shell();
  void scroll();
  cmdID execute(char * cmd);
  void printHelpMsg();
  void echo(char*arg);
  void clear();

#endif
