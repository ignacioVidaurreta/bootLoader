#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncPrintInColor(const char * string, uint8_t color);
void ncPrintCharInColor(char character, uint8_t color);
void ncPrintUser(const char* buffer);
void ncPrintUserChar(char c);
void ncPrintUserInColor(const char* buffer, uint8_t color);
void ncPrintUserCharInColor(char character, uint8_t color);
void ncClearUser(void);
void ncDeleteChar();
void ncMoveUpOneLine();
void ncClear();

#endif