#include <naiveConsole.h>

typedef struct {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} Colour;

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void drawAPixelWithColour(int x, int y, Colour col);

struct ModeInfoBlock {
    uint16_t ModeAttributes;
    uint8_t WinAAttributes;
    uint8_t WinBAttributes;
    uint16_t WinGranularity;
    uint16_t WinSize;
    uint16_t WinSegmentA;
    uint16_t WinSegmentB;
    uint32_t WinRealFctPtr;
    uint16_t pitch; 					// Bytes per ScanLine.

    uint16_t XResolution;
    uint16_t YResolution;
    uint8_t XcharSize;
    uint8_t YcharSize;
    uint8_t NumberOfPlanes;
    uint8_t BitsPerPixel;
    uint8_t NumberOfBanks;
    uint8_t MemoryModel;
    uint8_t BankSize;
    uint8_t NumberOfImagePages;
    uint8_t ReservedPage;

    uint8_t RedMaskSize;
    uint8_t RedMaskPosition;
    uint8_t GreenMaskSize;
    uint8_t GreenMaskPosition;
    uint8_t BlueMaskSize;
    uint8_t BlueMaskPosition;
    uint8_t ReservedMaskSize;
    uint8_t ReservedMaskPosition;
    uint8_t DirectColorAttributes;

    uint32_t PhysBasePtr;  				// Your LFB (Linear Framebuffer) address.
    uint32_t OffScreenMemOffset;
    uint16_t OffScreenMemSize;
} __attribute__((packed));

//The _attribute_ modifiers are needed to make gcc pack the structure into 
//the standard VESA layout, rather than adding pad bytes between some of the 
//fields like it would normally do.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

typedef struct ModeInfoBlock ModeInfoVBE;

ModeInfoVBE* VBEModeInfo = (ModeInfoVBE*) 0x5C00;

void drawAPixelWithColour(int x, int y, Colour col)
{
    char * video = (char *) ((uint64_t)(VBEModeInfo->PhysBasePtr + VBEModeInfo->pitch *y + x* (int)(VBEModeInfo->BitsPerPixel/8)));
    video[0] = col.Blue;
    video[1] = col.Green;
    video[2] = col.Red;
}


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

void scTest(void){

	Colour col;
	col.Red = 0;
	col.Green = 0xFF;
	col.Blue = 0;
	for(int i = 0; i < 100000; i++)
		for(int j = 0; j < 100000; j++)
			drawAPixelWithColour(i,j, col);
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