#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdint.h>

uint8_t getTimeFromRTC(uint8_t time);
int readFromKeyboard(void);
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);
void beepASM(uint16_t frequency);
uint8_t noBeepASM();

#endif