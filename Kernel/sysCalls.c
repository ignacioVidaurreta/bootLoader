#include <stdint.h>
#include <naiveConsole.h>
#include <sysCalls.h>
//#include <keyboard.h>  si se descomenta esto, kaput.

void write(uint64_t fd, char* buffer, uint64_t count);
void read(uint64_t fd, char* buffer, uint64_t count);

void int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID){

	switch (sysCallID){

		case SYS_WRITE:
			write(arg1, (char*)arg2, arg3);
			break;
		case SYS_READ:
			read(arg1, (char*)arg2, arg3);
			break;
	}
}

void write(uint64_t fd, char* buffer, uint64_t count){
	
	char aux[count + 1];
	int i;
	for(i = 0; i < count; i++)
		aux[i] = buffer[i];
	aux[i] = 0;
	switch (fd){

		case STD_OUT:
			ncPrint(aux);
			break;
		case USR_OUT:
			ncPrintUserLine(aux);
			break;
		case USR_ERR:
			ncPrintUserLineInColor(aux, RED);
	}
}

void read(uint64_t fd, char* buffer, uint64_t count){

	switch (fd){

		case 0:
			for(int i = 0; i < count; i++)
				//buffer[i] = keyBuffer[i];
			break;
	}
}