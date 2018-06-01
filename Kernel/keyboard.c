#include <peripherals.h>
#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>

void cleanBuffer(int count);

char keyBuffer[BUFFERSIZE];

//the following two arrays define the layout of the standard US keyboard
//in the form of a keymap. The signal sent to the processor by the keyboard will
//correspond to that key's position in the array. There are two separate arrays
//because the keys you receive depend on if shift is pressed or not.
//
//IMPORTANT: the code for the realease of a key is equal to the code for pressing
//that key + 128. Don't ask me I didn't design it.

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

int bufferIndex = 0;

int shiftl = 0;					//flags to know whether shift has been pressed or not.
int shiftr = 0;

void keyboardHandler(void){
	int key = readFromKeyboard();
	if(key==-1)
		keyBuffer[bufferIndex++]=0;
	switch (key){
		case RIGHT_SHIFT:
			shiftr = 1;
			break;
		case LEFT_SHIFT:
			shiftl = 1;
			break;
		case RIGHT_SHIFT + 128:
			shiftr = 0;
			break;
		case LEFT_SHIFT + 128:
			shiftl = 0;
			break;
	}
	if((shiftr | shiftl) && (key < 128)){
		keyBuffer[bufferIndex++] = asciiShift[key];
	}
	else if(key < 128){
		keyBuffer[bufferIndex++] = asciiNonShift[key];
	}
	if(bufferIndex == BUFFERSIZE)
			bufferIndex = 0;
}

//Para recordar: este comentario debe ser removido una vez cumplido
//al hacer funciones que lean de este buffer, debería limpiárselo de ante mano
//debido a que a la función que lea probablemente no ha de interesarle lo que
//se haya escrito antes de que empezó a correr.
void readKeyboardBuffer(char* buffer, int count){
	if(count > 0){
		for(int i = 0; i < count; i++)
			buffer[i] = keyBuffer[i];
		cleanBuffer(count);
	}
}

int readKeyboardBufferAll(char* buffer){
	int ret = bufferIndex;
	readKeyboardBuffer(buffer, bufferIndex);
	return ret;
}

// void cleanBuffer(int count){
// 	for(int i = 0, j = count; i < count, j < bufferIndex; i++, j++)
// 		keyBuffer[i] = keyBuffer[j];
// 	if(bufferIndex > count)
// 		bufferIndex -= count;
// 	else
// 		bufferIndex = 0;
// }
	void cleanBuffer(int count){
		for (int i =0; i< count; i++){
			keyBuffer[i] =0;
		}
		if (bufferIndex > count)
			bufferIndex -= count;
		else
			bufferIndex = 0;
	}
