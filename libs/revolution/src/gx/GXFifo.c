#include <revolution/base/PPCArch.h>
#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

#define TOPHYSICAL(a) (((u32)a) & 0x3FFFFFFF)

static __GXFifoObj CPUFifo;
static __GXFifoObj GPFifo;
static GXBool CPUFifoReady = FALSE;
static GXBool GPFifoReady = FALSE;

static GXBool CPGPLinked;
static OSThread* __GXCurrentThread;
static BOOL GXOverflowSuspendInProgress;
static GXBreakPtCallback BreakPointCB;
void* __GXCurrentBP;
static u32 __GXOverflowCount;

#if DEBUG
static BOOL IsWGPipeRedirected;
#endif

static void __GXFifoReadEnable(void);
static void __GXFifoReadDisable(void);
static void __GXFifoLink(u8 en);
static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn);
static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr);
GXBool CPGPLinkCheck(void);

static void GXOverflowHandler(__OSInterrupt interrupt, OSContext* context) {
#if DEBUG
    if (__gxVerif->verifyLevel > GX_WARN_SEVERE) {
        OSReport("[GXOverflowHandler]");
    }
#endif
    ASSERTLINE(417, !GXOverflowSuspendInProgress);

    __GXOverflowCount++;
    __GXWriteFifoIntEnable(0, 1);
    __GXWriteFifoIntReset(1, 0);
    GXOverflowSuspendInProgress = TRUE;

#if DEBUG
    if (__gxVerif->verifyLevel > GX_WARN_SEVERE) {
        OSReport("[GXOverflowHandler Sleeping]");
    }
#endif
    OSSuspendThread(__GXCurrentThread);
}

static void GXUnderflowHandler(s16 interrupt, OSContext* context) {
#if DEBUG
    if (__gxVerif->verifyLevel > GX_WARN_SEVERE) {
        OSReport("[GXUnderflowHandler]");
    }
#endif
    ASSERTLINE(459, GXOverflowSuspendInProgress);

    OSResumeThread(__GXCurrentThread);
    GXOverflowSuspendInProgress = FALSE;
    __GXWriteFifoIntReset(1, 1);
    __GXWriteFifoIntEnable(1, 0);
}

static void GXBreakPointHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;

    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, __GXData->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    if (BreakPointCB != 0) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        BreakPointCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void GXCPInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    __GXData->cpStatus = GX_GET_CP_REG(0);
    if (CP_REG_ENABLE_GET_UNFLINT(__GXData->cpEnable) && CP_REG_STATUS_GET_UNFL(__GXData->cpStatus)) {
        GXUnderflowHandler(interrupt, context);
    }

    if (CP_REG_ENABLE_GET_OVFLINT(__GXData->cpEnable) && CP_REG_STATUS_GET_OVFL(__GXData->cpStatus)) {
        GXOverflowHandler(interrupt, context);
    }

    if (CP_REG_ENABLE_GET_FIFOBRKINT(__GXData->cpEnable) && CP_REG_STATUS_GET_FIFOBRK(__GXData->cpStatus)) {
        GXBreakPointHandler(interrupt, context);
    }
}

void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;

    ASSERTMSGLINE(582, ((u32)base & 0x1F) == 0, "GXInitFifoBase: base must be 32B aligned");
    ASSERTMSGLINE(584, base != 0,               "GXInitFifoBase: base pointer is NULL");
    ASSERTMSGLINE(586, (size & 0x1F) == 0,      "GXInitFifoBase: size must be 32B aligned");
    ASSERTMSGLINE(588, size >= 0x10000,         "GXInitFifoBase: fifo is not large enough");

    realFifo->base = base;
    realFifo->top = (u8*)base + size - 4;
    realFifo->size = size;
    realFifo->count = 0;
    GXInitFifoLimits(fifo, size - 0x4000, (size >> 1) & ~0x1F);
    GXInitFifoPtrs(fifo, base, base);
}

void GXInitFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr) {
    __GXFifoObj* realFifo = (__GXFifoObj *)fifo;
    BOOL enabled;

    ASSERTMSGLINE(628, ((u32)readPtr & 0x1F) == 0,  "GXInitFifoPtrs: readPtr not 32B aligned");
    ASSERTMSGLINE(630, ((u32)writePtr & 0x1F) == 0, "GXInitFifoPtrs: writePtr not 32B aligned");
    ASSERTMSGLINE(633, realFifo->base <= readPtr && readPtr < realFifo->top,   "GXInitFifoPtrs: readPtr not in fifo range");
    ASSERTMSGLINE(636, realFifo->base <= writePtr && writePtr < realFifo->top, "GXInitFifoPtrs: writePtr not in fifo range");

    enabled = OSDisableInterrupts();
    realFifo->rdPtr = readPtr;
    realFifo->wrPtr = writePtr;
    realFifo->count = (u8*)writePtr - (u8*)readPtr;
    if (realFifo->count < 0) {
        realFifo->count += realFifo->size;
    }
    OSRestoreInterrupts(enabled);
}

void GXInitFifoLimits(GXFifoObj* fifo, u32 hiWatermark, u32 loWatermark) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;

    ASSERTMSGLINE(674, (hiWatermark & 0x1F) == 0, "GXInitFifoLimits: hiWatermark not 32B aligned");
    ASSERTMSGLINE(676, (loWatermark & 0x1F) == 0, "GXInitFifoLimits: loWatermark not 32B aligned");
    ASSERTMSGLINE(678, hiWatermark < realFifo->top - realFifo->base, "GXInitFifoLimits: hiWatermark too large");
    ASSERTMSGLINE(680, loWatermark < hiWatermark, "GXInitFifoLimits: hiWatermark below lo watermark");

    realFifo->hiWatermark = hiWatermark;
    realFifo->loWatermark = loWatermark;
}

GXBool CPGPLinkCheck(void) {
    u32 check = 0;
    s32 range1, range2, overlap;

    if (!CPUFifoReady || !GPFifoReady) {
        return GX_FALSE;
    }

    if (CPUFifo.base == GPFifo.base) {
        ++check;
    }

    if (CPUFifo.top == GPFifo.top) {
        ++check;
    }

    if (check == 2) {
        if (CPUFifo.hiWatermark == GPFifo.hiWatermark) {
            ++check;
        }

        if (CPUFifo.loWatermark == GPFifo.loWatermark) {
            ++check;
        }

        ASSERTMSGLINE(722, check == 4, "GXSetGPFifo/GXSetCPUFifo: mismatch of watermark parameter");
        return GX_TRUE;
    }
    
    range1 = (s32)((u8*)CPUFifo.top - (u8*)GPFifo.base);
    range2 = (s32)((u8*)GPFifo.top - (u8*)CPUFifo.base);
    overlap = ((range1 > 0) && (range2 > 0) || (range1 < 0 && (range2 < 0)));

    if (overlap) {
        OSReport("CPUFifo: %08X - %08X\n", (u32)CPUFifo.base, (u32)CPUFifo.top);
        OSReport("GP Fifo: %08X - %08X\n", (u32)GPFifo.base, (u32)GPFifo.top);
    }

    ASSERTMSGLINE(736, !overlap, "GXSetGPFifo/GXSetCPUFifo: invalid overlap of both buffers");
    return GX_FALSE;
}

// NONMATCHING DEBUG
void GXSetCPUFifo(GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    BOOL enabled = OSDisableInterrupts();
    u32 reg;

    if (fifo == NULL) {
        CPUFifoReady = GX_FALSE;
        CPGPLinked = GX_FALSE;
        CPUFifo.bind_cpu = CPUFifo.bind_gp = GX_FALSE;
        OSRestoreInterrupts(enabled);
        return;
    }

    CPUFifo = *realFifo;
    CPUFifoReady = GX_TRUE;
    CPUFifo.bind_cpu = GX_TRUE;

    if (CPGPLinkCheck()) {
        CPGPLinked = GX_TRUE;
        CPUFifo.bind_gp = GX_TRUE;

        reg = 0;

        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base));
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.top));

        SC_PI_CPWRT_REG_SET_CPWRT(787, reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(0, reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);

        __GXWriteFifoIntReset(1, 1);
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    } else {
        CPUFifo.bind_gp = GX_FALSE;

        if (CPGPLinked) {
            __GXFifoLink(0);
            CPGPLinked = GX_FALSE;
        }

        __GXWriteFifoIntEnable(0, 0);
        reg = 0;
        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base));
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.top));  
        SC_PI_CPWRT_REG_SET_CPWRT(819, reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(0, reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
    }

    PPCSync();
    OSRestoreInterrupts(enabled);
}

void GXSetGPFifo(GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    BOOL enabled;
    u32 stbtmp;

    ASSERTMSGLINE(850, ((u32)realFifo->base & 0x30000000) == 0, "GXSetGPFifo: GX Fifo should be located on MEM1 memory region");

    enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(0, 0);

    if (fifo == NULL) {
        GPFifoReady = GX_FALSE;
        CPGPLinked = GX_FALSE;
        __GXFifoLink(GX_FALSE);
        GPFifo.bind_gp = GPFifo.bind_cpu = GX_FALSE;
        OSRestoreInterrupts(enabled);
        return;
    }

    GPFifo = *realFifo;
    GPFifoReady = GX_TRUE;
    GPFifo.bind_gp = GX_TRUE;
    GX_CP_REG_WRITE_U16(CP_FIFO_BASEL, TOPHYSICAL(GPFifo.base));
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPL, TOPHYSICAL(GPFifo.top));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, TOPHYSICAL(GPFifo.wrPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, TOPHYSICAL(GPFifo.rdPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTL, GPFifo.hiWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTL, GPFifo.loWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_BASEH, TOPHYSICAL(GPFifo.base) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPH, TOPHYSICAL(GPFifo.top) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, GPFifo.count >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, TOPHYSICAL(GPFifo.wrPtr) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, TOPHYSICAL(GPFifo.rdPtr) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTH, GPFifo.hiWatermark >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTH, GPFifo.loWatermark >> 16);

    PPCSync();

    if (CPGPLinkCheck()) {
        CPGPLinked = GX_TRUE;
        GPFifo.bind_cpu = GX_TRUE;
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    } else {
        CPGPLinked = GX_FALSE;
        GPFifo.bind_cpu = GX_FALSE;
        __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
        __GXFifoLink(GX_FALSE);
    }

    stbtmp = __GXData->cpEnable;
    SC_CP_REG_ENABLE_SET_FIFOBRK(0, stbtmp, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, stbtmp, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, stbtmp);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

#define CP_READ_2U16(dest,reg,type) \
{\
    u32 temp;                                 \
    temp =  (u32)GX_CP_REG_READ_U16(reg##H)<<16;   \
    temp |= (u32)GX_CP_REG_READ_U16(reg##L);       \
    dest = (type)OSPhysicalToCached(temp);   \
}

#define CP_READ_2U16_INT(dest,reg,type) \
{\
    u32 temp;                                 \
    temp =  (u32)GX_CP_REG_READ_U16(reg##H)<<16;   \
    temp |= (u32)GX_CP_REG_READ_U16(reg##L);       \
    dest = (type)temp;   \
}

static void __GXSaveFifoCPStat(void) {
    CP_READ_2U16(GPFifo.rdPtr, CP_FIFO_RPTR, void*);
    CP_READ_2U16_INT(GPFifo.count, CP_FIFO_COUNT, s32);
}

static void __GXSaveFifoPIStat(void) {
    u32 reg = GX_PI_REG_READ_U32(PI_REG_CPWRT);
    CPUFifo.wrPtr = OSPhysicalToCached(reg & PI_CPWRT_REG_CPWRT_MASK);
    CPUFifo.wrap = (GXBool)((reg & PI_CPWRT_REG_CPWRAP_MASK) ? GX_TRUE : GX_FALSE);
}

void __GXSaveFifo(void) {
    BOOL enabled = OSDisableInterrupts();

    if (CPUFifoReady) {
        __GXSaveFifoPIStat();
    }

    if (GPFifoReady) {
        __GXSaveFifoCPStat();
    }

    if (CPGPLinked) {
        CPUFifo.rdPtr = GPFifo.rdPtr;
        CPUFifo.count = GPFifo.count;
        GPFifo.wrPtr = CPUFifo.wrPtr;
        GPFifo.wrap = CPUFifo.wrap;
    } else if (CPUFifoReady) {
        CPUFifo.count  = (s32)CPUFifo.wrPtr - (s32)CPUFifo.rdPtr;
        if (CPUFifo.count < 0) {
            CPUFifo.count += CPUFifo.size;
        }
    }

    OSRestoreInterrupts(enabled);
}

GXBool __GXIsGPFifoReady(void) {
    return GPFifoReady;
}

void GXGetGPStatus(GXBool* overhi, GXBool* underlow, GXBool* readIdle, GXBool* cmdIdle, GXBool* brkpt) {
    __GXData->cpStatus = GX_CP_REG_READ_U16(CP_STATUS);
    *overhi = (u8)CP_REG_STATUS_GET_OVFL(__GXData->cpStatus);
    *underlow = (u8)CP_REG_STATUS_GET_UNFL(__GXData->cpStatus);
    *readIdle = (u8)CP_REG_STATUS_GET_FIFO_RDIDLE(__GXData->cpStatus);
    *cmdIdle = (u8)CP_REG_STATUS_GET_CPIDLE(__GXData->cpStatus);
    *brkpt = (u8)CP_REG_STATUS_GET_FIFOBRK(__GXData->cpStatus);
}

void GXGetFifoPtrs(GXFifoObj* fifo, void** readPtr, void** writePtr) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    *readPtr = realFifo->rdPtr;
    *writePtr = realFifo->wrPtr;
}

void* GXGetFifoBase(const GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;

    return realFifo->base;
}

u32 GXGetFifoSize(const GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    return realFifo->size;
}

void GXGetFifoLimits(const GXFifoObj* fifo, u32* hi, u32* lo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    *hi = realFifo->hiWatermark;
    *lo = realFifo->loWatermark;
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
    GXBreakPtCallback oldcb = BreakPointCB;
    BOOL enabled = OSDisableInterrupts();

    BreakPointCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

void* __GXCurrentBP;

void GXEnableBreakPt(void* break_pt) {
    BOOL enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKL, TOPHYSICAL(break_pt));
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKH, TOPHYSICAL(break_pt) >> 16);
    SC_CP_REG_ENABLE_SET_FIFOBRK(0, __GXData->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, __GXData->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    SC_CP_REG_ENABLE_SET_FIFOBRK(0, __GXData->cpEnable, 1);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, __GXData->cpEnable, 1);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    __GXCurrentBP = break_pt;
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void GXDisableBreakPt(void) {
    BOOL enabled = OSDisableInterrupts();

    SC_CP_REG_ENABLE_SET_FIFOBRK(0, __GXData->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, __GXData->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    __GXCurrentBP = NULL;
    OSRestoreInterrupts(enabled);
}

void __GXFifoInit(void) {
    __OSSetInterruptHandler(0x11, GXCPInterruptHandler);
    __OSUnmaskInterrupts(0x4000);
    __GXCurrentThread = OSGetCurrentThread();
    GXOverflowSuspendInProgress = FALSE;
    memset(&CPUFifo, 0, sizeof(__GXFifoObj));
    memset(&GPFifo, 0, sizeof(__GXFifoObj));
    CPUFifoReady = GX_FALSE;
    GPFifoReady  = GX_FALSE;
}

static void __GXFifoReadEnable(void) {
    SC_CP_REG_ENABLE_SET_FIFORD(0, __GXData->cpEnable, 1); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)__GXData->cpEnable);
}

static void __GXFifoReadDisable(void) {
    SC_CP_REG_ENABLE_SET_FIFORD(0, __GXData->cpEnable, 0); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)__GXData->cpEnable);
}

static void __GXFifoLink(u8 en) {
    SC_CP_REG_ENABLE_SET_WRPTRINC(1385, __GXData->cpEnable, (en ? 0x1 : 0)); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)__GXData->cpEnable);
}

