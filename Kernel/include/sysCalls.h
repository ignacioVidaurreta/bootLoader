#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <stdint.h>
#include <keyboard.h>

//Sys call IDs:
#define SYS_READ 1
#define SYS_WRITE 2
#define SYS_CLR_SCRN 3		//fills all the screen, except for the command line with blank spaces.
#define SYS_READ_KEYA 4		//reads the totality of the keyboard buffer. Returns in rax the total ammount of characters read.
#define SYS_TIME 5 			//returns data related to time. Information expanded in the arguements section.
#define SYS_BEEP 6			//makes the pc speaker beep. arg1: frequency (1000 is recommended if you just want a normal beep).
#define SYS_NO_BEEP 7		//makes the pc speaker stop beeping.
#define SYS_SCRN_INFO 8		//returns information on the screen, such as its width or height, or the width or height of a particular character.
#define SYS_DEL_CHAR 9		//deletes the last user character.
#define SYS_DRAW_PXL 10		//draws a pixel at the specified location and with the specified colour. Information expanded in the arguements section.
#define SYS_SCRL 11			//scrawls the screen up by one line.
#define DRAW_NUM 12     	//Prints in ascii art a number (used to print the clock).
#define SYS_NEW_PROC 13     //Creates a new process
#define SYS_PRINT_PROC 14   //Prints every running process





//there are special characters which do not have an ascii representation
//however they are stored in the keyboard buffer and returned this are:
//backspace which is represented by an 8 and ESC which is represented by a 27.
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






//arguements for the DRAW_PXL sysCall. arg1: x position, distance in pixels from the left edge of the screen of the pixel to draw.
//									   arg2: y position, distance in pixels from the top edge of the screen of the pixel to draw.
//									   arg3: red, intensity of the color red in the pixel.
//									   arg4: green, intensity of the color green in the pixel.
//									   arg5: blue, intensity of the color blue in the pixel.
//please be aware colors must be of an 8 bit value, that is, they should go from 0 to 0xFF.
//For example passing red, green and blue as 0, 0, 0 will paint the pixel black. Passing them as 0xFF, 0xFF, 0xFF will create a white pixel.


// arguments for the DRAW_NUM syscall.
//                         arg1: x position, distance in pixels from the left edge of the screen of the pixel to draw.
//                                           arg2: y position, distance in pixels from the top edge of the screen of the pixel to draw.
//                         arg3: number that represents the color we will print the number
//                         arg4: The number (si no recibe un número imprime el separador : )
// In arg3: 0-> white, 1-> red, 2-> green, 3-> blue




//the following arguements are received in order according to C 64 bit calling convention.
//So, the arguements should be inserted in the order: arg1 = rdi, arg2 = rsi and so on for rdx, r10, r8
//except for the sysCallID which should be inserted into the rax register.
//returns -1 in rax as a failure sign.
int int80(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t sysCallID);

#endif
