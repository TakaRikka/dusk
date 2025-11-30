#ifndef _REVOLUTION_OSIPC_H_
#define _REVOLUTION_OSIPC_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void* __OSGetIPCBufferHi(void);
void* __OSGetIPCBufferLo(void);
void __OSInitIPCBuffer(void);

#ifdef __cplusplus
}
#endif


#endif // _REVOLUTION_OSIPC_H_