static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn) {
    SC_CP_REG_ENABLE_SET_OVFLINT(1407, __GXData->cpEnable, hiWatermarkEn);
    SC_CP_REG_ENABLE_SET_UNFLINT(1408, __GXData->cpEnable, loWatermarkEn);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)__GXData->cpEnable);
}

static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr) {
    SC_CP_REG_CLR_SET_OVFLINT(1431, __GXData->cpClr, hiWatermarkClr);
    SC_CP_REG_CLR_SET_UNFLINT(1432, __GXData->cpClr, loWatermarkClr);
    GX_CP_REG_WRITE_U16(CP_CLR, (u16)__GXData->cpClr);
}

void __GXCleanGPFifo(void) {
    BOOL enabled;

    if ( !GPFifoReady) {
        return;
    }

    enabled = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
    GPFifo.rdPtr = GPFifo.wrPtr;
    GPFifo.count = 0;
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, TOPHYSICAL(GPFifo.wrPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, TOPHYSICAL(GPFifo.rdPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, GPFifo.count>>16);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, TOPHYSICAL(GPFifo.wrPtr)>>16);
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, TOPHYSICAL(GPFifo.rdPtr)>>16);

    PPCSync();

    if (CPGPLinked) {
        u32 reg = 0;
        CPUFifo.rdPtr = GPFifo.rdPtr;
        CPUFifo.wrPtr = GPFifo.wrPtr;
        CPUFifo.count = GPFifo.count;
        SC_PI_CPWRT_REG_SET_CPWRT(1501, reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(0, reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }

    SC_CP_REG_ENABLE_SET_FIFOBRK(0, __GXData->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(0, __GXData->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, __GXData->cpEnable);
    __GXCurrentBP = 0;
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

OSThread* GXSetCurrentGXThread(void) {
    BOOL enabled;
    OSThread* prev;

    enabled = OSDisableInterrupts();
    prev = __GXCurrentThread;
    ASSERTMSGLINE(1541, !GXOverflowSuspendInProgress, "GXSetCurrentGXThread: Two threads cannot generate GX commands at the same time!");
    __GXCurrentThread = OSGetCurrentThread();
    OSRestoreInterrupts(enabled);
    return prev;
}

OSThread* GXGetCurrentGXThread(void) {
    return __GXCurrentThread;
}

GXFifoObj* GXGetCPUFifo(void) {
    return (GXFifoObj*)&CPUFifo;
}

GXFifoObj* GXGetGPFifo(void) {
    return (GXFifoObj*)&GPFifo;
}

u32 GXGetOverflowCount(void) {
    return __GXOverflowCount;
}

u32 GXResetOverflowCount(void) {
    u32 oldcount;

    oldcount = __GXOverflowCount;
    __GXOverflowCount = 0;
    return oldcount;
}

// NONMATCHING
volatile void* GXRedirectWriteGatherPipe(void* ptr) {
    u32 reg = 0;
    BOOL enabled = OSDisableInterrupts();

    CHECK_GXBEGIN(0, "GXRedirectWriteGatherPipe");
    ASSERTLINE(0, OFFSET(ptr, 32) == 0);
    ASSERTLINE(0, !IsWGPipeRedirected);

#if DEBUG
    IsWGPipeRedirected = TRUE;
#endif

    GXFlush();
    while (PPCMfwpar() & 1) {}

    PPCMtwpar((u32)OSUncachedToPhysical((void*)GXFIFO_ADDR));

    if (CPGPLinked) {
        __GXFifoLink(0);
        __GXWriteFifoIntEnable(0, 0);
    }

    CPUFifo.wrPtr = OSPhysicalToCached(GX_GET_PI_REG(5) & 0xFBFFFFFF);
    GX_SET_PI_REG(3, 0);
    GX_SET_PI_REG(4, 0x04000000);
    SET_REG_FIELD(0, reg, 21, 5, ((u32)ptr & 0x3FFFFFFF) >> 5);
    reg &= 0xFBFFFFFF;
    GX_SET_PI_REG(5, reg);

    PPCSync();
    OSRestoreInterrupts(enabled);
    return (volatile void *)GXFIFO_ADDR;
}

// NONMATCHING
void GXRestoreWriteGatherPipe(void) {
    u32 reg = 0;
    u32 i;
    BOOL enabled;

    ASSERTLINE(1552, IsWGPipeRedirected);

#if DEBUG
    IsWGPipeRedirected = FALSE;
#endif

    enabled = OSDisableInterrupts();
    for (i = 0; i < 31; i++) {
        GXWGFifo.u8 = 0;
    }

    PPCSync();
    while (PPCMfwpar() & 1) {}
    PPCMtwpar((u32)OSUncachedToPhysical((void *)GXFIFO_ADDR));
    GX_SET_PI_REG(3, (u32)CPUFifo.base & 0x3FFFFFFF);
    GX_SET_PI_REG(4, (u32)CPUFifo.top & 0x3FFFFFFF);
    SET_REG_FIELD(1578, reg, 21, 5, ((u32)CPUFifo.wrPtr & 0x3FFFFFFF) >> 5);
    reg &= 0xFBFFFFFF;
    GX_SET_PI_REG(5, reg);
    if (CPGPLinked) {
        __GXWriteFifoIntReset(1, 1);
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    }

    PPCSync();
    OSRestoreInterrupts(enabled);
}
