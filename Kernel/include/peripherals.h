#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdint.h>

uint8_t getTimeFromRTC(uint8_t time);
int readFromKeyboard(void);
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);
void outb(uint16_t address, uint8_t bits);
uint8_t inb(uint16_t address);

#endif