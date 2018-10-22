#include <stdlib.h>
#include <shell.h>
#include <date.h>
#include <clock.h>
#include <prodcons.h>
#include <philo.h>

char arg[BUFFER_SIZE - 5];
int prodconsPid = 0;


void shell(){
  int endFlag = 0;
  cmdID commandID= NONE;
  while(!endFlag){
    char command[BUFFER_SIZE]={0};
    uint64_t pid;
    printf("master> "); //prompts
    scanf("%s", command);
    scroll();
    commandID = execute(command);
    switch(commandID){
      case HELP:
        pid = start_proc_user("help", (void *) printHelpMsg, 0, 0, 0);
        wait(pid);
        break;
      case EXIT:
        endFlag=1;
        break;
      case DATE:
        pid = start_proc_user("getDate", (void *) getDate, 0, 0, 0);
        wait(pid);
        break;
      case CLOCK:
        pid = start_proc_user("start clock", (void *) startClock, 0, 0, 0);
        wait(pid);
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
        pid = start_proc_user("clear", (void *) clear, 0, 0, 0);
        wait(pid);
        break;
      case PS:
        pid = start_proc_user("ps", (void *) print_process, 0, 0, 0);
        wait(pid);
        break;
      case PRINT_MEM:
        int80(0, 0, 0, 0, 0, SYS_PRINT_MEM);
        break;
      case PROC_CASCADE:
        int80(0, 0, 0, 0, 0, SYS_PROC_CASCADE);
        break;
      case PRODCONS:
        if(prodconsPid == 0)
          prodconsPid = start_proc_user("prodcons", (void*) prodcons, 0, 0, 0);
        break;
      case END_PRODCONS:
        if(prodconsPid != 0)
          sendMessage(CREATION_MSG_QUEUE_ID, "exit", strlen("exit"));
        break;
      case ADD_READERS:
        printf("Reader Added");
        break;
      case ADD_WRITERS:
        printf("Writer Added");
        break;
      case PIPE_EXAMPLE:
        pid = start_proc_user("pipes", (void*) pipeExample, 0, 0, 0);
        wait(pid);
        scroll();
        break;
      case PHIL:
        pid = start_proc_user("philosophers", (void*) start_philosophers, 0, 0, 0);
        wait(pid);
        break;
      default:
        printf("'%s' is not a valid command. Please try again. Write help to get a list of the possible commands", command);
        scroll();
    }
  }
}

void scroll(){
  int80(0,0,0,0,0,SYS_SCRL);
}

cmdID execute(char * cmd){
  char* aux;
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
  }else if(strcmp(cmd, "clear")== 0){
      return CLEAR;
  }else if(strcmp(cmd, "ps") == 0){
      return PS;
  }else if(strcmp(cmd, "freeMem") == 0){
      return PRINT_MEM;
  }else if(strcmp(cmd, "procCascade") == 0){
      return PROC_CASCADE;
  }
  else if(strcmp(cmd, "prodcons") == 0){
      return PRODCONS;
  }
  else if(strcmp(cmd, "endProdcons") == 0){
    return END_PRODCONS;
  }
  else if(strcmp(cmd, "pipeItUp") == 0){
    return PIPE_EXAMPLE;
  }else if(strcmp(cmd, "philosophers") == 0){
      return PHIL;
  }else{
    aux="echo";
    if(strncmp(aux, cmd, 4) == 0){
      int len = strlen(cmd);
      if (len > BUFFER_SIZE){
        strncpy(arg, &cmd[5],BUFFER_SIZE-5);
      }else{
        strncpy(arg, &cmd[5],len-5);
      }

      return ECHO;
    }
    aux = "addReaders";
    if(strncmp(aux, cmd, 10) == 0){
      int readers = stringToInt(&cmd[10]);
      if(prodconsPid != 0 && readers < 1000){
        sendMessage(CREATION_MSG_QUEUE_ID, "reader", strlen("reader"));
        sendMessage(CREATION_MSG_QUEUE_ID, (void*)(&readers), sizeof(int));
        return ADD_READERS;
      }
    }
    aux = "addWriters";
    if(strncmp(aux, cmd, 10) == 0){
      int writers = stringToInt(&cmd[10]);
      if(prodconsPid != 0 && writers < 1000){
        sendMessage(CREATION_MSG_QUEUE_ID, "writer", strlen("writer"));
        sendMessage(CREATION_MSG_QUEUE_ID, (void*)(&writers), sizeof(int));
        return ADD_WRITERS;
      }
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
    printf("* freeMem: Prints free memory blocks and sizes");
    scroll();
    printf("* procCascade: Adds 100 processes to Ready queue and then frees them");
    scroll();
    printf("* ![UNDER CONSTRUCTION]! prodcons: starts the demonstration of the prodcons problem");
    scroll();
    printf("* ![UNDER CONSTRUCTION]! addWriters: adds writers to the prodcons demonstration (a negative number will remove writers)");
    scroll();
    printf("* ![UNDER CONSTRUCTION]! addReaders: adds readers to the prodcons demonstration (a negative number will remove readers)");
    scroll();
    printf("* ![UNDER CONSTRUCTION]! endProdcons: ends the prodcons demonstration, the ammount of messages read by the readers will be printed");
    scroll();
    printf("* pipeItUp: Simple program showing the work of pipes");
    scroll();
    printf("* philosophers: Run the Philosophers simulation");
    scroll();
}

void echo(char*arg){
  printf("%s\n", arg);
  scroll();
}

void clear(){
  int80(0,0,0,0,0,SYS_CLR_SCRN);
}

void print_process(){
  int80(0, 0, 0, 0, 0, SYS_PRINT_PROC); //Prints processes
}

void pipeReader(){

  char text[80] = {0};
  scanf("%s", text);
  printf("Pipe read: %s\n", text);
}

void pipeExample(){

  //joinByPipe("pipeReader", (void*) pipeReader, "getDate", (void*) getDate);
  joinByPipe("getDate", (void*) getDate, "pipeReader", (void*) pipeReader);
}

void start_philosophers(){
  testPhilosophers();
  scroll();
}
