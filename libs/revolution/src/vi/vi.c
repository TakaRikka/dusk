#include <revolution/gx.h>
#include <revolution/os.h>
#include <revolution/vi.h>
#include <revolution/si.h>
#include <revolution/sc.h>
#include <revolution/hw_regs.h>

#include "__gx.h"
#include "__os.h"
#include "__vi.h"

// normal assert macros don't match in some places here
#if DEBUG
#define VI_ASSERTMSGLINE1(line, cond, msg, arg1) \
    if (!(cond)) \
        OSPanic(__FILE__, line, msg, arg1)

#define VI_ASSERTMSGLINE2(line, cond, msg, arg1, arg2) \
    if (!(cond)) \
        OSPanic(__FILE__, line, msg, arg1, arg2)
#else
#define VI_ASSERTMSGLINE1(line, cond, msg, arg1) (void)0
#define VI_ASSERTMSGLINE2(line, cond, msg, arg1, arg2) (void)0
#endif

// extern
extern DVDCommandBlock __DVDStopMotorCommandBlock;

#ifdef SDK_AUG2010
#define BUILD_DATE "Aug 23 2010"
#if DEBUG
#define BUILD_TIME "17:27:58"
#else
#define BUILD_TIME "17:33:06"
#endif
#elif SDK_SEP2006
#define BUILD_DATE  "Sep 21 2006"
#define BUILD_TIME "14:32:13"
#endif

#ifdef SDK_AUG2010
#if DEBUG
const char* __VIVersion = "<< RVL_SDK - VI \tdebug build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#else
const char* __VIVersion = "<< RVL_SDK - VI \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#endif
#elif SDK_SEP2006
const char* __VIVersion = "<< RVL_SDK - VI \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4200_60422) >>";
#endif

typedef struct {
    u8 equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8 bs1;
    u8 bs2;
    u8 bs3;
    u8 bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8 hsy;
    u8 hcs;
    u8 hce;
    u8 hbe640;
    u16 hbs640;
    u8 hbeCCIR656;
    u16 hbsCCIR656;
} timing_s;

typedef struct {
    u16 DispPosX;
    u16 DispPosY;
    u16 DispSizeX;
    u16 DispSizeY;
    u16 AdjustedDispPosX;
    u16 AdjustedDispPosY;
    u16 AdjustedDispSizeY;
    u16 AdjustedPanPosY;
    u16 AdjustedPanSizeY;
    u16 FBSizeX;
    u16 FBSizeY;
    u16 PanPosX;
    u16 PanPosY;
    u16 PanSizeX;
    u16 PanSizeY;
    VIXFBMode FBMode;
    u32 nonInter;
    u32 tv;
    u8 wordPerLine;
    u8 std;
    u8 wpl;
    u32 bufAddr;
    u32 tfbb;
    u32 bfbb;
    u8 xof;
    BOOL black;
    BOOL threeD;
    u32 rbufAddr;
    u32 rtfbb;
    u32 rbfbb;
    timing_s* timing;
} SomeVIStruct;

static BOOL IsInitialized = FALSE;

static volatile u32 retraceCount;
static volatile u32 flushFlag;
static volatile u32 flushFlag3in1;
static volatile u32 vsync_timing_err_cnt = 0;
static volatile u32 vsync_timing_test_flag = 0;

static volatile BOOL __VIDimming_All_Clear = FALSE;
static volatile BOOL __VIDimmingFlag_Enable;
static volatile BOOL __VIDVDStopFlag_Enable;
static volatile VITimeToDIM g_current_time_to_dim;
static vu32 THD_TIME_TO_DIMMING = 0;
static vu32 NEW_TIME_TO_DIMMING = 0;
static vu32 THD_TIME_TO_DVD_STOP = 0; 
static vu32 _gIdleCount_dimming = 0; 
static vu32 _gIdleCount_dvd = 0; 
static vu32 __VIDimmingFlag_RF_IDLE;
static vu32 __VIDimmingFlag_SI_IDLE;
static vu32 __VIDimmingFlag_DEV_IDLE[10];
static volatile BOOL __VIDimmingState = FALSE;

extern VIVideo Vdac_Flag_Region;
extern volatile u32 Vdac_Flag_Changed;

static OSThreadQueue retraceQueue;

static void (*PreCB)(u32);
static void (*PostCB)(u32);

static u32 encoderType;
static u16 regs[59];
static volatile u32 __VIDimmingFlag_DEV_IDLE[10];
static timing_s* CurrTiming;
static u32 CurrTvMode;
static u32 NextBufAddr;
static u32 CurrBufAddr;
static u16 shdwRegs[59];

static void (*PositionCallback)(s16, s16) = NULL;
static s16 displayOffsetH = 0;
static s16 displayOffsetV = 0;
static volatile u32 changeMode = 0;
static volatile u64 changed = 0;
static volatile u32 shdwChangeMode = 0;
static volatile u64 shdwChanged = 0;
static u32 FBSet = 0;
static timing_s* timingExtra = NULL;

#define MARK_CHANGED(index) (changed |= 1LL << (63 - (index)))

static timing_s timing[11] = {
    { 6, 240, 24, 25, 3, 2, 12, 13, 12, 13, 520, 519, 520, 519, 525, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 12, 12, 12, 12, 520, 520, 520, 520, 526, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 5, 287, 35, 36, 1, 0, 13, 12, 11, 10, 619, 618, 617, 620, 625, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 5, 287, 33, 33, 2, 2, 13, 11, 13, 11, 619, 621, 619, 621, 624, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 6, 240, 24, 25, 3, 2, 16, 15, 14, 13, 518, 517, 516, 519, 525, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 16, 14, 16, 14, 518, 520, 518, 520, 526, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 12, 480, 48, 48, 6, 6, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 12, 480, 44, 44, 10, 10, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 168, 379, 122, 412 },
    { 6, 241, 24, 25, 1, 0, 12, 13, 12, 13, 520, 519, 520, 519, 525, 429, 64, 71, 105, 159, 370, 122, 412 },
    { 12, 480, 48, 48, 6, 6, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 180, 391, 122, 412 },
    { 10, 576, 62, 62, 6, 6, 20, 20, 20, 20, 1240, 1240, 1240, 1240, 1250, 432, 64, 75, 106, 172, 380, 122, 412 }
};

static u16 taps[25] = {
    0x01F0, 0x01DC,
    0x01AE, 0x0174,
    0x0129, 0x00DB,
    0x008E, 0x0046,
    0x000C, 0x00E2,
    0x00CB, 0x00C0,
    0x00C4, 0x00CF,
    0x00DE, 0x00EC,
    0x00FC, 0x0008,
    0x000F, 0x0013,
    0x0013, 0x000F,
    0x000C, 0x0008,
    0x0001
};

