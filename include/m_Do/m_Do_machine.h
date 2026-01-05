#ifndef M_DO_M_DO_MACHINE_H
#define M_DO_M_DO_MACHINE_H

#include <dolphin/gx.h>

typedef struct OSContext OSContext;
class JKRHeap;

void myHeapCheckRecursive(JKRHeap*);
BOOL mDoMch_IsProgressiveMode();
void exceptionReadPad(u32*, u32*);
void exceptionRestart();
void myExceptionCallback(u16, OSContext*, u32, u32);
void my_SysPrintHeap(char const*, void*, u32);
void mDoMch_HeapCheckAll();
void mDoMch_HeapFreeFillAll();
int mDoMch_Create();

extern GXRenderModeObj g_ntscZeldaProg;

class mDoMch_render_c {
public:
    static void setRenderModeObj(GXRenderModeObj* obj) { mRenderModeObj = obj; }
    static void setProgressiveMode() { setRenderModeObj(&g_ntscZeldaProg); }
    static u16 getEfbHeight() { return mRenderModeObj->efbHeight; }
    static u16 getFbWidth() { return mRenderModeObj->fbWidth; }

    static GXRenderModeObj* getRenderModeObj() { return mRenderModeObj; }

    static GXRenderModeObj* mRenderModeObj;
};

namespace mDoMch {
    extern u8 mDebugFill;
    extern u8 mDebugFillNotUse;
    extern u8 mDebugFillNew;
    extern u8 mDebugFillDelete;

    extern u8 myHeapVerbose;
    extern u8 myHeapCallbackCheck;
    extern u8 FpscrEnableBits;
    extern u8 GXWarningLevel;
    extern u8 GXWarningExecuteFrame;
};

#endif /* M_DO_M_DO_MACHINE_H */
