#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <stdint.h>

#define SYS_WRITE 4

void int80(uint64_t sysCallID, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif