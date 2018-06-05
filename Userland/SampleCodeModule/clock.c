#include <date.h>
#include <stdlib.h>
#include <shell.h>
#include <clock.h>

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
    while(counter <= qtyRead && changed == 0){
      c=readInput();
      if ( c == 's'){
        beep();
        for(int i = 0; i<100; i++){
          for(int j = 0; j<100000;){
            j++;
          }
        }
        stopBeep();
        color++;
        if (color == 4){
          color =0;
        }
        changed = 1;
      }

      if (c == 'q'){
        flag=0;
      }
      counter++;
    }
    counter=0;
    changed = 0;
    int posHr = getScreenHeight()/3;
    posLim1 = writeNumwColor(posHr, posY, color, getHour());
    posMin = writeNumwColor(posLim1, posY, color, SEPARADOR);
    posLim2 = writeNumwColor(posMin, posY, color, getMinutes());
    posSec = writeNumwColor(posLim2, posY, color, SEPARADOR);
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
  if (num == SEPARADOR){
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

void beep(){
  int80(1000, 0, 0, 0, 0, 6);
}

void stopBeep(){
  int80(0, 0, 0, 0, 0, 7);
}

char readInput(){
  char c=0;
  int80(1, (uint64_t)&c, 1, 0, 0, 1);

  return c;
}
