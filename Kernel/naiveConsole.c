#include <naiveConsole.h>
#include <bitMap.h>
#include <time.h>

char* title[16] ={"                                                                   OOOOOOOOO         SSSSSSSSSSSSSSS ",
				  "                                                                 OO:::::::::OO     SS:::::::::::::::S",
				  "                                                               OO:::::::::::::OO  S:::::SSSSSS::::::S",
				  "                                                              O:::::::OOO:::::::O S:::::S     SSSSSSS",
				  "    ssssssssss    nnnn  nnnnnnnn        mmmmmmm    mmmmmmm    O::::::O   O::::::O S:::::S            ",
				  "  ss::::::::::s   n:::nn::::::::nn    mm:::::::m  m:::::::mm  O:::::O     O:::::O S:::::S            ",
				  "ss:::::::::::::s  n::::::::::::::nn  m::::::::::mm::::::::::m O:::::O     O:::::O  S::::SSSS         ",
				  "s::::::ssss:::::s nn:::::::::::::::n m::::::::::::::::::::::m O:::::O     O:::::O   SS::::::SSSSS    ",
				  " s:::::s  ssssss    n:::::nnnn:::::n m:::::mmm::::::mmm:::::m O:::::O     O:::::O     SSS::::::::SS  ",
				  "   s::::::s         n::::n    n::::n m::::m   m::::m   m::::m O:::::O     O:::::O        SSSSSS::::S ",
				  "      s::::::s      n::::n    n::::n m::::m   m::::m   m::::m O:::::O     O:::::O             S:::::S",
				  "ssssss   s:::::s    n::::n    n::::n m::::m   m::::m   m::::m O::::::O   O::::::O             S:::::S",
				  "s:::::ssss::::::s   n::::n    n::::n m::::m   m::::m   m::::m O:::::::OOO:::::::O SSSSSSS     S:::::S",
				  "s::::::::::::::s    n::::n    n::::n m::::m   m::::m   m::::m  OO:::::::::::::OO  S::::::SSSSSS:::::S",
				  " s:::::::::::ss     n::::n    n::::n m::::m   m::::m   m::::m    OO:::::::::OO    S:::::::::::::::SS ",
				  "  sssssssssss       nnnnnn    nnnnnn mmmmmm   mmmmmm   mmmmmm      OOOOOOOOO       SSSSSSSSSSSSSSS   ",};

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

vbeModeInfoStructure* vbeInfo = (vbeModeInfoStructure*) 0x5C00;
Position position = {0,0};
Colour WHITE = {.red = 0xFF, .green = 0xFF, .blue = 0xFF};
Colour RED = {.red = 0xFF, .green = 0, .blue = 0};
Colour GREEN = {.red = 0, .green = 0xFF, .blue = 0};
Colour BLUE = {.red = 0, .green = 0, .blue = 0xFF};
Colour BLACK = {.red = 0, .green = 0, .blue = 0};

void drawPixelWithColour(int x, int y, Colour col)
{
    char * video = (char *) ((uint64_t)(vbeInfo->framebuffer + vbeInfo->pitch *y + x* (int)(vbeInfo->bpp/8)));
    video[0] = col.blue;
    video[1] = col.green;
    video[2] = col.red;
}


static char buffer[64] = { '0' };

void initializeScreen(void){
	position.x = 0;
	position.y = vbeInfo->height - CHAR_HEIGHT;
	Position titlePosition = {0,0};
	ncPrintInColorAt("welcome to:", GREEN, &titlePosition);
	for(int i = 0; i < 16; i++){
		titlePosition.x = CHAR_WIDTH*5;
		titlePosition.y = CHAR_HEIGHT*(i + 2);
		ncPrintInColorAt(title[i], GREEN, &titlePosition);
	}
	while(secondsElapsed() < 3){;}		//espera para mostrar el siguinte mensaje
	ncClear();
	titlePosition.x = vbeInfo->width/2 - CHAR_WIDTH*12;
	ncPrintInColorAt("hope you enjoy yourself!", GREEN, &titlePosition);
	while(secondsElapsed() < 6){;}		//espera para borrar la pantalla
	ncClear();
}

void ncPrintInColor(const char * string, Colour color)
{
	ncPrintInColorAt(string, color, &position);
}

void ncPrintInColorAt(const char * string, Colour color, Position* p){
	for (int i = 0; string[i] != 0; i++)
		ncPrintCharInColorAt(string[i], color, p);
}

void ncPrintCharInColorAt(char character, Colour color, Position* p)
{	
	if(p->x >= vbeInfo->width){
		p->x = 0;
		if(p->y >= vbeInfo->height - CHAR_HEIGHT)
			ncMoveUpOneLine();
		else
			p->y += CHAR_HEIGHT;
	}
    uint8_t * letter = pixelMap(character);
	for(int i = 0; i < CHAR_HEIGHT; i++) {
      for(int j = 0; j < CHAR_WIDTH; j++) {
         if(1<<j & letter[i])
            drawPixelWithColour(CHAR_WIDTH - 1 - j + p->x, i + p->y, color);
         else
            drawPixelWithColour(CHAR_WIDTH - 1 - j + p->x, i + p->y, BLACK);   
      }
    }
    p->x += CHAR_WIDTH;
}

void ncPrint(const char * string)
{
	ncPrintInColor(string, WHITE);
}

void ncNewline(Position* p)
{
	int startingY = p->y;
	while(p->x != vbeInfo->width  && startingY == p->y)
		ncPrintCharInColorAt(' ', WHITE, p);
}

void copyPixel(int toX, int toY, int fromX, int fromY){
	char* videoFrom = (char *) ((uint64_t)(vbeInfo->framebuffer + vbeInfo->pitch *fromY + fromX* (int)(vbeInfo->bpp/8)));
	char* videoTo = (char *) ((uint64_t)(vbeInfo->framebuffer + vbeInfo->pitch *toY + toX* (int)(vbeInfo->bpp/8)));
	videoTo[0] = videoFrom[0];
	videoTo[1] = videoFrom[1];
	videoTo[2] = videoFrom[2];
}

void ncDeleteChar(){
	if(position.y != 0){
		if(position.x == 0){
			position.x = vbeInfo->width - CHAR_WIDTH;
		}
		else
			position.x -= CHAR_WIDTH;
	}
	else if(position.x != 0)
		position.x -= CHAR_WIDTH;
	Position p = {position.x, position.y};
	ncPrintCharInColorAt(' ', WHITE, &p);
}

void ncClear()
{
	for(int y = 0; y < vbeInfo->height; y ++)
		for(int x = 0; x < vbeInfo->width; x ++)
			drawPixelWithColour(x,y, BLACK);
}

void ncMoveUpOneLine(void){
	for(int y = 0; y < vbeInfo->height; y++)
		for(int x = 0; x < vbeInfo->width; x++)
			copyPixel(x, y, x, y + CHAR_HEIGHT);
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

int width(){return vbeInfo->width;}
int height(){return vbeInfo->height;}
int cwidth(){return CHAR_WIDTH;}
int cheight(){return CHAR_HEIGHT;}