GXRenderModeObj GXPal528Prog = {
    6,
    640, 528, 528,
    40, 23,
    640, 528,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXPal528ProgSoft = {
    6,
    640, 528, 528,
    40, 23,
    640, 528,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXPal524ProgAa = {
    6,
    640, 264, 524,
    40, 23,
    640, 524,
    0,
    0,
    1,
    { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 },
    { 4, 8, 12, 16, 12, 8, 4 }
};

static BOOL OnShutdown(BOOL final, u32 event);
static OSShutdownFunctionInfo ShutdownFunctionInfo = {
    OnShutdown,
    127
};

static SomeVIStruct HorVer;


// prototypes
static u32 getCurrentFieldEvenOdd(void);
timing_s* __VISetExtraTiming(timing_s* t);
void __VIEnableRawPositionInterrupt(s16 x, s16 y, void (*callback)(s16, s16));
void (*__VIDisableRawPositionInterrupt())(s16, s16);
void __VIDisplayPositionToXY(u32 hct, u32 vct, s16* x, s16* y);
void __VISetLatchMode(u32 mode);
int __VIGetLatch0Position(s16* px, s16* py);
int __VIGetLatch1Position(s16* px, s16* py);
int __VIGetLatchPosition(u32 port, s16* px, s16* py);
static void GetCurrentDisplayPosition(u32* hct, u32* vct);

static BOOL OnShutdown(BOOL final, u32 event) {
    BOOL retval;
    static BOOL first = TRUE;
    static u32 count;

    if (final == FALSE) {
        switch (event) {
        case 3:
        case 1:
        case 2:
            if (first) {
                VISetRGBModeImm();
                VIFlush();
                count = retraceCount;
                first = FALSE;   
                retval = FALSE;
            } else {
                if (count == retraceCount) {
                    retval = FALSE;
                } else {
                    retval = TRUE;
                }
            }
            break;
        case 4:
        case 0:
        case 6:
        case 5:
            __VISetGamma1_0();
            retval = TRUE;
            break;
        }
    } else {
        retval = TRUE;
    }

    return retval;
}

static u32 getEncoderType(void) {
    return 1;
}

static s32 cntlzd(u64 bit) {
    u32 hi;
    u32 lo;
    s32 value;

    hi = bit >> 32;
    lo = bit & 0xFFFFFFFF;
    value = __cntlzw(hi);
    if (value < 32) {
        return value;
    }
    return __cntlzw(lo) + 32;
}

static int VISetRegs(void) {
    s32 regIndex;

    if (shdwChangeMode != 1 || getCurrentFieldEvenOdd() != 0) {
        while (shdwChanged != 0) {
            regIndex = cntlzd(shdwChanged);
            __VIRegs[regIndex] = shdwRegs[regIndex];
            shdwChanged &= ~((u64)1 << (63 - regIndex));
        }

        shdwChangeMode = 0;
        CurrTiming = HorVer.timing;
        CurrTvMode = HorVer.tv;
        CurrBufAddr = NextBufAddr;
        return 1;
    }

    return 0;
}

static void __VIRetraceHandler(__OSInterrupt unused, OSContext* context) {
    OSContext exceptionContext;
    u16 reg;
    u32 inter = 0;
    u32 regIndex;
    static u32 old_dtvStatus = 999;
    static u32 old_tvtype = 999;
    u32 now_dtvStatus = 0;
    u32 now_tvtype = 0;
    static BOOL __VIDimmingFlag_Enable_old = TRUE;
    static BOOL __VIDVDStopFlag_Enable_old = TRUE;
    u32 i;

    #if DEBUG
    static u32 dbgCount;
    #endif
    static u32 DimmingON_Pending = 0;
    static u32 DimmingOFF_Pending = 0;


    reg = __VIRegs[0x18];
    if (reg & 0x8000) {
        __VIRegs[0x18] = reg & ~0x8000;
        inter |= 1;
    }
    reg = __VIRegs[0x1A];
    if (reg & 0x8000) {
        __VIRegs[0x1A] = reg & ~0x8000;
        inter |= 2;
    }
    reg = __VIRegs[0x1C];
    if (reg & 0x8000) {
        __VIRegs[0x1C] = reg & ~0x8000;
        inter |= 4;
    }
    reg = __VIRegs[0x1E];
    if (reg & 0x8000) {
        __VIRegs[0x1E] = reg & ~0x8000;
        inter |= 8;
    }
    reg = __VIRegs[0x1E];

    if ((inter & 4) || (inter & 8)) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);

        if (PositionCallback != 0) {
            s16 x, y;
            __VIGetCurrentPosition(&x, &y);
            (*PositionCallback)(x, y);
        }

        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
        return;
    }

    if (inter == 0) {
        ASSERTLINE(1350, FALSE);
    }

    retraceCount += 1;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);

    if (PreCB) {
        PreCB(retraceCount);
    }

    if (vsync_timing_test_flag) {
        u32 hcount, vcount;

        GetCurrentDisplayPosition(&hcount, &vcount);

        if(!((vcount == 1) || (vcount == (CurrTiming->nhlines / 2 + 1)))) {
            vsync_timing_err_cnt++;
        }
    }

    if (flushFlag != 0) {
#if DEBUG
        dbgCount = 0;
#endif
        if (VISetRegs() != 0) {
            flushFlag = 0;
            SIRefreshSamplingRate();
        }
    }

    now_dtvStatus = VIGetDTVStatus();
    if(now_dtvStatus != old_dtvStatus) {
        __VISetYUVSEL((VIBool)now_dtvStatus);
    }

    old_dtvStatus = now_dtvStatus;
    now_tvtype = VIGetTvFormat();

    if (now_tvtype != old_tvtype) {
        if (now_tvtype == VI_EURGB60) {
            __VISetFilter4EURGB60(VI_ENABLE);
        }
        else {
            __VISetFilter4EURGB60(VI_DISABLE);
        }
        
        switch (now_tvtype) {
        case VI_PAL:
            switch(g_current_time_to_dim) {
            case VI_DM_10M:
                NEW_TIME_TO_DIMMING = 30000;
                break;
            case VI_DM_15M:
                NEW_TIME_TO_DIMMING = 45000;
                break;
            default:
                NEW_TIME_TO_DIMMING = 15000;
                break;
            }
            THD_TIME_TO_DVD_STOP = 90000;
            break;
        default:
            switch(g_current_time_to_dim) {
            case VI_DM_10M:
                NEW_TIME_TO_DIMMING = 36000;
                break;
            case VI_DM_15M:
                NEW_TIME_TO_DIMMING = 54000;
                break;
            default:
                NEW_TIME_TO_DIMMING = 18000;
                break;
            }
            THD_TIME_TO_DVD_STOP = 108000;
            break;
        }
    
        _gIdleCount_dimming = 0;
        _gIdleCount_dvd = 0;
    }

    old_tvtype = now_tvtype;
    
    if (flushFlag3in1) {
        while (Vdac_Flag_Changed) {
            regIndex = (u32)__cntlzw(Vdac_Flag_Changed);
            regIndex = (u32)(1 << (31 - regIndex));
            
            switch(regIndex) {
            case 1:
                __VISetCGMS();
                break;
            case 2:
                __VISetWSS();
                break;
            case 4:
                __VISetClosedCaption();
                break;
            case 8:
                __VISetMacrovision();
                break;
            case 0x10:
                __VISetGamma();
                break;
            case 0x20:
                __VISetTrapFilter();
                break;
            case 0x40:
                __VISetRGBOverDrive();
                break;
            case 0x80:
                __VISetRGBModeImm();
                break;
            }

            Vdac_Flag_Changed &= ~regIndex;
        }

        flushFlag3in1 = 0;
    }
    #if DEBUG
    else if (changed != 0) {
        dbgCount++;
        if (dbgCount > 60) {
            OSReport("Warning: VIFlush() was not called for 60 frames although VI settings were changed\n");
            dbgCount = 0;
        }
    }
    #endif

    if (PostCB) {
        OSClearContext(&exceptionContext);
        PostCB(retraceCount);
    }

    OSWakeupThread(&retraceQueue);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);

    if (__VIDimming_All_Clear == TRUE) {
        if(__OSSetVIForceDimming(FALSE, 0, 0) == TRUE) {
            __VIDimming_All_Clear = FALSE;
            _gIdleCount_dimming = 0;
        }
    }
    
    for (i = 0; i < 10; i++) {
        if(__VIDimmingFlag_DEV_IDLE[i] == 0) {
            __VIDimmingFlag_DEV_IDLE[0] = 0;
            break;
        }
    }
    
    if(__VIDimmingFlag_RF_IDLE && __VIDimmingFlag_SI_IDLE && __VIDimmingFlag_DEV_IDLE[0]) {
        if ((__VIDimmingFlag_Enable == TRUE) && (_gIdleCount_dimming < 0xFFFFFFFF)) {
            _gIdleCount_dimming++;
        }
        
        if ((__VIDVDStopFlag_Enable == TRUE) && (_gIdleCount_dvd < 0xFFFFFFFF)) {
            _gIdleCount_dvd++;
        }
    }
    else {
        if (_gIdleCount_dimming >= THD_TIME_TO_DIMMING) {
            DimmingOFF_Pending = 1;
        }
        if (_gIdleCount_dvd >= THD_TIME_TO_DVD_STOP) {
            __DVDRestartMotor();
        }

        _gIdleCount_dimming = 0;
        _gIdleCount_dvd = 0;
        THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
    }
    
    if (__VIDimmingFlag_Enable_old != __VIDimmingFlag_Enable) {
        if (__VIDimmingFlag_Enable == FALSE) {
            if (_gIdleCount_dimming >= THD_TIME_TO_DIMMING) {
                DimmingOFF_Pending = 1;
            }
        }

        _gIdleCount_dimming = 0;
        THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
    }
    
    if (_gIdleCount_dimming == THD_TIME_TO_DIMMING) {
        DimmingON_Pending = 1;
    }
    
    if (DimmingOFF_Pending) {
        if (__OSSetVIForceDimming(FALSE, 2, 2) == TRUE) {
            DimmingOFF_Pending = 0;
            __VIDimmingState = FALSE;
        }
    }

    if (DimmingON_Pending) {
        if (__OSSetVIForceDimming(TRUE, 2, 2) == TRUE) {
            DimmingON_Pending = 0;
            __VIDimmingState = TRUE;
        }
    }

    if (__VIDVDStopFlag_Enable_old != __VIDVDStopFlag_Enable) {
        if (__VIDVDStopFlag_Enable == FALSE) {
            if (_gIdleCount_dvd >= THD_TIME_TO_DVD_STOP) {
                __DVDRestartMotor();
            }
        }
        _gIdleCount_dvd = 0;
    }
    
    if(_gIdleCount_dvd  == THD_TIME_TO_DVD_STOP) {
        __DVDStopMotorAsync(&__DVDStopMotorCommandBlock, NULL);
    }

    __VIDimmingFlag_RF_IDLE = 1;
    __VIDimmingFlag_SI_IDLE = 1;

    for(i = 0; i < 10; i++)
    {
        __VIDimmingFlag_DEV_IDLE[i] = 1;
    }
    
    __VIDimmingFlag_Enable_old = __VIDimmingFlag_Enable;
    __VIDVDStopFlag_Enable_old = __VIDVDStopFlag_Enable;
    
    if ((NEW_TIME_TO_DIMMING > _gIdleCount_dimming) && (__VIDimmingState == FALSE)) {
        THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
    }
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
    BOOL enabled;
    VIRetraceCallback oldcb;

    oldcb = PreCB;
    enabled = OSDisableInterrupts();
    PreCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) {
    BOOL enabled;
    VIRetraceCallback oldcb;

    oldcb = PostCB;
    enabled = OSDisableInterrupts();
    PostCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

timing_s* __VISetExtraTiming(timing_s* t) {
    timing_s* old = timingExtra;

    timingExtra = t;
    return old;
}

#pragma dont_inline on
static timing_s* getTiming(VITVMode mode) {
    switch (mode) {
    case VI_TVMODE_NTSC_INT:        return &timing[0];
    case VI_TVMODE_NTSC_DS:         return &timing[1];
    case VI_TVMODE_PAL_INT:         return &timing[2];
    case VI_TVMODE_PAL_DS:          return &timing[3];
    case VI_TVMODE_EURGB60_INT:     return &timing[0];
    case VI_TVMODE_EURGB60_DS:      return &timing[1];
    case VI_TVMODE_MPAL_INT:        return &timing[4];
    case VI_TVMODE_MPAL_DS:         return &timing[5];
    case VI_TVMODE_NTSC_PROG:
    case VI_TVMODE_MPAL_PROG:
    case VI_TVMODE_EURGB60_PROG:    return &timing[6];
    case VI_TVMODE_NTSC_3D:         return &timing[7];
    case VI_TVMODE_DEBUG_PAL_INT:   return &timing[2];
    case VI_TVMODE_DEBUG_PAL_DS:    return &timing[3];
    case VI_TVMODE_GCA_INT:         return &timing[8];
    case VI_TVMODE_GCA_PROG:        return &timing[9];
    case VI_TVMODE_PAL_PROG:        return &timing[10];
    case VI_TVMODE_EXTRA_INT:
    case VI_TVMODE_EXTRA_DS:
    case VI_TVMODE_EXTRA_PROG:
    case VI_TVMODE_HD720_PROG:      return timingExtra;
    default:
        return NULL;
    }
}
#pragma dont_inline reset

void __VIInit(VITVMode mode) {
    timing_s* tm;
    u32 nonInter;
    u32 tv;
    u32 tvForReg;
    volatile u32 a;
    u16 hct;
    u16 vct;

    nonInter = mode & 3;
    tv = (u32)mode >> 2;
    *(u32*)OSPhysicalToCached(0xCC) = tv;

    tm = getTiming(mode);
    __VIRegs[1] = 2;

    // why?
    for (a = 0; a < 1000; a++) {}

    __VIRegs[1] = 0;
    __VIRegs[3] = (u32)tm->hlw;
    __VIRegs[2] = tm->hce | (tm->hcs << 8);
    __VIRegs[5] = tm->hsy | ((tm->hbe640 & 0x1FF) << 7);
    __VIRegs[4] = (tm->hbe640 >> 9) | (tm->hbs640 << 1);

    if (encoderType == 0) {
        __VIRegs[0x39] = tm->hbeCCIR656 | 0x8000;
        __VIRegs[0x3A] = (u32)tm->hbsCCIR656;
    }

    __VIRegs[0] = (u32)tm->equ;
    __VIRegs[7] = (u32)(tm->prbOdd + (tm->acv * 2) - 2);
    __VIRegs[6] = (u32)(tm->psbOdd + 2);
    __VIRegs[9] = (u32)(tm->prbEven + (tm->acv * 2) - 2);
    __VIRegs[8] = (u32)(tm->psbEven + 2);
    __VIRegs[11] = tm->bs1 | (tm->be1 << 5);
    __VIRegs[10] = tm->bs3 | (tm->be3 << 5);
    __VIRegs[13] = tm->bs2 | (tm->be2 << 5);
    __VIRegs[12] = tm->bs4 | (tm->be4 << 5);
    __VIRegs[36] = 0x2828;
    __VIRegs[27] = 1;
    __VIRegs[26] = 0x1001;
    hct = tm->hlw + 1;
    vct = (tm->nhlines / 2) + 1;
    __VIRegs[25] = (u16)(u32)hct;
    __VIRegs[24] = vct | 0x1000;

    switch (tv) {
    case VI_PAL:
    case VI_MPAL:
    case VI_DEBUG:
        tvForReg = tv;
        break;
    default:
        tvForReg = 0;
    }

    if (nonInter == 0 || nonInter == 1) {
        __VIRegs[1] = ((nonInter & 1) << 2) | 1 | (tvForReg << 8);
        __VIRegs[54] = 0;
        return;
    }

    __VIRegs[1] = (tvForReg << 8) | 5;
    __VIRegs[54] = 1;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(val, min, max) ((val) > (max) ? (max) : (val) < (min) ? (min) : (val))

static void AdjustPosition(u16 acv) {
    s32 coeff;
    s32 frac;

    HorVer.AdjustedDispPosX = CLAMP((s16)HorVer.DispPosX + displayOffsetH, 0, 0x2D0 - HorVer.DispSizeX);
    coeff = (HorVer.FBMode == VI_XFBMODE_SF) ? 2 : 1;
    frac = HorVer.DispPosY & 1;
    HorVer.AdjustedDispPosY = MAX((s16)HorVer.DispPosY + displayOffsetV, frac);
    HorVer.AdjustedDispSizeY = HorVer.DispSizeY
                             + MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0)
                             - MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV - (((s16)acv * 2) - frac), 0);
    HorVer.AdjustedPanPosY = HorVer.PanPosY
                           - (MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff);
    HorVer.AdjustedPanSizeY = HorVer.PanSizeY
                            + (MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff)
                            - (MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV - (((s16)acv * 2) - frac), 0) / coeff);
}

static void ImportAdjustingValues(void) {
    displayOffsetH = SCGetDisplayOffsetH();
    displayOffsetV = 0;
}

void VIInit(void) {
    u16 dspCfg;
    u32 value;
    u32 tv;
    u32 tvInBootrom;

    if (IsInitialized) {
        return;
    }

    OSRegisterVersion(__VIVersion);
    IsInitialized = TRUE;

    if (!(__VIRegs[1] & 1)) {
        __VIInit(VI_TVMODE_NTSC_INT);
    }

    retraceCount = 0;
    changed = 0;
    shdwChanged = 0;
    changeMode = 0;
    shdwChangeMode = 0;
    flushFlag = 0;
    flushFlag3in1 = 0;

    __VIRegs[39] = taps[0] | ((taps[1] & 0x3F) << 10);
    __VIRegs[38] = (taps[1] >> 6) | (taps[2] << 4);
    __VIRegs[41] = taps[3] | ((taps[4] & 0x3F) << 10);
    __VIRegs[40] = (taps[4] >> 6) | (taps[5] << 4);
    __VIRegs[43] = taps[6] | ((taps[7] & 0x3F) << 10);
    __VIRegs[42] = (taps[7] >> 6) | (taps[8] << 4);
    __VIRegs[45] = taps[9] | (taps[10] << 8);
    __VIRegs[44] = taps[11] | (taps[12] << 8);
    __VIRegs[47] = taps[13] | (taps[14] << 8);
    __VIRegs[46] = taps[15] | (taps[16] << 8);
    __VIRegs[49] = taps[17] | (taps[18] << 8);
    __VIRegs[48] = taps[19] | (taps[20] << 8);
    __VIRegs[51] = taps[21] | (taps[22] << 8);
    __VIRegs[50] = taps[23] | (taps[24] << 8);
    __VIRegs[56] = 0x280;
    ImportAdjustingValues();

    tvInBootrom = *(u32*)OSPhysicalToCached(0xCC);
    dspCfg = __VIRegs[1];
    HorVer.nonInter = VIGetScanMode();
    HorVer.tv = ((u32)(dspCfg) & 0x300) >> 8;

    if (tvInBootrom == VI_EURGB60 || (tvInBootrom == VI_PAL && HorVer.tv == VI_NTSC)) {
        HorVer.tv = VI_EURGB60;
    }

    tv = (HorVer.tv == 3) ? 0 : HorVer.tv;
    HorVer.timing = getTiming((tv << 2) + HorVer.nonInter);
    regs[1] = dspCfg;

    CurrTiming = HorVer.timing;
    CurrTvMode = HorVer.tv;

    HorVer.DispSizeX = 640;
    HorVer.DispSizeY = CurrTiming->acv * 2;
    HorVer.DispPosX = (720 - HorVer.DispSizeX) / 2;
    HorVer.DispPosY = 0;
    AdjustPosition(CurrTiming->acv);
    HorVer.FBSizeX = 640;
    HorVer.FBSizeY = CurrTiming->acv * 2;
    HorVer.PanPosX = 0;
    HorVer.PanPosY = 0;
    HorVer.PanSizeX = 640;
    HorVer.PanSizeY = CurrTiming->acv * 2;
    HorVer.FBMode = 0;

    HorVer.wordPerLine = 40;
    HorVer.std = 40;
    HorVer.wpl = 40;
    HorVer.xof = 0;
    HorVer.black = 1;
    HorVer.threeD = 0;
    OSInitThreadQueue(&retraceQueue);
    value = __VIRegs[24];
    value &= ~0x8000;
#if !DEBUG
    value = (u16)value;
#endif
    __VIRegs[24] = value;
    value = __VIRegs[26];
    value = value & ~0x8000;
#if !DEBUG
    value = (u16)value;
#endif
    __VIRegs[26] = value;
    PreCB = NULL;
    PostCB = NULL;
    __OSSetInterruptHandler(0x18, __VIRetraceHandler);
    __OSUnmaskInterrupts(0x80);
    OSRegisterShutdownFunction(&ShutdownFunctionInfo);

    switch(VIGetTvFormat()) {
    case VI_PAL:
        THD_TIME_TO_DIMMING = 15000;
        NEW_TIME_TO_DIMMING = 15000;
        THD_TIME_TO_DVD_STOP = 90000;
        break;
    default:
        THD_TIME_TO_DIMMING = 18000;
        NEW_TIME_TO_DIMMING = 18000;
        THD_TIME_TO_DVD_STOP = 108000;
        break;
    }

    _gIdleCount_dimming = 0;
    _gIdleCount_dvd = 0;
    g_current_time_to_dim = VI_DM_DEFAULT;
    __VIDimming_All_Clear = TRUE;
    __VIDimmingState = FALSE;
    VIEnableDimming(TRUE);

    VIEnableDVDStopMotor(FALSE);
    __VISetRevolutionModeSimple();
}

void VIWaitForRetrace(void) {
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = retraceCount;
    do {
        OSSleepThread(&retraceQueue);
    } while (count == retraceCount);
    OSRestoreInterrupts(enabled);
}

static void setInterruptRegs(timing_s* tm) {
#if DEBUG
    u16 vct, hct;
#else
    u16 hct, vct;
#endif
    u16 borrow;

    vct = tm->nhlines / 2;
    borrow = tm->nhlines % 2;
    hct = (u16)((borrow)? tm->hlw : (u16)0);
    vct++;
    hct++;
    regs[25] = (u16)(u32)hct;
    MARK_CHANGED(25);
    regs[24] = vct | 0x1000;
    MARK_CHANGED(24);

    vct;  // fixes regalloc
}

static void setPicConfig(u16 fbSizeX, VIXFBMode xfbMode, u16 panPosX, u16 panSizeX, u8* wordPerLine, u8* std, u8* wpl, u8* xof) {
    *wordPerLine = (fbSizeX + 15) / 16;
    *std = (xfbMode == VI_XFBMODE_SF) ? *wordPerLine : (u8)(*wordPerLine * 2);
    *xof = panPosX % 16;
    *wpl = (*xof + panSizeX + 15) / 16;
    regs[0x24] = *std | (*wpl << 8);
    changed |= 0x8000000;
}

static void setBBIntervalRegs(timing_s* tm) {
    u16 val;

    val = tm->bs1 | (tm->be1 << 5);
    regs[11] = val;
    changed |= 0x10000000000000;

    val = tm->bs3 | (tm->be3 << 5);
    regs[10] = val;
    changed |= 0x20000000000000;

    val = tm->bs2 | (tm->be2 << 5);
    regs[13] = val;
    changed |= 0x4000000000000;

    val = tm->bs4 | (tm->be4 << 5);
    regs[12] = val;
    changed |= (1LL << (63-12));
}

static void setScalingRegs(u16 panSizeX, u16 dispSizeX, BOOL threeD) {
    u32 scale;

    panSizeX = threeD ? (panSizeX << 1) : panSizeX;
    if (panSizeX < dispSizeX) {
        scale = (u32)(dispSizeX + (panSizeX << 8) - 1) / dispSizeX;
        regs[37] = scale | 0x1000;
        changed |= 0x04000000;
        regs[56] = (u32)panSizeX;
        changed |= 0x80;
    } else {
        regs[37] = 0x100;
        changed |= 0x04000000;
    }
}

static void calcFbbs(u32 bufAddr, u16 panPosX, u16 panPosY, u8 wordPerLine, VIXFBMode xfbMode, u16 dispPosY, u32* tfbb, u32* bfbb) {
    u32 bytesPerLine;
    u32 xoffInWords;

    xoffInWords = (u32)panPosX / 16;
    bytesPerLine = (u32)wordPerLine * 32;
    *tfbb = bufAddr + (xoffInWords << 5) + (bytesPerLine * panPosY);
    *bfbb = (xfbMode == VI_XFBMODE_SF) ? *tfbb : *tfbb + bytesPerLine;

    if (dispPosY % 2 == 1) {
        u32 tmp = *tfbb;
        *tfbb = *bfbb;
        *bfbb = tmp;
    }

    *tfbb &= 0x3FFFFFFF;
    *bfbb &= 0x3FFFFFFF;
}

static void setFbbRegs(SomeVIStruct* HorVer, u32* tfbb, u32* bfbb, u32* rtfbb, u32* rbfbb) {
    u32 shifted;

    calcFbbs(HorVer->bufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY, HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY, tfbb, bfbb);
    if (HorVer->threeD) {
        calcFbbs(HorVer->rbufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY, HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY, rtfbb, rbfbb);
    }

    if (*tfbb < 0x01000000U && *bfbb < 0x01000000U && *rtfbb < 0x01000000U && *rbfbb < 0x01000000U) {
        shifted = 0;
    } else {
        shifted = 1;
    }

    if (shifted) {
        *tfbb >>= 5;
        *bfbb >>= 5;
        *rtfbb >>= 5;
        *rbfbb >>= 5;
    }

    regs[15] = (u16)(*tfbb & 0xFFFF);
    MARK_CHANGED(15);
    regs[14] = (shifted << 12) | ((*tfbb >> 16) | (HorVer->xof << 8));
    MARK_CHANGED(14);
    regs[19] = (u16)(*bfbb & 0xFFFF);
    MARK_CHANGED(19);
    regs[18] = (*bfbb >> 16);
    MARK_CHANGED(18);

    if (HorVer->threeD) {
        regs[17] = (u16)(*rtfbb & 0xFFFF);
        MARK_CHANGED(17);
        regs[16] = *rtfbb >> 16;
        MARK_CHANGED(16);
        regs[21] = (u16)(*rbfbb & 0xFFFF);
        MARK_CHANGED(21);
        regs[20] = *rbfbb >> 16;
        MARK_CHANGED(20);
    }
}

static void setHorizontalRegs(timing_s* tm, u16 dispPosX, u16 dispSizeX) {
    u32 hbe;
    u32 hbs;
    u32 hbeLo;
    u32 hbeHi;

    regs[3] = (u16)(u32)tm->hlw;
    MARK_CHANGED(3);
    regs[2] = tm->hce | (tm->hcs << 8);
    MARK_CHANGED(2);

    if (HorVer.tv == 8) {
        hbe = (u32)(tm->hbe640 + 172);
        hbs = tm->hbs640;
    } else {
        hbe = (u32)(tm->hbe640 - 40 + dispPosX);
        hbs = (u32)(tm->hbs640 + 40 + dispPosX - (720 - dispSizeX));    
    }

    hbeLo = hbe & 0x1FF;
    hbeHi = hbe >> 9;

    regs[5] = tm->hsy | (hbeLo << 7);
    MARK_CHANGED(5);
    regs[4] = hbeHi | (hbs * 2);
    MARK_CHANGED(4);
}

static void setVerticalRegs(u16 dispPosY, u16 dispSizeY, u8 equ, u16 acv, u16 prbOdd, u16 prbEven, u16 psbOdd, u16 psbEven, BOOL black) {
    u16 actualPrbOdd;
    u16 actualPrbEven;
    u16 actualPsbOdd;
    u16 actualPsbEven;
    u16 actualAcv;
    u16 c;
    u16 d;

    if (HorVer.nonInter == 2 || HorVer.nonInter == 3) {
        c = 1;
        d = 2;
    } else {
        c = 2;
        d = 1;
    }

    if ((dispPosY % 2) == 0) {
        actualPrbOdd = prbOdd + (d * dispPosY);
        actualPsbOdd = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbEven + (d * dispPosY);
        actualPsbEven = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
    } else {
        actualPrbOdd = prbEven + (d * dispPosY);
        actualPsbOdd = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbOdd + (d * dispPosY);
        actualPsbEven = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
    }

    actualAcv = dispSizeY / c;

    if (black) {
        actualPrbOdd += 2 * actualAcv - 2;
        actualPsbOdd += 2;
        actualPrbEven += 2 * actualAcv - 2;
        actualPsbEven += 2;
        actualAcv = 0;
    }

    regs[0] = equ | (actualAcv << 4);
    MARK_CHANGED(0);
    regs[7] = (u16)(u32)actualPrbOdd;
    MARK_CHANGED(7);
    regs[6] = (u16)(u32)actualPsbOdd;
    MARK_CHANGED(6);
    regs[9] = (u16)(u32)actualPrbEven;
    MARK_CHANGED(9);
    regs[8] = (u16)(u32)actualPsbEven;
    MARK_CHANGED(8);
}

static void PrintDebugPalCaution(void) {
    static u32 message;

    if (message == 0) {
        message = 1;
        OSReport("***************************************\n");
        OSReport(" ! ! ! C A U T I O N ! ! !             \n");
        OSReport("This TV format \"DEBUG_PAL\" is only for \n");
        OSReport("temporary solution until PAL DAC board \n");
        OSReport("is available. Please do NOT use this   \n");
        OSReport("mode in real games!!!                  \n");
        OSReport("***************************************\n");
    }
}

void VIConfigure(const GXRenderModeObj* rm) {
    timing_s* tm;
    u32 regDspCfg;
    u32 regClksel;
    BOOL enabled;
    u32 newNonInter;
    u32 tvInBootrom;
    u32 tvInGame;

    enabled = OSDisableInterrupts();
    newNonInter = rm->viTVmode & 3;

    if (HorVer.nonInter != newNonInter) {
        changeMode = 1;
        HorVer.nonInter = newNonInter;
    }

    VI_ASSERTMSGLINE1(2617, (rm->viHeight & 1) == 0,
        "VIConfigure(): Odd number(%d) is specified to viHeight\n",
        rm->viHeight);

#if DEBUG
    if (rm->xFBmode == VI_XFBMODE_DF || newNonInter == VI_TVMODE_NTSC_PROG || newNonInter == 3) {
        VI_ASSERTMSGLINE2(2624, rm->xfbHeight == rm->viHeight,
            "VIConfigure(): xfbHeight(%d) is not equal to viHeight(%d) when DF XFB mode or progressive mode is specified\n",
            rm->xfbHeight, rm->viHeight);
    }

    if (rm->xFBmode == VI_XFBMODE_SF && newNonInter != VI_TVMODE_NTSC_PROG && newNonInter != 3) {
        VI_ASSERTMSGLINE2(2632, rm->viHeight == rm->xfbHeight * 2,
            "VIConfigure(): xfbHeight(%d) is not as twice as viHeight(%d) when SF XFB mode is specified\n",
            rm->xfbHeight, rm->viHeight);
    }
#endif

    tvInGame = (u32)rm->viTVmode >> 2;
    tvInBootrom = *(u32*)OSPhysicalToCached(0xCC);
    
    if (tvInGame == VI_DEBUG_PAL) {
        PrintDebugPalCaution();
    }

    if (((tvInBootrom != VI_PAL && tvInBootrom != VI_EURGB60) && (tvInGame == VI_PAL || tvInGame == VI_EURGB60)) ||
        ((tvInBootrom == VI_PAL || tvInBootrom == VI_EURGB60) && (tvInGame != VI_PAL && tvInGame != VI_EURGB60)))
    {
        OSPanic(__FILE__, 2651, "VIConfigure(): Tried to change mode from (%d) to (%d), which is forbidden\n", tvInBootrom, tvInGame);
    }

    if ((tvInGame == VI_NTSC) || (tvInGame == VI_MPAL)) {
        HorVer.tv = tvInBootrom;
    } else {
        HorVer.tv = tvInGame;
    }

    HorVer.DispPosX  = rm->viXOrigin;
    HorVer.DispPosY  = (HorVer.nonInter == 1) ? (u16)(rm->viYOrigin * 2) : rm->viYOrigin;
    HorVer.DispSizeX = rm->viWidth;
    HorVer.FBSizeX   = rm->fbWidth;
    HorVer.FBSizeY   = rm->xfbHeight;
    HorVer.FBMode    = rm->xFBmode;
    HorVer.PanSizeX  = HorVer.FBSizeX;
    HorVer.PanSizeY  = HorVer.FBSizeY;
    HorVer.PanPosX   = 0;
    HorVer.PanPosY   = 0;
    HorVer.DispSizeY = (HorVer.nonInter == 2)           ? HorVer.PanSizeY :
                       (HorVer.nonInter == 3)           ? HorVer.PanSizeY :
                       (HorVer.FBMode == VI_XFBMODE_SF) ? (u16)(HorVer.PanSizeY * 2) :
                                                          HorVer.PanSizeY;
    HorVer.threeD = (HorVer.nonInter == 3) ? TRUE : FALSE;

    tm = getTiming((HorVer.tv << 2) + HorVer.nonInter);
    HorVer.timing = tm;

    AdjustPosition(tm->acv);
    VI_ASSERTMSGLINE2(2694, rm->viXOrigin <= tm->hlw + 40 - tm->hbe640,
        "VIConfigure(): viXOrigin(%d) cannot be greater than %d in this TV mode\n",
        rm->viXOrigin, tm->hlw + 40 - tm->hbe640);
    VI_ASSERTMSGLINE2(2699,  rm->viXOrigin + rm->viWidth >= 680 - tm->hbs640,
        "VIConfigure(): viXOrigin + viWidth (%d) cannot be less than %d in this TV mode\n",
        rm->viXOrigin + rm->viWidth, 680 - tm->hbs640);

    setInterruptRegs(tm);

    regDspCfg = regs[1];
    regClksel = regs[54];

    if (HorVer.nonInter == VI_PROGRESSIVE || HorVer.nonInter == 3) {
        regDspCfg = (((u32)(regDspCfg)) & ~0x00000004) | (((u32)(1)) << 2);

        if (HorVer.tv == VI_HD720) {
            regClksel = (((u32)(regClksel)) & ~0x00000001) | 0;
        } else {
            regClksel = (((u32)(regClksel)) & ~0x00000001) | 1;
        }
        
    } else {
        OLD_SET_REG_FIELD(2722, regDspCfg, 1, 2, HorVer.nonInter & 1);
        regClksel = (((u32)(regClksel)) & ~0x00000001);
    }

    OLD_SET_REG_FIELD(2726, regDspCfg, 1, 3, HorVer.threeD);

    if ((HorVer.tv == VI_PAL) || (HorVer.tv == VI_MPAL) || (HorVer.tv == 3)) {
        OLD_SET_REG_FIELD(2730, regDspCfg, 2, 8, HorVer.tv);
    } else {
        regDspCfg = (((u32)(regDspCfg)) & ~0x00000300);
    }

    regs[1] = regDspCfg;
    regs[54] = (u16)regClksel;

    MARK_CHANGED(1);    
    MARK_CHANGED(54);

    setScalingRegs(HorVer.PanSizeX, HorVer.DispSizeX, HorVer.threeD);
    setHorizontalRegs(tm, HorVer.AdjustedDispPosX, HorVer.DispSizeX);
    setBBIntervalRegs(tm);
    setPicConfig(HorVer.FBSizeX, HorVer.FBMode, HorVer.PanPosX, HorVer.PanSizeX, &HorVer.wordPerLine, &HorVer.std, &HorVer.wpl, &HorVer.xof);
    if (FBSet != 0) {
        setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    }
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.AdjustedDispSizeY, tm->equ, tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}

void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {
    BOOL enabled;
    timing_s* tm;

#if DEBUG
    VI_ASSERTMSGLINE1(2788, (xOrg & 1) == 0,
        "VIConfigurePan(): Odd number(%d) is specified to xOrg\n",
        xOrg);
    if (HorVer.FBMode == VI_XFBMODE_DF) {
        VI_ASSERTMSGLINE1(2793, (height & 1) == 0,
            "VIConfigurePan(): Odd number(%d) is specified to height when DF XFB mode\n",
            height);
    }
#endif
    enabled = OSDisableInterrupts();
    HorVer.PanPosX   = xOrg;
    HorVer.PanPosY   = yOrg;
    HorVer.PanSizeX  = width;
    HorVer.PanSizeY  = height;
    HorVer.DispSizeY = (HorVer.nonInter == 2)           ? HorVer.PanSizeY :
                       (HorVer.nonInter == 3)           ? HorVer.PanSizeY :
                       (HorVer.FBMode == VI_XFBMODE_SF) ? (u16)(HorVer.PanSizeY * 2) :
                                                          HorVer.PanSizeY;
    tm = HorVer.timing;
    AdjustPosition(tm->acv);
    setScalingRegs(HorVer.PanSizeX, HorVer.DispSizeX, HorVer.threeD);
    setPicConfig(HorVer.FBSizeX, HorVer.FBMode, HorVer.PanPosX, HorVer.PanSizeX, &HorVer.wordPerLine, &HorVer.std, &HorVer.wpl, &HorVer.xof);
    if (FBSet != 0) {
        setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    }
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.DispSizeY, tm->equ, tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}

void VIFlush(void) {
    BOOL enabled;
    s32 regIndex;

    enabled = OSDisableInterrupts();
    shdwChangeMode |= changeMode;
    changeMode = 0;
    shdwChanged |= changed;

    while (changed != 0) {
        regIndex = cntlzd(changed);
        shdwRegs[regIndex] = regs[regIndex];
        changed &= ~((u64)1 << (63 - regIndex));
    }

    flushFlag = 1;
    flushFlag3in1 = 1;
    NextBufAddr = HorVer.bufAddr;
    OSRestoreInterrupts(enabled);
}

void VISetNextFrameBuffer(void* fb) {
    BOOL enabled;

    VI_ASSERTMSGLINE1(2886, ((u32)fb & 0x1F) == 0,
        "VISetNextFrameBuffer(): Frame buffer address(0x%08x) is not 32byte aligned\n",
        fb);
    enabled = OSDisableInterrupts();
    HorVer.bufAddr = (u32)fb;
    FBSet = 1;
    setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    OSRestoreInterrupts(enabled);
}

void* VIGetNextFrameBuffer(void) {
    return *(void**)(&NextBufAddr);
}

void* VIGetCurrentFrameBuffer(void) {
    return *(void**)(&CurrBufAddr);
}

void VISetNextRightFrameBuffer(void* fb) {
    BOOL enabled;

    ASSERTMSGLINEV(2284, ((u32)fb & 0x1F) == 0,
        "VISetNextFrameBuffer(): Frame buffer address(0x%08x) is not 32byte aligned\n",
        fb);
    enabled = OSDisableInterrupts();
    HorVer.rbufAddr = (u32)fb;
    FBSet = 1;
    setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    OSRestoreInterrupts(enabled);
}

void VISetBlack(BOOL black) {
    BOOL enabled;
    timing_s* tm;

    enabled = OSDisableInterrupts();
    HorVer.black = black;
    tm = HorVer.timing;
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.DispSizeY, tm->equ, tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}

void VISet3D(BOOL threeD) {
    BOOL enabled;
    u32 reg;

    enabled = OSDisableInterrupts();
    HorVer.threeD = threeD;
    reg = regs[1];
    OLD_SET_REG_FIELD(2355, reg, 1, 3, HorVer.threeD);
    regs[1] = reg;
    MARK_CHANGED(1);
    setScalingRegs(HorVer.PanSizeX, HorVer.DispSizeX, HorVer.threeD);
    OSRestoreInterrupts(enabled);
}

u32 VIGetRetraceCount(void) {
    return retraceCount;
}

static void GetCurrentDisplayPosition(u32* hct, u32* vct) {
    u32 hcount, vcount0, vcount;
    vcount = __VIRegs[VI_VERT_COUNT] & 0x7FF;

    do {
        vcount0 = vcount;
        hcount = __VIRegs[VI_HORIZ_COUNT] & 0x7FF;
        vcount = __VIRegs[VI_VERT_COUNT] & 0x7FF;
    } while (vcount0 != vcount);

    *hct = hcount;
    *vct = vcount;
}

static u32 getCurrentHalfLine(void) {
    u32 hcount, vcount;
    GetCurrentDisplayPosition(&hcount, &vcount);

    return ((vcount - 1) << 1) + ((hcount - 1) / CurrTiming->hlw);
}

static u32 getCurrentFieldEvenOdd(void) {
    return (getCurrentHalfLine() < CurrTiming->nhlines) ? 1 : 0;
}

u32 VIGetNextField(void) {
    s32 nextField;
    BOOL enabled;
#if !DEBUG
    u8 unused[4];
#endif

    enabled = OSDisableInterrupts();
    nextField = getCurrentFieldEvenOdd() ^ 1;
    OSRestoreInterrupts(enabled);
    return nextField ^ (HorVer.AdjustedDispPosY & 1);
}

u32 VIGetCurrentLine(void) {
    u32 halfLine;
    timing_s* tm;
    BOOL enabled;

    tm = CurrTiming;
    enabled = OSDisableInterrupts();
    halfLine = getCurrentHalfLine();
    OSRestoreInterrupts(enabled);
    if (halfLine >= tm->nhlines) {
        halfLine -= tm->nhlines;
    }
    return halfLine >> 1U;
}

u32 VIGetTvFormat(void) {
    u32 format;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    switch (CurrTvMode) {
    case VI_NTSC:
    case VI_DEBUG:
    case 6:
    case 7:
    case 8:
        format = VI_NTSC;
        break;
    case VI_PAL:
    case VI_DEBUG_PAL:
        format = VI_PAL;
        break;
    case VI_EURGB60:
    case VI_MPAL:
        format = CurrTvMode;
        break;
    default:
        ASSERTLINE(3198, FALSE);
    }
    
    OSRestoreInterrupts(enabled);
    return format;
}

u32 VIGetScanMode(void) {
    u32 scanMode;
    BOOL enabled = OSDisableInterrupts();

    if ((u32)(__VIRegs[54] & 1) == 1) {
        scanMode = 2;
    } else if (((u32)(__VIRegs[1] & 4) >> 2) == 0) {
        scanMode = 0;
    } else {
        scanMode = 1;
    }
    
    OSRestoreInterrupts(enabled);
    return scanMode;
}

u32 VIGetDTVStatus(void) {
    u32 dtvStatus;
    BOOL enabled = OSDisableInterrupts();

    dtvStatus = __VIRegs[55] & 3;
    OSRestoreInterrupts(enabled);
    return dtvStatus & 1;
}

