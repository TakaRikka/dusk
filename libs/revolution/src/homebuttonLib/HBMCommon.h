#ifndef HOMEBUTTON_COMMON_H
#define HOMEBUTTON_COMMON_H

#include <revolution/mem.h>

extern "C" MEMAllocator* spAllocator;

void* HBMAllocMem(u32 length);
void HBMFreeMem(void* ptr);

#endif
