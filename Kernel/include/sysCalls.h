#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <stdint.h>
#include <keyboard.h>

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_RST_USR 2		//limpia la línea de comandos del usuario

//defined file descriptors for write
//file descriptors: 1 standard output.
//					2 user output, this represents the last line, into which the user inputs commands.
//					3 user error. The command line will display a red message.
#define STD_OUT 1
#define USR_OUT 2
#define USR_ERR 3

//defined file descriptors for read
//file descriptors: 1 keyboard buffer defined in keyboard.h.
//					2 the entirety of the keyboard buffer
#define KEY_BUF 1
#define KEY_ALL 2

void int80(uint64_t sysCallID, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif