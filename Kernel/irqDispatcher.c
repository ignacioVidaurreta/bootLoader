#include <time.h>
#include <peripherals.h>
#include <stdint.h>
#include <keyboard.h>

static void int20();
static void int21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int20();
			break;
		case 1:
			int21();
			break;
	}
	return;
}

void int20() {
	timerHandler();
}

void int21(){
	keyboardHandler();
}