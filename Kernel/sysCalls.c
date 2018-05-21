#include <stdint.h>
#include <naiveConsole.h>
#include <sysCalls.h>

void write(uint64_t fd, char* buffer, uint64_t count);

void int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID){

	switch (sysCallID){

		case SYS_WRITE:
			write(arg1, (char*)arg2, arg3);
	}
}

void write(uint64_t fd, char* buffer, uint64_t count){

	ncPrintDec(count);
	ncNewline();
	ncPrintDec((uint64_t) buffer);
	ncNewline();
	char aux[count + 1];
	int i;
	for(i = 0; i < 12; i++)
		aux[i] = buffer[i];
	aux[i] = 0;
	switch (fd){

		case 1:
			ncPrint(aux);
			break;
		case 2:
			ncPrintInColor(aux, RED);
			break;
	}
}