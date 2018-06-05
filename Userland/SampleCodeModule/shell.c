#include <stdlib.h>
#include <shell.h>
#include <date.h>
char arg[BUFFER_SIZE - 5];

void shell(){
  int endFlag = 0;
  int exception;
  cmdID commandID= NONE;
  while(!endFlag){
    char command[BUFFER_SIZE]={0};
    /*
     * Opciones:
     *    - snmOS>
     *    - $
     *    - $>
     *    - root>
     */
    printf("snmOS> "); //prompt
    //putChar(' ');
    scanf("%s", command);
    scroll();
    commandID = execute(command);
    switch(commandID){
      case HELP:
        printHelpMsg();
        break;
      case EXIT:
        endFlag=1;
        break;
      case DATE:
        getDate();
        break;
      case CLOCK:
        startClock();
        break;
      case DIV:
        exception = 10/0;
        scroll();
        break;
      case ECHO:
        echo(arg);
        break;
      case CLEAR:
        clear();
        break;
      default :
        printf("Invalid command: Please try again");
        scroll();
    }
  }
}

void scroll(){
  int80(0,0,0,0,0,11);
}

cmdID execute(char * cmd){
  if(strcmp(cmd, " ")== 0){
    return NONE;
  }
  if(strcmp(cmd, "help ")== 0){
    return HELP;
  }else if(strcmp(cmd, "exit ") == 0){
    printf("See you next time!");
    return EXIT;
  }else if (strcmp(cmd, "date ") == 0){
    return DATE;
  }else if (strcmp(cmd, "clock ")== 0){
    return CLOCK;
  }else if(strcmp(cmd, "divByZero ") == 0){
    return DIV;
  }
  else if(strcmp(cmd, "clear ")==0){
    return CLEAR;
  }
  else{
    char* aux="echo ";
    if(strncmp(aux, cmd, 5) == 0){
      strncpy(arg, &cmd[5],strlen(cmd)-5);
      return ECHO;
    }
  }
  return NONE;
}

/*
 *  Habría que lograr que el \n salte a la siguiente linea así no tengo que
 * llamar a scroll cada vez.
 */
void printHelpMsg(){
  printf("Thank you for using snmOS, you can try using the following commands:");
  scroll();
  printf("* exit: Exit the OS");
  scroll();
  printf("* echo [msg]: Print [msg] in the shell");
  scroll();
  printf("* help: Prints this help message");
  scroll();
  printf("* date: Prints actual date and time ");
  scroll();
  printf("* clock: Opens the clock application");
  scroll();
  printf("* clear: clears the screen");
  scroll();
  printf("* divByZero: Generates a division by zero exception");
  scroll();
}

void echo(char*arg){
  printf("%s\n", arg);
  scroll();
}

void clear(){
  int80(0,0,0,0,0,3);
}