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
				
#define WIDTH 80							//since the user will only be able to write to the last line
											//the array length is merely the width of the screen.

char keyBuffer[WIDTH];

//the following two arrays define the layout of the standard US keyboard
//in the form of a keymap. The signal sent to the processor by the keyboard will
//correspond to that key's position in the array. There are two separate arrays
//because the keys you receive depend on if shift is pressed or not.
//
//IMPORTANT: the code for the realease of a key is equal to the code for pressing
//that key + 128. Don't ask me I didn't desing it.

const unsigned char asciiNonShift[] = {
0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
'\t', 'q', 'w','e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', '\n', 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHIFT, '\\',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', SHIFT, 0, 0, ' ', 0,
KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

const unsigned char asciiShift[] = {
0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE,
'\t', 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', '\n', 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
KF1,   KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP, '-', KLEFT, '5',   KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };

void keyboardHandler(void);

#endif