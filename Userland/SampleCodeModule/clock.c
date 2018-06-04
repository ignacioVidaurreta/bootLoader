#include <clock.h>
#include <date.h>
#include <stdlib.h>
#include <shell.h>

/*
 *  Blanco : 0
 *  Rojo   : 1
 *  Verde  : 2
 *  Azul   : 3
 */

void startClock(){
  clearScreen();
  char c;
  int color = 0;
  int counter=0;
  int posY = getScreenWidth()/2;
  int posLim1, posLim2, posMin, posSec;
  int changed = 0;
  printf("Press Q to exit or S to switch the color of the clock");
  scroll();
  int qtyRead=60;
  int flag=1;
  while(flag){
    //char * str;
    while(counter <= qtyRead && changed == 0){
      if ( c=readyChar() == 's'){
        beep();
        color++;
        if (color == 4){
          color =0;
        }
        changed = 1;
      }
      if (c=readyChar() == 'q'){
        flag=0;
      }
      counter++;
    }
    counter=0;
    changed = 0;
    int posHr = getScreenHeight()/3;
    posLim1 = writeNumwColor(posHr, posY, color, getHour());
    posMin = writeNumwColor(posLim1, posY, color, ':');
    posLim2 = writeNumwColor(posMin, posY, color, getMinutes());
    posSec = writeNumwColor(posLim2, posY, color, ':');
    writeNumwColor(posSec, posY, color, getSeconds());
  }
}


int getScreenHeight(){
  return int80(2, 0, 0, 0, 0, 8);
}

int getScreenWidth(){
  return int80(1, 0, 0, 0, 0, 8);
}

int writeNumwColor(int x, int y, int color, int num){
  if (num == ':'){
    int80(x,y, color, 10, 0, 12); //imprime el separador
    return x + (5*8);
  }
  if (num >= 10){
    int aux = num/10;
    int80(x,y,color,aux,0, 12);
    int80(x+5*8, y, color, num%10, 0, 12);
    return x+(5*8)*2;
  }else{
    int80(x,y,color,0,0,12);
    int80(x + 5*8,y,color,num,0, 12);
    return x+ (5*8)*2;
  }
}

void clearScreen(){
  int80(0,0,0,0,0,3);
}

int readInput(char* str){
  int num;
  num = int80((uint64_t)str,1 , 1, 0, 0, 4);
  return num;

}

void beep(){
  int80(1000, 0, 0, 0, 0, 6);
}

char readyChar(){
  char c=0;
  int80(1, (uint64_t)&c, 1, 0, 0, 1);

  return c;
}
