#ifndef _DOLPHIN_AM_H_
#define _DOLPHIN_AM_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AMCallback)(char* path);

void* AMLoadFile(char* path, u32* length);
u32 AMPush(char* path);
u32 AMPushData(void* buffer, u32 length);
void AMPop(void);
u32 AMGetZeroBuffer(void);
u32 AMGetReadStatus(void);
u32 AMGetFreeSize(void);
u32 AMGetStackPointer(void);
void AMInit(u32 aramBase, u32 aramBytes);

#ifdef __cplusplus
}
#endif

#endif
