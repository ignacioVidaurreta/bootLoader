#include <time.h>
#include <peripherals.h>
#include <stdint.h>
#include <keyboard.h>

static void int20();
static void int21();

uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			return int20(rsp);
		case 1:
			int21();
			break;
	}
	return rsp;
}

void int20(uint64_t rsp) {
	timerHandler();
	return contextSwitch(rsp);
}

void int21(){
	keyboardHandler();
}