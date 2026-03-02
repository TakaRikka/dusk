#ifndef _REVOLUTION_GX_GXDISPLIST_H_
#define _REVOLUTION_GX_GXDISPLIST_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void* list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList(void* list, u32 nbytes);

#ifdef __cplusplus
}
#endif

#endif
