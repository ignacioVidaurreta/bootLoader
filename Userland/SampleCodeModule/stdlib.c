#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void putChar(char c){
  _int80(1,&c, 1, 0,0,2);
}


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
          putChar(str[i]);
      }
    }else{
      if (str[i] == 'd'){
        intToString(va_arg(argsList, int), num);
        for (int index=0; num[index]!=0; index++){
          putChar(num[index]);
        }
        state=0;
      }else if(str[i] == 's'){
        /*
         *  printf("%s", "Hola") es igual a hace printf("Hola"):
        */
        printf(va_arg(argsList, char*));
        state = 0;
      }else if(str[i] == 'c'){
        putChar(va_arg(argsList, char));
        state = 0;
      }else{
        putChar('%');
        putChar(str[i]);
        state = 0;
      }
    }
  }
  va_end(argsList);

}

char* strcpy(char* dest, char* src){
  if (src == 0){
    return 0;
  }
  int i=0;
  for (; src[i] != 0; i++){
    dest[i] = src[i];
  }
  dest[i] = 0;
  return dest;
}

int scanf(const char* format, ...){
	va_list args;
	va_start (args, format);
	int count=0;
	int i=0;
	int j=0;
	int* num;
	char* line;
  char* str;
  readBuffer(line);
	while(format[i]!=0){
		if(format[i]!='%'){
			if(format[i]==line[j]){ //Si fmt es igual a input lo ignoro
				i++;
				j++;
			}else{
        return count;
      }
		}
		else{
			if(format[j]=='%'){ //TODO SACARLO DESPUÉS
				i++;
				j++;
				if(format[i]=='d'){
					num=va_arg(args,int*);
					*num=0;
					while(line[j]!=0 && isNum(line[j])){ //TODO hacer isNum
						*num=(*num)*10+line[j]-'0';
						j++;
					}
				}
        else if(format[i]=='s'){
  					str=va_arg(args,char*);
  					while(line[j]!=0){
  						*str=line[j];
  						j++;
  						*str++;
  					}
				  }
          else if(format[i]=='c'){
  					str=va_arg(args,char*);
  					*str=line[j];
  					j++;
  				}
				count++;
			}
			i++;
		}
  }
	va_end(args);
	return count;
}

int readBuffer(char * read){
  return _int80(1, read, 0, 0, 0, 4);
}

int isNum(char c){
  return (c>='0' && c<='9');
}
