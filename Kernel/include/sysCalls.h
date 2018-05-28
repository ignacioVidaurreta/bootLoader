#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <stdint.h>
#include <keyboard.h>

/* ---------------------------------ATTENTION-------------------------------------------


USR_OUT, USR_ERR and SYS_CLR_USR have been deprecated, now whenever one writes to the screen
it is assumed you are writting to the command line (last line) unless specified otherwise.



------------------------------THANK YOU FOR YOUR TIME--------------------------------*/

//Sys call IDs:
#define SYS_READ 1			
#define SYS_WRITE 2			
#define SYS_CLR_USR 3		//DEPRECATED: now whenever you write to the screen it assumes its the last line unless expressed otherwise
#define SYS_CLR_SCRN 4		//fills all the screen, except for the command line with blank spaces.
#define SYS_READ_KEYA 5		//reads the totality of the keyboard buffer. Returns in rax the total ammount of characters read.
#define SYS_TIME 6 			//returns data related to time. Information expanded in the arguements section.
#define SYS_BEEP 7			//makes the pc speaker beep. arg1: frequency (1000 is recommended if you just want a normal beep).
#define SYS_NO_BEEP 8		//makes the pc speaker stop beeping.
#define SYS_SCRN_INFO 9		//returns information on the screen, such as its width or height, or the width or height of a particular character.
#define SYS_DEL_CHAR 10		//deletes the last user character.

//arguements for read: 1 file descriptor
//					   2 buffer to be read into
//					   3 ammount of characters to be read
//file descriptors: 1 keyboard buffer as defined in keyboard.c.
#define KEY_BUF 1

//arguements for write: 1 file descriptor.
//					    2 buffer to be written from.
//					    3 ammount of characters to be written.
//file descriptors: 1 standard output.
//					2 user output, this represents the last line, into which the user inputs commands.
//					3 user error. The command line will display a red message.
#define STD_OUT 1
#define STD_ERR 2

//possible arguements for calling the time sysCall, all of them received as arg1
//please be aware that the information is returned in greenwich mean time, which
//is 3 hours ahead of Argentina.
#define TICKS 1			//returns in rax the ammount of ticks since system start.
#define SECS 2			//returns seconds since the current minute began.
#define MINS 3			//returns minutes since the current hour began.
#define HRS	4			//returns hours since the current day began.
#define DAYS_WEEK 5		//returns days since the current week began.
#define DAYS_MONTH 6	//returns days since the current month began.
#define MONTHS 7		//returns months since the current year began.
#define YRS 8			//returns years since the current birth of christ (I ain't rewriting this for the second coming).

//possible arguements for calling the SCRN_INFO sysCall, all of them received as arg1
//all of these are measured in pixels.
#define WIDTH 1			//width of the screen.
#define HEIGHT 2		//height of the screen.
#define CWIDTH 3		//width of a character.
#define CHEIGHT 4		//height of a character.

//the following arguements are received in order according to C 64 bit calling convention.
//So, the arguements should be inserted in the order: arg1 = rdi, arg2 = rsi and so on for rdx, r10, r8
//except for the sysCallID which should be inserted into the rax register.
//returns -1 in rax as a failure sign.
int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID);

#endif