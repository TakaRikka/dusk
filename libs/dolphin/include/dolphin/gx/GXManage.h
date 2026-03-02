#ifndef _DOLPHIN_GX_GXMANAGE_H_
#define _DOLPHIN_GX_GXMANAGE_H_

#include <dolphin/gx/GXFifo.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GXDrawSyncCallback)(u16 token);
typedef void (*GXDrawDoneCallback)(void);

// Init
BOOL IsWriteGatherBufferEmpty(void);
GXFifoObj* GXInit(void* base, u32 size);

// Misc
void GXSetMisc(GXMiscToken token, u32 val);
void GXFlush(void);
void GXResetWriteGatherPipe(void);
void GXAbortFrame(void);
void GXSetDrawSync(u16 token);
u16 GXReadDrawSync(void);
void GXSetDrawDone(void);
void GXWaitDrawDone(void);
void GXDrawDone(void);
void GXPixModeSync(void);
void GXTexModeSync(void);
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb);
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb);

#ifdef __cplusplus
}
#endif

#endif
