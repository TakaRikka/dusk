#include <dolphin/dolphin.h>
#include <dolphin/pad.h>
#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include <dolphin/demo.h>

#include "__demo.h"

extern u8 DemoStatEnable;

static GXRenderModeObj rmodeobj;

static u8 DemoFirstFrame = 1;

static void* DefaultFifo = NULL;
static GXFifoObj* DefaultFifoObj = NULL;
static GXRenderModeObj* rmode;
static u32 allocatedFrameBufferSize;
static int GPHangWorkaround;
static u32 FrameCount;
static u32 FrameMissThreshold;
void* DemoFrameBuffer1;
void* DemoFrameBuffer2;
void* DemoCurrentBuffer;

// prototypes
static void __DEMOInitRenderMode(GXRenderModeObj* mode);
static void __DEMOInitMem(void);
static void __DEMOInitGX(void);
static void __DEMOInitVI(void);
static void __DEMOInitForEmu(void);
static void __NoHangRetraceCallback(u32 count);
static void __NoHangDoneRender(void);
static void __DEMODiagnoseHang(void);

void DEMOInit(GXRenderModeObj* mode) {
    OSInit();
    DVDInit();
    VIInit();
    DEMOPadInit();
    __DEMOInitRenderMode(mode);
    __DEMOInitMem();
    VIConfigure(rmode);
    DefaultFifo = OSAllocFromHeap(__OSCurrHeap, 0x40000);
    DefaultFifoObj = GXInit(DefaultFifo, 0x40000);
    __DEMOInitGX();
    __DEMOInitVI();
}

static void __DEMOInitRenderMode(GXRenderModeObj* mode) {
    if (mode != NULL) {
        rmodeobj = *mode;
        rmode = &rmodeobj;
        return;
    }

    switch(VIGetTvFormat()) {
    case VI_NTSC:
        rmode = &GXNtsc480IntDf;
        break;
    case VI_PAL:
        rmode = &GXPal528IntDf;
        break;
    case VI_EURGB60:
        rmode = &GXEurgb60Hz480IntDf;
        break;    
    case VI_MPAL:
        rmode = &GXMpal480IntDf;
        break;
    default:
        OSPanic(__FILE__, 473, "DEMOInit: invalid TV format\n");
        break;
    }

    GXAdjustForOverscan(rmode, &rmodeobj, 0, 16);
    rmode = &rmodeobj;
}

static void __DEMOInitMem(void) {
    void* arenaLo = OSGetArenaLo();
    void* arenaHi = OSGetArenaHi();
    u32 fbSize = ((u16)(rmode->fbWidth + 15) & 0xFFF0) * rmode->xfbHeight * 2;
    allocatedFrameBufferSize = fbSize;

    DemoFrameBuffer1 = (void*)OSRoundUp32B(arenaLo);
    DemoFrameBuffer2 = (void*)OSRoundUp32B((u32)DemoFrameBuffer1 + fbSize);
    DemoCurrentBuffer = DemoFrameBuffer2;

    arenaLo = (void*)OSRoundUp32B((u32)DemoFrameBuffer2 + fbSize);
    OSSetArenaLo(arenaLo);

    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetArenaLo(arenaLo);

    arenaLo = (void*)OSRoundUp32B(arenaLo);
    arenaHi = (void*)OSRoundDown32B(arenaHi);
    OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
    OSSetArenaLo((arenaLo = arenaHi));
}

static void __DEMOInitGX(void) {
    u16 xfbHeight;
    f32 yScale;

    GXSetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);

    yScale = GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    xfbHeight = GXSetDispCopyYScale(yScale);

    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, xfbHeight);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, 1, rmode->vfilter);

    if (rmode->aa != 0) {
        GXSetPixelFmt(2, 0);
    } else {
        GXSetPixelFmt(0, 0);
    }

    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDispCopyGamma(0);
}

static void __DEMOInitVI(void) {
    u32 nin;

    VISetNextFrameBuffer(DemoFrameBuffer1);
    DemoCurrentBuffer = DemoFrameBuffer2;
    VIFlush();
    VIWaitForRetrace();
    nin = rmode->viTVmode & 1;
    if (nin != 0) {
        VIWaitForRetrace();
    }
}

static void __DEMOInitForEmu(void) {}

void DEMOBeforeRender(void) {
    if (GPHangWorkaround != 0) {
        GXSetDrawSync(0xFEEB);
        GXClearGPMetric();
    }

    if (rmode->field_rendering != 0) {
        GXSetViewportJitter(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f, VIGetNextField());
    } else {
        GXSetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);
    }

    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

