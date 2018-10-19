#include <stdarg.h>
#include <stdlib.h>

void putChar(char c){
  int80(getStdout(),(uint64_t)&c, 1, 0,0, SYS_WRITE);
}


int strlen(const char * str){
  int i;
  for (i= 0; str[i] != 0; i++){
  }

  return i;

}

int strcmp(char* str1, char* str2){;
  int i=0;
  for (i = 0; str1[i] != 0; i++){
    if(str1[i] != str2[i]){
      return (str1[i]>str2[i])?1:-1;
    }
  }
  return str1[i] - str2[i];
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

int stringToInt(char *string){
  int ret = 0;
  int digits = 1;
  for(int i = strlen(string); i > 0; i--){
    if(isNum(string[i])){
      ret += digits*(string[i] - 48);
      digits *= 10;
    }
    else{
        if (string[i] != 0 ){
            return 1000;   //ya sé, re choto, por ahora el valor de error es 1000
        }
    }

  }
  return ret;
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
        putChar((char)va_arg(argsList, int));
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
	char line[BUFFER_SIZE];
  char* str;
  readBuffer(line);
	while(format[i]!=0){
		if(format[i]!='%'){
			if(format[i]==line[j]){ //Si fmt es igual a input lo ignoro
				i++;
				j++;
			}else{
                va_end(args);
                return count;
            }
		}
		else{
			i++; //leo el asterisco
			if(format[i]=='d'){
				num=va_arg(args,int*);
				*num=0;
				while(line[j]!=0 && isNum(line[j])){
					*num=(*num)*10+line[j]-'0';
					j++;
				}
			}
      else if(format[i]=='s'){
					str=va_arg(args,char*);
					while(line[j]!=0){
						*str=line[j];
						j++;
						str++;
					}
          *str=0;
			  }
        else if(format[i]=='c'){
					str=va_arg(args,char*);
					*str=line[j];
					j++;
				}
			count++;
			i++;
		}
  }
	va_end(args);
	return count;
}

char* readBuffer(char* read){
  char c;
  int n=0;
  while((c=readChar())!= '\n'){
    if (n == BUFFER_SIZE-1){
      read[n]=0;
      return read;
    }
    if( c == '\b'){
      if(n !=0){
        n--;
        deleteCharacter();
      }
    }else{
      read[n++]=c;
      putChar(c);
    }

    }
  read[n]=0;
  return read;
}

char readChar(){
  char c=0;
  while (c == 0){
    int80(getStdin(), (uint64_t)&c, 1, 0, 0, SYS_READ);
  }

  return c;
}

int isNum(char c){
  return (c>='0' && c<='9') || c=='\n';
}

void deleteCharacter(){
  int80(0,0,0,0,0,SYS_DEL_CHAR);
}


char* strncpy(char *destination, const char *source, int n){
  char* aux;
  aux = destination;
  for (; n>0 && *source!=0; n--){
    *destination = *source;
    destination++;
    source++;
  }
  *destination='\0';
  return aux;
}

int strncmp(const char* str1, const char* str2, int n){
  int i;
  for (i = 0; n>0; i++, n--){
    if(str1[i] != str2[i]){
      return (str1[i]>str2[i])?1:-1;
    }
  }
  return 0;
}

void createMessageQueue(char *id){
  int80((uint64_t)id, 0, 0, 0, 0, CREATE_MAILBOX);
}

void closeMessageQueue(char *id){
  int80((uint64_t)id, 0, 0, 0, 0, DESTROY_MAILBOX);
}

void sendMessage(char *id, void *msg, int msgSize){
  int80((uint64_t)id, (uint64_t)msg, (uint64_t)msgSize, 0, 0, SEND_MAILBOX);
}

void* receiveMessage(char *id){
  return (void*) int80((uint64_t)id, 0, 0, 0, 0, RECEIVE_MAILBOX);
}

int start_proc_user(char *procName, void *procPointer, int argc, char *argv[], uint64_t priority) {
    return int80((uint64_t) procName, (uint64_t) procPointer, (uint64_t) argc, (uint64_t) argv, (uint64_t) priority, SYS_NEW_PROC);
}

void kill(int pid){
  int80(pid, 0, 0, 0, 0, SYS_KILL);
}

void createMutex(char *mutexId){
  int80((uint64_t) mutexId, 0, 0, 0, 0, CREATE_MUTEX);
}

void lockMutex(char *mutexId){
  int80((uint64_t) mutexId, 0, 0, 0, 0, LOCK_MUTEX);
}

void unlockMutex(char *mutexId){
  int80((uint64_t) mutexId, 0, 0, 0, 0, UNLOCK_MUTEX);
}

void closeMutex(char *mutexId){
  int80((uint64_t) mutexId, 0, 0, 0, 0, TERMINATE_MUTEX);
}

int getStdout(){
  return int80(STDOUT, 0, 0, 0, 0, SYS_FDS);
}

int getStdin(){
  return int80(STDIN, 0, 0, 0, 0, SYS_FDS);
}

int *createPipe(){

  return (int*) int80(0, 0, 0, 0, 0, SYS_CREATE_PIPE);
}

void destroyPipe(int fd){
  
  int80(fd, 0, 0, 0, 0, SYS_DESTROY_PIPE);
}

void writeToFd(int fd, char *buffer, int size){

  int80(fd, (uint64_t) buffer, size, 0, 0, SYS_WRITE);
}

void readFromFd(int fd, char *buffer, int size){

  int80(fd, (uint64_t) buffer, size, 0, 0, SYS_READ);
}

void switchFd(int fdType, int newFd){

  int80(fdType, newFd, 0, 0, 0, SYS_SWITCH_FD);
}

int *malloc(uint64_t size){

  return (int*) int80(size, 0, 0, 0, 0, ALLOCATE_MEMORY);
}

void wait(uint64_t pid) {
  int80(pid, 0, 0, 0, 0, SYS_WAIT);
}

int *joinByPipe(char *readerProcName, void *readerProcPointer, char *writerProcName, void *writerProcPointer){

  int* pipe = createPipe();
  switchFd(STDOUT, pipe[1]);
  int writerPid = start_proc_user(writerProcName, writerProcPointer, 0, 0, 0);
  switchFd(STDOUT, STDOUT);
  switchFd(STDIN, pipe[0]);
  int readerPid = start_proc_user(readerProcName, readerProcPointer, 0, 0, 0);
  switchFd(STDIN, STDIN);
  int* pids = malloc(2*sizeof(int));
  pids[1] = writerPid;
  pids[0] = readerPid;
  return pids;
}
