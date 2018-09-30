#include <stdint.h>
#include <naiveConsole.h>
#include <sysCalls.h>
#include <time.h>
#include <peripherals.h>
#include <bitMap.h>
#include "process.h"
#include "messageQueue.h"
#include "mutex.h"
#include "buddy.h"
#include "RoundRobin.h"
#include "wait.h"

void write(uint64_t fd, char* buffer, uint64_t count);
void read(uint64_t fd, char* buffer, uint64_t count);
void cleanUser(void);
int time(uint64_t timeType);
int screenInfo(uint8_t arg1);


int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID){

	Colour c;
	Position p;
	switch(sysCallID){
		case SYS_READ:
			read(arg1, (char*)arg2, arg3);
			return 1;
		case SYS_WRITE:
			write(arg1, (char*)arg2, arg3);
			return 1;
		case SYS_CLR_SCRN:
			ncClear();
			return 1;
		case SYS_READ_KEYA:
			return readKeyboardBufferAll((char*)arg1);
		case SYS_TIME:
			return time(arg1);
		case SYS_BEEP:
			beepASM(arg1);
			return 1;
		case SYS_NO_BEEP:
			noBeepASM();
			return 1;
		case SYS_SCRN_INFO:
			return screenInfo(arg1);
		case SYS_DEL_CHAR:
			ncDeleteChar();
			return 1;
		case SYS_DRAW_PXL:
			c.red = arg3;
			c.green = arg4;
			c.blue = arg5;
			drawPixelWithColour(arg1, arg2, c);
			return 1;
		case SYS_SCRL:
			ncScroll();
			return 1;
		case DRAW_NUM:
      		//arg3: Color num, arg4: número a imprimir
			p.x = arg1;
			p.y = arg2;
			ncPrintNumberParser(arg4, arg3, &p);
			return 1;
		case SYS_NEW_PROC:
			ncPrint("Created");
			int ret = start_proc((char *) arg1, (void *) arg2); //Name and pointer to the function
			ncScroll();
			return ret;
		case SYS_PRINT_PROC:
			print_proc();
			return 1;
		case SEND_MAILBOX:
			send((const char*)arg1, (const void *)arg2, arg3);
			return 0;
		case RECEIVE_MAILBOX:
			return (uint64_t)receive((char *)arg1);
		case CREATE_MAILBOX:
			return createMailBox((char *)arg1);
		case DESTROY_MAILBOX:
			closeMailbox((char*)arg1);
			return 0;
		case CREATE_MUTEX:
			return createMutex((char *)arg1, get_current_proc()->pid);
		case LOCK_MUTEX:
			return lock((char *)arg1, get_current_proc()->pid);
		case UNLOCK_MUTEX:
			return unlock((char *)arg1, get_current_proc()->pid);
		case TERMINATE_MUTEX:
			return terminateMutex((char *)arg1, get_current_proc()->pid);
		case PRINT_MEM:
			print_free_memory();
			return 1;
		case PROC_CASCADE:
			proc_cascade();
			return 1;
		case SYS_KILL:
			kill(arg1);
			return 1;
        case SYS_WAIT:
            wait(arg1);
            return 1;
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
	Colour error = {.red = 0xFF, .green = 0, .blue = 0};
	for(i = 0; i < count; i++)
		aux[i] = buffer[i];
	aux[i] = 0;
	switch (fd){

		case STD_OUT:
			ncPrint(aux);
			break;
		case STD_ERR:
			ncPrintInColor(aux, error);
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

int screenInfo(uint8_t arg1){
	switch(arg1){
		case HEIGHT:
			return height();
		case WIDTH:
			return width();
		case CHEIGHT:
			return cheight();
		case CWIDTH:
			return cwidth();
	}
	return -1;
}