void DEMODoneRender(void) {
    if (GPHangWorkaround != 0) {
        ASSERTMSGLINE(749, !DemoStatEnable, "DEMOStats and GP hang diagnosis are mutually exclusive");
        __NoHangDoneRender();
        return;
    }

    if (DemoStatEnable != 0) {
        GXDrawDone();
        DEMOUpdateStats(1);
        DEMOPrintStats();
        GXDrawDone();
        DEMOUpdateStats(0);
    }

    GXSetZMode(1, 3, 1);
    GXSetColorUpdate(1);
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXDrawDone();
    DEMOSwapBuffers();
}

void DEMOSwapBuffers(void) {
    VISetNextFrameBuffer(DemoCurrentBuffer);
    if (DemoFirstFrame != 0) {
        VISetBlack(0);
        DemoFirstFrame = 0;
    }

    VIFlush();
    VIWaitForRetrace();

    if ((u32)DemoCurrentBuffer == (u32)DemoFrameBuffer1) {
        DemoCurrentBuffer = DemoFrameBuffer2;
    } else {
        DemoCurrentBuffer = DemoFrameBuffer1;
    }
}

void DEMOSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) {
    u32 swap = 0;

    if (a == GX_CC_TEXC) {
        swap = 0xF;
    } else if (a >= GX_CC_TEXRRR) {
        swap = a;
        a = GX_CC_TEXC;
    }

    if (b == GX_CC_TEXC) {
        swap = 0xF;
    } else if (b >= GX_CC_TEXRRR) {
        swap = b;
        b = GX_CC_TEXC;
    }

    if (c == GX_CC_TEXC) {
        swap = 0xF;
    } else if (c >= GX_CC_TEXRRR) {
        swap = c;
        c = GX_CC_TEXC;
    }

    if (d == GX_CC_TEXC) {
        swap = 0xF;
    } else if (d >= GX_CC_TEXRRR) {
        swap = d;
        d = GX_CC_TEXC;
    }

    GXSetTevColorIn(stage, a, b, c, d);
    if (swap != 0) {
        GXSetTevSwapMode(stage, 0, swap - 0xF);
    }
}

