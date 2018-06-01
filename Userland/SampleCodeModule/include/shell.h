#ifndef SHELL_H
#define SHELL_H

  typedef enum {NONE, HELP, ECHO, EXIT} cmdID;

  void shell();
  void scroll();
  cmdID execute(char * cmd);
  void printHelpMsg();
#endif
