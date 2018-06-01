#include <stdlib.h>
#include <shell.h>

void shell(){
  int endFlag = 0;
  while(!endFlag){
    char command[BUFFER_SIZE]={0};
    printf("$> "); //prompt
    scanf("%s", command);

    scroll();
  }
}

void scroll(){
  int80(0,0,0,0,0,11);
}
