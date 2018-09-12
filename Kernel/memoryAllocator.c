#include <stdint.h>

#define PAGE_SIZE 4096
#define MEMORY_SIZE 100*4096	//placeholder number
#define NUMBER_OF_PAGES 32768	//placeholder number

uint64_t roundToPageSizeMultiple(uint64_t memory);
int isFree(uint64_t page);
int pageNumber(uint64_t page);
void freeAllMemory();
void markAsTaken(uint64_t lastAdress, uint64_t memoryOccupied);
void freeMemory(uint64_t startingAdress, uint64_t finalAdress);

uint64_t baseAddress;
uint64_t firstAvailableMemoryAdress;
uint8_t freeAddresses[NUMBER_OF_PAGES];

void initializeMemoryAllocation(uint64_t baseAd){
	baseAddress = roundToPageSizeMultiple((uint64_t)baseAd);
	firstAvailableMemoryAdress = baseAd;
	freeAllMemory();
}

uint64_t allocateMemory(uint64_t desiredMemory){

	uint64_t roundedMemory = roundToPageSizeMultiple(desiredMemory);
	uint64_t memoryYetToAllocate = roundedMemory;
	uint64_t memoryToBeSearched = firstAvailableMemoryAdress;
	while(memoryYetToAllocate > 0 && memoryToBeSearched < baseAddress + MEMORY_SIZE){
		if(isFree((uint64_t)memoryToBeSearched))
			memoryYetToAllocate -= PAGE_SIZE;
		else
			memoryYetToAllocate = roundedMemory;
		memoryToBeSearched += PAGE_SIZE;
	}
	if(memoryYetToAllocate == 0){
		markAsTaken((uint64_t)memoryToBeSearched, roundedMemory);
		return memoryToBeSearched - roundedMemory;
	}
	else
		return 0;	// 0, also known as the null pointer.
}

uint64_t roundToPageSizeMultiple(uint64_t memory){
	return memory - (memory%PAGE_SIZE) + PAGE_SIZE;
}

int isFree(uint64_t page){
	return freeAddresses[pageNumber(page)];
}

int pageNumber(uint64_t page){
	return (page - baseAddress)/PAGE_SIZE;
}

void freeAllMemory(){
	freeMemory(baseAddress, baseAddress + MEMORY_SIZE);
}

void markAsTaken(uint64_t lastAdress, uint64_t memoryOccupied){
	uint64_t firstAdress = lastAdress - memoryOccupied;
	while(lastAdress > firstAdress){
		freeAddresses[pageNumber(lastAdress)] = 0;
		lastAdress -= PAGE_SIZE;
	}
}

void freeMemory(uint64_t startingAdress, uint64_t finalAdress){
	while(startingAdress < finalAdress){
		freeAddresses[pageNumber(startingAdress)] = 1;
		startingAdress += PAGE_SIZE;
	}
}
