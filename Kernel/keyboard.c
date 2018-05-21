#include <peripherals.h>
#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>

//por ahora solo maneja caracteres nonshift

int isAlphaNumeric(char c);
int pressed = 0;

void keyboardHandler(void){

	char key = readFromKeyboard();
	char c = asciiNonShift[key];
	if((isAlphaNumeric(c) | (c == ' ')) && !pressed){
		pressed = 1;
		ncPrintChar(c);
	}
	else if (c == BACKSPACE){
		ncDeleteChar();
	}
	else pressed = 0;
}

int isAlphaNumeric(char c){

	if((c >= 48 && c <= 57) | (c >= 97 && c <= 122))
		return 1;
	return 0;
}