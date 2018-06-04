#include <stdlib.h>
#include <shell.h>
#include <date.h>
char arg[BUFFER_SIZE - 5];

void shell(){
  int endFlag = 0;
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
        printf("No tiro excepción");
        int j = 10/0;
        break;
      case ECHO:
        echo(arg);
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
  /*
   * NO TENGO NI IDEA POR QUÉ NO FUNCIONA SIN EL ESPACIO AL FINAL
   * TODO arreglar
   */
  if(strcmp(cmd, "help ")==0){
    return HELP;
  }else if(strcmp(cmd, "exit ") == 0){
    printf("See you next time!");
    return EXIT;
  }else if (strcmp(cmd, "date ") == 0){
    return DATE;
  }else if (strcmp(cmd, "clock ")== 0){
    return CLOCK;
  }else if(strcmp(cmd, "div ") == 0){
    int i = 3/0;
    return DIV;
  }else{
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
  printf("* echo [msg]: Print [msg] in the shell"); //falta implementar
  scroll();
  printf("* help: Prints this help message");
  scroll();
  printf("* date: Prints actual date and time ");
  scroll();
  printf("* clock: "); // falta implementar
  scroll();
}

void echo(char*arg){
  printf("%s\n", arg);
  scroll();
}
