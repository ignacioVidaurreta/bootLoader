#include <peripherals.h>
#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>

//por ahora solo maneja caracteres nonshift
		
int bufferIndex = 0;

int shiftl = 0;					//flags to know whether shift has been pressed or not.
int shiftr = 0;

void keyboardHandler(void){
	int key = readFromKeyboard();
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
	if((shiftr | shiftl) && (key < 128))
		keyBuffer[bufferIndex++] = asciiShift[key];
	else if(key < 128)
		keyBuffer[bufferIndex++] = asciiNonShift[key];
}