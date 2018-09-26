#include <stdlib.h>
#include <shell.h>
#include <date.h>
#include <clock.h>

char arg[BUFFER_SIZE - 5];

void shell(){
  int endFlag = 0;
  cmdID commandID= NONE;
  while(!endFlag){
    char command[BUFFER_SIZE]={0};
    printf("master> "); //prompts
    scanf("%s", command);
    scroll();
    commandID = execute(command);
    switch(commandID){
      case HELP:
        start_proc_user("help", (void *) printHelpMsg);
        break;
      case EXIT:
        endFlag=1;
        break;
      case DATE:
        start_proc_user("getDate", (void *) getDate);
        break;
      case CLOCK:
        start_proc_user("start clock", (void *) startClock);
        break;
      case DIV:
        zeroDivException();
        break;
      case INVOPC:
        invOpcodeException();
        break;
      case ECHO:
        echo(arg);
        break;
      case CLEAR:
        start_proc_user("clear", (void *) clear);
        break;
      case PS:
        start_proc_user("ps", (void *) print_process);
        break;
      case TEST:
        int80(0, 0, 0, 0, 0, 26);
        break;
      case PRINT_MEM:
        int80(0, 0, 0, 0, 0, 27);
        break;
      case PROC_CASCADE:
        int80(0, 0, 0, 0, 0, 28);
        break;
      default :
        printf("Invalid command: Please try again. Write help to get a list of the possible commands");
        scroll();
    }
  }
}
void start_proc_user(char *name, void *function) {
    int80((uint64_t) name, (uint64_t) function, 0, 0, 0, 13);
}

void scroll(){
  int80(0,0,0,0,0,11);
}

cmdID execute(char * cmd){

  if(strcmp(cmd, "help")== 0){
      return HELP;
  }else if(strcmp(cmd, "exit") == 0){
      printf("Thank you, come again ;)");
      return EXIT;
  }else if (strcmp(cmd, "date") == 0){
      return DATE;
  }else if (strcmp(cmd, "clock")== 0){
      return CLOCK;
  }else if(strcmp(cmd, "divByZero") == 0){
      return DIV;
  }else if(strcmp(cmd, "invOpcode") == 0){
      return INVOPC;
  }
  else if(strcmp(cmd, "clear")== 0){
      return CLEAR;
  }else if(strcmp(cmd, "ps") == 0){
      return PS;
  }else if(strcmp(cmd, "test") == 0){
      return TEST;
  }else if(strcmp(cmd, "freeMem") == 0){
      return PRINT_MEM;
  }else if(strcmp(cmd, "procCascade") == 0){
      return PROC_CASCADE;
  }else{
    char* aux="echo";
    if(strncmp(aux, cmd, 4) == 0){
      int len = strlen(cmd);
      if (len > BUFFER_SIZE){
        strncpy(arg, &cmd[5],BUFFER_SIZE-5);
      }else{
        strncpy(arg, &cmd[5],len-5);
      }

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
    printf("* invOpcode: Generates an invalid opcode exception");
    scroll();
    printf("* ps: Prints all current processes (UNDER CONSTRUCTION)");
    scroll();
    printf("* test: Runs all the test suites");
    scroll();
    printf("* freeMem: Prints free memory blocks and sizes");
    scroll();
    printf("* procCascade: Adds 100 processes to Ready queue and then frees them");
    scroll();
}

void echo(char*arg){
  printf("%s\n", arg);
  scroll();
}

void clear(){
  int80(0,0,0,0,0,3);
}
void print_process(){
    int80(0, 0, 0, 0, 0, 14); //Prints processes
}
