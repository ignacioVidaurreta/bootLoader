#include <stdint.h>
#include <naiveConsole.h>
#include <sysCalls.h>
#include <time.h>
#include <peripherals.h>

void write(uint64_t fd, char* buffer, uint64_t count);
void read(uint64_t fd, char* buffer, uint64_t count);
void cleanUser(void);
int time(uint64_t timeType);

int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID){

	switch(sysCallID){
		case SYS_READ:
			read(arg1, (char*)arg2, arg3);
			return 1;
		case SYS_WRITE:
			write(arg1, (char*)arg2, arg3);
			return 1;
		case SYS_CLR_USR:
			cleanUser();
			return 1;
		case SYS_CLR_SCRN:
			ncClear();
			return 1;
		case SYS_READ_KEYA:
			return readKeyboardBufferAll((char*)arg1);
		case SYS_TIME:
			return time(arg1);
	}
	return -1;
}

int time(uint64_t timeType){
	switch(timeType){
		case TICKS:
			return ticksElapsed();
		case SECS:
			return getTimeFromRTC(0);
		case MINS:
			return getTimeFromRTC(2);
		case HRS:
			return getTimeFromRTC(4);
		case DAYS_WEEK:
			return getTimeFromRTC(6);
		case DAYS_MONTH:
			return getTimeFromRTC(7);
		case MONTHS:
			return getTimeFromRTC(8);
		case YRS:
			return getTimeFromRTC(9);
	}
	return -1;
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
			ncPrintUser(aux);
			break;
		case USR_ERR:
			ncPrintUserInColor(aux, RED);
			break;
	}
}

void read(uint64_t fd, char* buffer, uint64_t count){

	switch (fd){

		case KEY_BUF:
			readKeyboardBuffer(buffer, count);
			break;
	}
}

void cleanUser(){
	ncClearUser();
}