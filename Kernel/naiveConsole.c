#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#pragma pack(push)
#pragma pack(1)
//http://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
typedef struct {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;				// number of bytes per horizontal line
	uint16_t width;				// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;				// unused...
	uint8_t y_char;				// ...
	uint8_t planes;
	uint8_t bpp;				// bits per pixel in this mode
	uint8_t banks;				// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;			// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;				// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;		// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
}vbeModeInfoStructure;

#pragma pack(pop)

vbeModeInfoStructure* vbeInfo = (vbeModeInfoStructure*) 0x5C00;

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static uint8_t * lastNonUserLine = (uint8_t*)0xB8E60;
static uint8_t * lastNonUserChar = (uint8_t*)0xB8EFF;
static uint8_t * userVideo = (uint8_t*)0xB8F00;
static uint8_t * currentUser = (uint8_t*)0xB8F00;
static uint8_t * maxVideo = (uint8_t*)0xB9040;
static const uint32_t width = 80;
static const uint32_t height = 24;					//the ammount of lines in the screen is actually 25, however,
													//since the last line will be reserved for user commands
													//writing to it will have its own special function.

void ncPrintInColor(const char * string, uint8_t color)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintCharInColor(string[i], color);
}

void scTest(){
	ncPrintDec(vbeInfo->width);
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

void ncPrint(const char * string)
{
	int i;
	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character){
	ncPrintCharInColor(character, WHITE);
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintUser(const char* buffer){
	ncPrintUserInColor(buffer, WHITE);
}

void ncPrintUserInColor(const char* buffer, uint8_t color){
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
	while(currentUser != userVideo){
		*currentUser = ' ';
		currentUser -= 2;
	}
}

void ncMoveUpOneLine(void){

	int newScreen = width*2;
	int i = 0;
	while(i < width*2*height){
		video[i] = video[newScreen];
		i++;
		newScreen++;
	}
	while(i < width*2){
		lastNonUserLine[i] = ' ';
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