#ifndef _REVOLUTION_OSMEMORY_H_
#define _REVOLUTION_OSMEMORY_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_PROTECT_CHAN0 0
#define OS_PROTECT_CHAN1 1
#define OS_PROTECT_CHAN2 2
#define OS_PROTECT_CHAN3 3

#define OS_PROTECT_CONTROL_NONE 0x00
#define OS_PROTECT_CONTROL_READ 0x01
#define OS_PROTECT_CONTROL_WRITE 0x02
#define OS_PROTECT_CONTROL_RDWR (OS_PROTECT_CONTROL_READ | OS_PROTECT_CONTROL_WRITE)

void OSProtectRange(u32 chan, void* addr, u32 nBytes, u32 control);
u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);

#ifdef __cplusplus
}
#endif

#endif
