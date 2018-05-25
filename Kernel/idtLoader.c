#include <stdint.h>
#include <defs.h>
#include <idtLoader.h>
#include <peripherals.h>
#include <interrupts.h>

//the following code obtained from an example on campus
//as well as most of he code on interrupts.asm
INT_DESCR * idt = (INT_DESCR *) 0;	// IDT with 255 entries

static void setupIDTEntry (int index, uint64_t offset);

void loadIDT() {

  setupIDTEntry (0x20, (uint64_t)&irq00Handler);
  setupIDTEntry (0x21, (uint64_t)&irq01Handler);
  setupIDTEntry (0x00, (uint64_t)&exception0Handler);
  setupIDTEntry (0x06, (uint64_t)&exception6Handler);
  setupIDTEntry (0x80, (uint64_t)&int80Handler);

	picMasterMask(0xFC); 
	picSlaveMask(0xFF);
        
	setI();
}

static void setupIDTEntry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}