void DEMOSetTevOp(GXTevStageID id, GXTevMode mode) {
    GXTevColorArg carg = GX_CC_RASC;
    GXTevAlphaArg aarg = GX_TEVSTAGE5;

    if (id != 0) {
        carg = 0;
        aarg = 0;
    }

    switch(mode) {
    case 0:
        DEMOSetTevColorIn(id, 0xF, 8, carg, 0xF);
        GXSetTevAlphaIn(id, 7, 4, aarg, 7);
        break;
    case 1:
        DEMOSetTevColorIn(id, carg, 8, 9, 0xF);
        GXSetTevAlphaIn(id, 7, 7, 7, aarg);
        break;
    case 2:
        DEMOSetTevColorIn(id, carg, 0xC, 8, 0xF);
        GXSetTevAlphaIn(id, 7, 4, aarg, 7);
        break;
    case 3:
        DEMOSetTevColorIn(id, 0xF, 0xF, 0xF, 8);
        GXSetTevAlphaIn(id, 7, 7, 7, 4);
        break;
    case 4:
        DEMOSetTevColorIn(id, 0xF, 0xF, 0xF, carg);
        GXSetTevAlphaIn(id, 7, 7, 7, aarg);
        break;
    default:
        ASSERTMSGLINE(914, FALSE, "DEMOSetTevOp: Invalid Tev Mode");
        break;
    }

    GXSetTevColorOp(id, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(id, 0, 0, 0, 1, 0);
}

GXRenderModeObj* DEMOGetRenderModeObj(void) {
    return rmode;
}

u32 DEMOGetCurrentBuffer(void) {
    return (u32)DemoCurrentBuffer;
}

void DEMOEnableGPHangWorkaround(u32 timeoutFrames) {
    if (timeoutFrames != 0) {
        ASSERTMSGLINE(989, !DemoStatEnable, "DEMOStats and GP hang diagnosis are mutually exclusive");
        GPHangWorkaround = 1;
        FrameMissThreshold = timeoutFrames;
        VISetPreRetraceCallback(__NoHangRetraceCallback);
        DEMOSetGPHangMetric(1);
    } else {
        GPHangWorkaround = 0;
        FrameMissThreshold = 0;
        DEMOSetGPHangMetric(0);
        VISetPreRetraceCallback(NULL);
    }
}

static void __NoHangRetraceCallback(u32 count) {
    static u32 ovFrameCount = 0;
    static u32 lastOvc = 0;
    u32 ovc;
    u8 overhi;
    u8 junk;

    FrameCount++;

    GXGetGPStatus(&overhi, &junk, &junk, &junk, &junk);
    ovc = GXGetOverflowCount();

    if (overhi && ovc == lastOvc) {
        ovFrameCount++;
        if (ovFrameCount >= FrameMissThreshold)  {
            OSReport("---------WARNING : HANG AT HIGH WATERMARK----------\n");
            __DEMODiagnoseHang();
            OSPanic(__FILE__, 1048, "Halting program");
        }
    } else {
        lastOvc = ovc;
        ovFrameCount = 0;
    }
}

static void __NoHangDoneRender(void) {
    BOOL abort = FALSE;

    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDrawSync(0xB00B);

    FrameCount = 0;

    while (GXReadDrawSync() != 0xB00B && !abort) {
        if (FrameCount >= FrameMissThreshold) {
            OSReport("---------WARNING : ABORTING FRAME----------\n");
            abort = TRUE;
            __DEMODiagnoseHang();
            DEMOReInit(rmode);
            DEMOSetGPHangMetric(1);
        }
    }

    DEMOSwapBuffers();
}

void DEMOSetGPHangMetric(u8 enable) {
    if (enable) {
        GXSetGPMetric(GX_PERF0_NONE, GX_PERF1_NONE);

        GXCmd1u8(0x61);
        GXParam1u32(0x2402C004);

        GXCmd1u8(0x61);
        GXParam1u32(0x23000020);
    
        GXCmd1u8(0x10);
        GXParam1u16(0);
        GXParam1u16(0x1006);
        GXParam1u32(0x84400);
    } else {
        GXCmd1u8(0x61);
        GXParam1u32(0x24000000);

        GXCmd1u8(0x61);
        GXParam1u32(0x23000000);

        GXCmd1u8(0x10);
        GXParam1u16(0);
        GXParam1u16(0x1006);
        GXParam1u32(0);
    }
}

static void __DEMODiagnoseHang(void) {
    u32 xfTop0, xfBot0, suRdy0, r0Rdy0;
    u32 xfTop1, xfBot1, suRdy1, r0Rdy1;
    u32 xfTopD, xfBotD, suRdyD, r0RdyD;
    u8 readIdle, cmdIdle;
    u8 junk;

    GXReadXfRasMetric(&xfBot0, &xfTop0, &r0Rdy0, &suRdy0);
    GXReadXfRasMetric(&xfBot1, &xfTop1, &r0Rdy1, &suRdy1);

    xfTopD = (xfTop1 - xfTop0) == 0;
    xfBotD = (xfBot1 - xfBot0) == 0;
    suRdyD = (suRdy1 - suRdy0) > 0;
    r0RdyD = (r0Rdy1 - r0Rdy0) > 0;

    GXGetGPStatus(&junk, &junk, &readIdle, &cmdIdle, &junk);
    OSReport("GP status %d%d%d%d%d%d --> ", readIdle, cmdIdle, xfTopD, xfBotD, suRdyD, r0RdyD);

    if (xfBotD == 0 && suRdyD != 0) {
        OSReport("GP hang due to XF stall bug.\n");
    } else if (xfTopD == 0 && xfBotD != 0 && suRdyD != 0) {
        OSReport("GP hang due to unterminated primitive.\n");
    } else if (cmdIdle == 0 && xfTopD != 0 && xfBotD != 0 && suRdyD != 0) {
        OSReport("GP hang due to illegal instruction.\n");
    } else if (readIdle != 0 && cmdIdle != 0 && xfTopD != 0 && xfBotD != 0 && suRdyD != 0 && r0RdyD != 0) {
        OSReport("GP appears to be not hung (waiting for input).\n");
    } else {
        OSReport("GP is in unknown state.\n");
    }
}

void DEMOReInit(GXRenderModeObj* mode) {
    u32 fbSize;
    GXFifoObj tmpobj;
    void* tmpFifo;
    GXFifoObj* realFifoObj;
    void* realFifoBase;
    u32 realFifoSize;

    tmpFifo = OSAlloc(64 * 1024);

    realFifoObj = GXGetCPUFifo();
    realFifoBase = GXGetFifoBase(realFifoObj);
    realFifoSize = GXGetFifoSize(realFifoObj);

    GXAbortFrame();
  
    GXInitFifoBase(&tmpobj, tmpFifo, 64 * 1024);
    
    GXSetCPUFifo(&tmpobj);
    GXSetGPFifo(&tmpobj);    

    __DEMOInitRenderMode(mode);

    fbSize = ((u16)(rmode->fbWidth + 15) & ~0xF) * rmode->xfbHeight * 2;
    ASSERTMSGLINE(1260, fbSize <= allocatedFrameBufferSize, "DEMOReInit - Previously allocated frame buffer is too small for the new render mode.");
    DefaultFifoObj = GXInit(realFifoBase, realFifoSize);

    __DEMOInitGX();
    VIConfigure(rmode);
    __DEMOInitVI();
    OSFree(tmpFifo);
}
