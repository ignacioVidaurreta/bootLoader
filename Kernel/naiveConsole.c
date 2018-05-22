#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static uint8_t * lastNonUserLine = (uint8_t*)0xB8FA0;
static uint8_t * lastNonUserChar = (uint8_t*)0xB8E99;
static uint8_t * userVideo = (uint8_t*)0xB8F00;
static uint8_t * currentUser = (uint8_t*)0xB8F00;
static uint8_t * maxVideo = (uint8_t*)0xB9040;
static const uint32_t width = 80;
static const uint32_t height = 24;					//the ammount of lines in the screen is actually 25, however,
													//since the last line will be reserved for user commands
													//writing to it will have its own special function.

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character){
	ncPrintCharInColor(character, WHITE);
}

void ncDeleteChar(){
	currentVideo -= 2;
	*currentVideo = ' ';
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintUserLine(char* buffer){
	ncPrintUserLineInColor(buffer, WHITE);
}

void ncPrintUserLineInColor(char* buffer, uint8_t color){
	for(int i = 0; buffer[i] != 0; i++){
		ncPrintUserCharInColor(buffer[i], color);
	}
}

void ncPrintUserChar(char c){
	ncPrintUserCharInColor(c, WHITE);
}

void ncPrintUserCharInColor(char c, uint8_t color){
	*currentUser = c;
	currentUser++;
	*currentUser = color;
	currentUser++;

	if(currentUser == maxVideo)
		ncClearUser();
}

void ncClearUser(void){
	currentUser = userVideo;
	while(currentUser < maxVideo)
		ncPrintUserChar(' ');
	currentUser = userVideo;
}

void ncMoveUpOneLine(void){

	int newScreen = width*2;
	int i = 0;
	while(i < width*2*(height-1)){
		video[i*2] = currentVideo[newScreen*2];
		i++;
		newScreen++;
	}
	while(i < width*height*2){
		currentVideo[i] = ' ';
		i++;
	}
	currentVideo = lastNonUserLine;
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncPrintInColor(const char * string, uint8_t color)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintCharInColor(string[i], color);
}

void ncPrintCharInColor(char character, uint8_t color)
{
	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;

	if(currentVideo == lastNonUserChar)
		ncMoveUpOneLine();
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}