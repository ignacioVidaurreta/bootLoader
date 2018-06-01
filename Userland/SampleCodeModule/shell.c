#include <stdlib.h>
#include <shell.h>

void shell(){
  int endFlag = 0;
  cmdID commandID= NONE;
  while(!endFlag){
    char command[BUFFER_SIZE]={0};
    putChar('$'); //prompt
    putChar(' ');
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
  printf("* time:"); //falta implementar
  scroll();
  printf("* clock: "); // falta implementar
  scroll();
}
