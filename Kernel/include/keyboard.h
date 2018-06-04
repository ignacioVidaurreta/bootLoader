#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define ESC 27
#define BACKSPACE 8
#define LEFT_SHIFT 42		//these two definitions are the position of these two keys in the array
#define RIGHT_SHIFT 54		//they serve the purpose of knowing from which array to draw from
#define SHIFT 0				//the actual shift key is actually unnecesary,
#define KF1 0				//as are all the following keys. they are set as constants in case of
#define KF2 0				//future use.
#define KF3 0
#define KF4 0
#define KF5 0
#define KF6 0
#define KF7 0
#define KF8 0
#define KF9 0
#define KF10 0
#define KF11 0
#define KF12 0
#define KHOME 0
#define KUP 0
#define KPGUP 0
#define KLEFT 0
#define KRIGHT 0
#define KEND 0
#define KDOWN 0
#define KPGDN 0
#define KINS 0
#define KDEL 0

#define BUFFERSIZE 80							//since the user will only be able to write to the last line
												//the array length is merely the width of the screen.

void keyboardHandler(void);
void readKeyboardBuffer(char* buffer, int count);
int readKeyboardBufferAll(char* buffer);
void shiftBuffer();

#endif