void __VISetAdjustingValues(s16 x, s16 y) {
    BOOL enabled;
    timing_s* tm;

    ASSERTMSGLINE(2611, (y & 1) == 0, "__VISetAdjustValues(): y offset should be an even number");
    enabled = OSDisableInterrupts();
    displayOffsetH = x;
    displayOffsetV = y;
    tm = HorVer.timing;
    AdjustPosition(tm->acv);
    setHorizontalRegs(tm, HorVer.AdjustedDispPosX, HorVer.DispSizeX);
    if (FBSet != 0) {
        setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    }
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.AdjustedDispSizeY, tm->equ, tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}

void __VIGetAdjustingValues(s16* x, s16* y) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    *x = displayOffsetH;
    *y = displayOffsetV;
    OSRestoreInterrupts(enabled);
}

// DEBUG NONMATCHING - wrong reg use, equivalent
void __VIEnableRawPositionInterrupt(s16 x, s16 y, void (*callback)(s16, s16)) {
    BOOL enabled;
    u32 halfLine;
    u32 halfLineOff;

    enabled = OSDisableInterrupts();
    __VIRegs[29] = x + 1U;
    __VIRegs[31] = x + 1U;

    if (HorVer.nonInter == 0) {
        if (y & 1) {
            halfLineOff = CurrTiming->prbEven + ((CurrTiming->equ * 3) + CurrTiming->nhlines);
            __VIRegs[30] = (((halfLineOff / 2) + (y / 2)) + 1) | 0x1000;
        } else {
            halfLineOff = CurrTiming->prbOdd + (CurrTiming->equ * 3);
            __VIRegs[28] = (((halfLineOff / 2) + (y / 2)) + 1) | 0x1000;
        }
    } else if (HorVer.nonInter == 1) {
        ASSERTLINE(2702, (y & 1) == 0);
        halfLine = CurrTiming->prbOdd + ((CurrTiming->equ * 3)) + y;
        __VIRegs[28] = ((halfLine / 2) + 1) | 0x1000;
        __VIRegs[30] = (((halfLine + CurrTiming->nhlines) / 2) + 1) | 0x1000;
    } else if (HorVer.nonInter == 2) {
        halfLine = CurrTiming->prbOdd + ((CurrTiming->equ * 3)) + y;
        __VIRegs[28] = (halfLine + 1) | 0x1000;
        __VIRegs[30] = 0;
    }

    PositionCallback = callback;
    OSRestoreInterrupts(enabled);
}

void (*__VIDisableRawPositionInterrupt())(s16, s16) {
    BOOL enabled;
    void (*old)(s16, s16);

    enabled = OSDisableInterrupts();
    __VIRegs[28] = 0;
    __VIRegs[30] = 0;

    old = PositionCallback;
    PositionCallback = 0;
    OSRestoreInterrupts(enabled);
    return old;
}

void __VIDisplayPositionToXY(u32 hct, u32 vct, s16* x, s16* y) {
    u32 halfLine = ((vct - 1) << 1) + ((hct - 1) / CurrTiming->hlw);

    if (HorVer.nonInter == VI_INTERLACE) {
        if (halfLine < CurrTiming->nhlines) {
            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
                *y = -1;
            } else {
                *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd) & ~1);
            }
        } else {
            halfLine -= CurrTiming->nhlines;

            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbEven) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbEven) {
                *y = -1;
            } else {
                *y = (s16)(((halfLine - CurrTiming->equ * 3 - CurrTiming->prbEven) & ~1) + 1);
            }
        }
    } else if (HorVer.nonInter == VI_NON_INTERLACE) {
        if (halfLine >= CurrTiming->nhlines) {
            halfLine -= CurrTiming->nhlines;
        }

        if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
            *y = -1;
        } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
            *y = -1;
        } else {
            *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd) & ~1);
        }
    } else if (HorVer.nonInter == VI_PROGRESSIVE) {
        if (halfLine < CurrTiming->nhlines) {
            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
                *y = -1;
            } else {
                *y = (s16)(halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd);
            }
        } else {
            halfLine -= CurrTiming->nhlines;

            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbEven) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbEven) {
                *y = -1;
            } else
                *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbEven) & ~1);
        }
    }

    *x = (s16)(hct - 1);
}

void __VIGetCurrentPosition(s16* x, s16* y) {
    u32 hcount, vcount;
    GetCurrentDisplayPosition(&hcount, &vcount);
    __VIDisplayPositionToXY(hcount, vcount, x, y);
}

void __VISetLatchMode(u32 mode) {
    u32 reg;

    reg = __VIRegs[1];
    OLD_SET_REG_FIELD(2834, reg, 2, 4, mode);
    OLD_SET_REG_FIELD(2835, reg, 2, 6, mode);
    __VIRegs[1] = reg;
}

#pragma dont_inline on
int __VIGetLatch0Position(s16* px, s16* py) {
    u32 hcount;
    u32 vcount;

    if (((__VIRegs[32] & 0x8000) >> 15) != 0) {
        vcount = __VIRegs[32] & 0x7FF;
        hcount = __VIRegs[33] & 0x7FF;
        __VIRegs[32] = 0;
        __VIRegs[33] = 0;
        __VIDisplayPositionToXY(hcount, vcount, px, py);
        return 1;
    }

    *px = *py = -1;
    return 0;
}
#pragma dont_inline reset

#pragma dont_inline on
int __VIGetLatch1Position(s16* px, s16* py) {
    u32 hcount;
    u32 vcount;

    if (((__VIRegs[34] & 0x8000) >> 15) != 0) {
        vcount = __VIRegs[34] & 0x7FF;
        hcount = __VIRegs[35] & 0x7FF;
        __VIRegs[34] = 0;
        __VIRegs[35] = 0;
        __VIDisplayPositionToXY(hcount, vcount, px, py);
        return 1;
    }

    *px = *py = -1;
    return 0;
}
#pragma dont_inline reset

int __VIGetLatchPosition(u32 port, s16* px, s16* py) {
    if (port == 0) {
        return __VIGetLatch0Position(px, py);
    } else {
        return __VIGetLatch1Position(px, py);
    }
}

BOOL VIEnableDimming(BOOL enable) {
    u8 value;
    BOOL old = __VIDimmingFlag_Enable;
    
    if (enable == TRUE) {
        value = SCGetScreenSaverMode();
        
        if (value == 0) {
            enable = FALSE;
        }
    }

    __VIDimmingFlag_Enable = enable;
    return old;
}

BOOL VIResetDimmingCount() {
    return __VIResetDev0Idle();
}

BOOL VIResetRFIdle() {
    return __VIResetRFIdle();
}

BOOL VIEnableDVDStopMotor(BOOL enable) {
    BOOL old = __VIDVDStopFlag_Enable;
    __VIDVDStopFlag_Enable = enable;
    return old;
}

BOOL __VIResetSIIdle() {
    __VIDimmingFlag_SI_IDLE = 0;
    return TRUE;
}

BOOL __VIResetRFIdle() {
    __VIDimmingFlag_RF_IDLE = 0;
    return TRUE;
}

BOOL __VIResetDev0Idle() {
    __VIDimmingFlag_DEV_IDLE[0] = 0;
    return TRUE;
}
