#include <stdarg.h>
extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

int strlen(const char * str){
  int i;
  for (i= 0; str[i] != 0; i++){
  }

  return i;

}

void swap(char* numStr, int index1, int index2){
  char aux = numStr[index1];
  numStr[index1] = numStr[index2];
  numStr[index2] = aux;
}

char * intToString(int num, char * numStr){
  int i =0, j;
  if (num != 0){
    if (num< 0){
      numStr[i++] = '-';
    }
    for(;num >0; i++){
        numStr[i]=num%10 + '0';
        num = num/10;
    }
    for (j =0; j< i/2; j++){ //No es necesario recorrerlo completo
      swap(numStr, j, i-j-1);
    }
    numStr[i] = 0;
  }else{
    numStr[0] = '0';
  }

  return numStr;
}

/*
 *  https://bobobobo.wordpress.com/2008/01/28/how-to-use-variable-argument-lists-va_list/
 *  Imprime el string dado cambiando el:
 *  - %d por un integer pasado por parámetro
 *  - %s con un string pasado por parámetro
 *  - %c caracter pasado por parámetro
 */
void printf(const char * str, ...){
  char num[12];
  va_list argsList;
  va_start(argsList, str);
  int length = strlen(str);
  // State = 0 -> Caracter normal
  // state = 1 -> caracter con %
  int state = 0;
  for (int i = 0; i< length; i++){
    if (state == 0){
      if (str[i] == '%'){
        state = 1;
      }else{
          //imprimir letra
      }
    }else{
      if (str[i] == 'd'){
        intToString(va_arg(argsList, int), num);
        //imprimir
        state=0;
      }else if(str[i] == 's'){
        //imprimir va_arg(argsList, char*);
        state = 0;
      }else if(str[i] == 'c'){
        //imprimir
        state = 0;
      }else{
        //imprimir %c y el caracter
        state = 0;
      }
    }
  }
  va_end(argsList);

}
