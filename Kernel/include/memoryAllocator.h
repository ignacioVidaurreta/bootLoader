#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

void initializeMemoryAllocation(uint64_t *baseAd);
uint64_t allocateMemory(uint64_t desiredMemory);

#endif