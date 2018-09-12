#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <math.h>
#include <time.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <memoryAllocator.h>
#include <RoundRobin.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	loadIDT();
	initializeMemoryAllocation(getStackBase());
	initializeScreen();

	return getStackBase();
}

int main(){
	ncResetPosition();

	int testing = 1;

	if(testing){
		testAddElementToHeader();
		testAddMultipleElementsToHeader();
		testAddALotOfElementsToQueue();
		testRoundRobin();
		testNotFinishedProcessGoesToTail();
	}else{
		((EntryPoint)sampleCodeModuleAddress)();
	}

	haltCPU();
	return 0;
}
