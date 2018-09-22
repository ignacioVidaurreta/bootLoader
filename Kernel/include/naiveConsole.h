#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Colour;

typedef struct {
	int x;
	int y;
} Position;

//structure obtained from https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
typedef struct {
	uint16_t attributes;				// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;					// deprecated
	uint8_t window_b;					// deprecated
	uint16_t granularity;				// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;			// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;					// number of bytes per horizontal line
	uint16_t width;					// width in pixels
	uint16_t height;					// height in pixels
	uint8_t x_char;					// unused...
	uint8_t y_char;					// ...
	uint8_t planes;
	uint8_t bpp;						// bits per pixel in this mode
	uint8_t banks;					// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;				// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
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
} __attribute__ ((packed)) vbeModeInfoStructure;

void drawPixelWithColour(int x, int y, Colour col);

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewLineAt(Position* p);
void ncNewLine();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncPrintInColor(const char * string, Colour color);
void ncPrintCharInColorAt(char character, Colour color, Position* p);
void ncPrintInColorAt(const char* character, Colour color, Position* line);
void ncMoveUpOneLine();
void ncDeleteChar();
void ncClear();
void ncResetPosition();
void ncScroll();

void initializeScreen(void);

void ncPrintNumberParser(int num, int color, Position* p);
void ncPrintNumber( int num, Colour c, Position* p);

int width();
int height();
int cwidth();
int cheight();

#endif
