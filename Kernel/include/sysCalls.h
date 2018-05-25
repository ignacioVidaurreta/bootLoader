#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <stdint.h>
#include <keyboard.h>

#define SYS_READ 1
#define SYS_WRITE 2
#define SYS_CLR_USR 3		//fills the user command line with blank spaces.
#define SYS_CLR_SCRN 4		//fills all the screen, except for the command line with blank spaces.
#define SYS_READ_KEYA 5		//reads the totality of the keyboard buffer. Returns in rax the total ammount of characters read.
#define SYS_TIME 6 			//returns data related to time. Information expanded in the arguements section

//defined file descriptors for write, all of them received as arg1.
//file descriptors: 1 standard output.
//					2 user output, this represents the last line, into which the user inputs commands.
//					3 user error. The command line will display a red message.
#define STD_OUT 1
#define USR_OUT 2
#define USR_ERR 3

//defined file descriptors for read, all of them received as arg1.
//file descriptors: 1 keyboard buffer as defined in keyboard.c.
#define KEY_BUF 1

//possible arguement for calling the time sysCall, all of them received as arg1
//please be aware that the information is returned in greenwich mean time, which
//is 3 hours ahead of Argentina.
#define TICKS 1			//returns in rax the ammount of ticks since system start.
#define SECS 2			//returns seconds since the current minute began.
#define MINS 3			//returns the minutes since the current hour began.
#define HRS	4			//returns the hours since the current day began.
#define DAYS_WEEK 5		//returns the days since the current week began.
#define DAYS_MONTH 6	//returns the days since the current month began.
#define MONTHS 7		//returns the months since the current year began.
#define YRS 8			//returns the years since the current birth of christ (I ain't rewriting this for the second coming).

//the following arguements are received in order according to C 64 bit calling convention.
//So, the arguements should be inserted in the order: arg1 = rdi, arg2 = rsi and so on for rdx, r10, r8
//except for the sysCallID which should be inserted into the rax register.
//returns -1 in rax as a failure sign.
int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID);

#endif