#include <stdint.h>
#include <naiveConsole.h>
#include <sysCalls.h>
#include <time.h>
#include <peripherals.h>
#include <bitMap.h>
#include <pipes.h>
#include "process.h"
#include "messageQueue.h"
#include "mutex.h"
#include "buddy.h"
#include "RoundRobin.h"
#include "wait.h"
#include <lib.h>

int sysRead(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysWrite(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysClearScreen(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysReadKeya(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysBeep(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysNoBeep(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysScreenInfo(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysDeleteChar(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysDrawPxl(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysScroll(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysDrawNum(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysNewProc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysPrintProc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysSendMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysReceiveMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysCreateMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysCloseMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysCreateMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysLockMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysUnlockMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysGetFds(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysTerminateMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysAlloc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysFree(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysPrintFreeMem(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysProcCascade(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysKill(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysWait(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysSwitchFd(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
int sysCreatePipe(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
void cleanUser(void);

typedef int (*sysFun)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

sysFun sysCalls[] = 
	{/*0*/sysRead, /*1*/sysWrite, /*2*/sysClearScreen, /*3*/sysReadKeya, /*4*/sysTime, /*5*/sysBeep, /*6*/sysNoBeep, /*7*/sysScreenInfo, 
	 /*8*/sysDeleteChar, /*9*/sysDrawPxl, /*10*/sysScroll, /*11*/sysDrawNum, /*12*/sysNewProc, /*13*/sysPrintProc, /*14*/sysSendMailbox, 
	 /*15*/sysReceiveMailbox, /*16*/sysCreateMailbox, /*17*/sysCloseMailbox, /*18*/sysCreateMutex, /*19*/sysLockMutex, /*20*/sysUnlockMutex, 
	 /*21*/sysGetFds, /*22*/sysTerminateMutex, /*23*/sysAlloc, /*24*/sysFree, /*25*/sysPrintFreeMem, /*26*/sysProcCascade, /*27*/sysKill, 
	 /*28*/sysWait, /*29*/sysSwitchFd, /*30*/sysCreatePipe, /*31*/sysDestroyPipe};

int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID){

	if(sysCallID < SYS_CALL_COUNT)
		return sysCalls[sysCallID - 1](arg1, arg2, arg3, arg4, arg5);
	return SYS_CALL_FAILURE;
}

int sysRead(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	int fd = arg1;
	char *buffer = (char*) arg2;
	int count = arg3;
	switch (fd){
		case KEY_BUF:
			readKeyboardBuffer(buffer, count);
			break;
		default:
			if(isPipe(fd))
				readFromPipe(buffer, fd, count);
	}
	return SYS_CALL_SUCCESS;
}

int sysWrite(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	int fd = arg1;
	char *buffer = (char*) arg2;
	int count = arg3;
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
		default:
			if(isPipe(fd))
				writeToPipe(buffer, count, fd);
	}
	return SYS_CALL_SUCCESS;
}

int sysClearScreen(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	ncClear();
	return SYS_CALL_SUCCESS;
}

int sysReadKeya(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){
	return readKeyboardBufferAll((char*)arg1);
}

int sysTime(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	int timeType = arg1;
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
	return SYS_CALL_FAILURE;
}

int sysBeep(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	beepASM(arg1);
	return SYS_CALL_SUCCESS;
}

int sysNoBeep(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	noBeepASM();
	return SYS_CALL_SUCCESS;
}

int sysScreenInfo(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

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
	return SYS_CALL_FAILURE;
}

int sysDeleteChar(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	ncDeleteChar();
	return 1;
}

int sysDrawPxl(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	Colour c;
	c.red = arg3;
	c.green = arg4;
	c.blue = arg5;
	drawPixelWithColour(arg1, arg2, c);
	return SYS_CALL_SUCCESS;
}

int sysScroll(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	ncScroll();
	return SYS_CALL_SUCCESS;
}

int sysDrawNum(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	Position p;
	//arg3: Color num, arg4: número a imprimir
	p.x = arg1;
	p.y = arg2;
	ncPrintNumberParser(arg4, arg3, &p);
	return SYS_CALL_SUCCESS;
}

int sysNewProc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return start_proc((char *) arg1, (void *) arg2, arg3, (char**) arg4);
}

int sysPrintProc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	print_proc();
	return SYS_CALL_SUCCESS;
}

int sysSendMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	send((char*)arg1, (const void *)arg2, arg3);
	return 0;
}

int sysReceiveMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return (uint64_t) receive((char *)arg1);
}

int sysCreateMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return createMailBox((char *)arg1);
}

int sysCloseMailbox(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	closeMailbox((char*)arg1);
	return SYS_CALL_SUCCESS;
}

int sysCreateMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return createMutex((char *)arg1, get_current_proc()->pid);
}

int sysLockMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return lock((char *)arg1, get_current_proc()->pid);
}

int sysUnlockMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return unlock((char *)arg1, get_current_proc()->pid);
}

int sysGetFds(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return getFd(get_current_proc(), arg1);
}

int sysTerminateMutex(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return terminateMutex((char *)arg1, get_current_proc()->pid);
}

int sysAlloc(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){
	return SYS_CALL_FAILURE;
}

int sysFree(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){
	return SYS_CALL_FAILURE;
}

int sysPrintFreeMem(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	print_free_memory();
	return SYS_CALL_SUCCESS;
}

int sysProcCascade(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	proc_cascade();
	return SYS_CALL_SUCCESS;
}

int sysKill(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	kill(arg1);
	return SYS_CALL_SUCCESS;	
}

int sysWait(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	wait(arg1);
    return SYS_CALL_SUCCESS;
}

int sysSwitchFd(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){
	
	switchFd(get_current_proc(), arg1, arg2);
	return SYS_CALL_SUCCESS;
}

int sysCreatePipe(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	return createPipe();
}

int sysDestroyPipe(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5){

	destroyPipe(arg1);
	return SYS_CALL_SUCCESS;
}
