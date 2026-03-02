#include <revolution/ai.h>
#include <revolution/gx.h>
#include <revolution/os.h>
#include <revolution/hw_regs.h>

#include "__gx.h"

#ifdef SDK_AUG2010
#define BUILD_DATE "Aug 23 2010"
#if DEBUG
#define BUILD_TIME "17:24:31"
#else
#define BUILD_TIME "17:33:06"
#endif
#elif SDK_SEP2006
#define BUILD_DATE  "Sep 21 2006"
#define BUILD_TIME "14:32:13"
#endif

#ifdef SDK_AUG2010
#if DEBUG
const char* __AIVersion = "<< RVL_SDK - AI \tdebug build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#else
const char* __AIVersion = "<< RVL_SDK - AI \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#endif
#elif SDK_SEP2006
const char* __AIVersion = "<< RVL_SDK - AI \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4200_60422) >>";
#endif

static AIDCallback __AID_Callback;
static u8* __CallbackStack;
static u8* __OldStack;
static BOOL __AI_init_flag;
static BOOL __AID_Active;
static OSTime bound_32KHz;
static OSTime bound_48KHz;
static OSTime min_wait;
static OSTime max_wait;
static OSTime buffer;

typedef struct {
    OSTime t_start;
    OSTime t1;
    OSTime t2;
    OSTime t3;
    OSTime t4;
    OSTime t_end;
} STRUCT_TIMELOG;
STRUCT_TIMELOG __AIprofile;

OSTime __ai_src_time_start;
OSTime __ai_src_time_end;

// prototypes
STRUCT_TIMELOG* __ai_src_get_time(void);

static void __AIDHandler(__OSInterrupt interrupt, OSContext* context);
static void __AICallbackStackSwitch(void* cb);
static void __AI_SRC_INIT(void);

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
    AIDCallback old_callback;
    BOOL old;

    old_callback = __AID_Callback;
    old = OSDisableInterrupts();
    __AID_Callback = callback;
    OSRestoreInterrupts(old);
    return old_callback;
}

void AIInitDMA(u32 start_addr, u32 length) {
    BOOL old;

    old = OSDisableInterrupts();
    __DSPRegs[24] = (__DSPRegs[24] & ~0x1FFF) | (start_addr >> 16);
    __DSPRegs[25] = (__DSPRegs[25] & ~0xFFE0) | (start_addr & 0xFFFF);
    ASSERTMSGLINE(342, (length & 0x1F) == 0, "AIStartDMA: length must be multiple of 32 bytes");
    __DSPRegs[27] = (__DSPRegs[27] & ~0x7FFF) | (u16)((length >> 5) & 0xFFFF);
    OSRestoreInterrupts(old);
}

BOOL AIGetDMAEnableFlag(void) {
    return (__DSPRegs[27] & (1 << 15)) >> 15;
}

void AIStartDMA(void) {
    __DSPRegs[27] = __DSPRegs[27] | 0x8000;
}

void AIStopDMA(void) {
    __DSPRegs[27] = __DSPRegs[27] & ~0x8000;
}

u32 AIGetDMABytesLeft(void) {
    return (__DSPRegs[29] & 0x7FFF) << 5;
}

u32 AIGetDMAStartAddr(void) {
    return (((__DSPRegs[24] & 0x1FFF) << 16) | (__DSPRegs[25] & 0xFFE0));
}

u32 AIGetDMALength(void) {
    return (__DSPRegs[27] & 0x7FFF) << 5;
}

BOOL AICheckInit(void) {
    return __AI_init_flag;
}

void AISetDSPSampleRate(u32 rate) {
    BOOL old;

    if (rate != AIGetDSPSampleRate()) {
        __AIRegs[0] = (__AIRegs[0] & ~0x40);
        if (rate == AI_SAMPLERATE_32KHZ) {
            old = OSDisableInterrupts();
            __AI_SRC_INIT();
            __AIRegs[0] |= 0x40;
            OSRestoreInterrupts(old);
        }
    }
}

u32 AIGetDSPSampleRate(void) {
    return GET_REG_FIELD(__AIRegs[0], 1, 6) ^ 1;
}

void AIInit(u8* stack) {
    u32 reg;

    if (__AI_init_flag != TRUE) {
        OSRegisterVersion(__AIVersion);

        bound_32KHz = OSNanosecondsToTicks(31524);
        bound_48KHz = OSNanosecondsToTicks(42024);
        min_wait = OSNanosecondsToTicks(42000);
        max_wait = OSNanosecondsToTicks(63000);
        buffer = OSNanosecondsToTicks(3000);

        reg = __AIRegs[0];
        reg &= ~(0x1 | 0x4 | 0x10);
        __AIRegs[0] = reg;
        __AIRegs[1] = 0;
        __AIRegs[3] = 0;
        __AIRegs[0] = (__AIRegs[0] & ~0x20) | (0x1 << 5);

        AISetDSPSampleRate(AI_SAMPLERATE_32KHZ);
#if DEBUG
        OSReport("AIInit(): DSP is 32KHz\n");
#endif

        __AID_Callback = NULL;
        __CallbackStack = stack;

        ASSERTMSGLINE(663, !stack || ((u32)stack & 7) == 0, "AIInit: stack must be 8-byte aligned");

        __OSSetInterruptHandler(5, __AIDHandler);
        __OSUnmaskInterrupts(0x04000000);
        __AI_init_flag = TRUE;
    }
}

void AIReset(void) {
    __AI_init_flag = FALSE;
}

static void __AIDHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA0) | 8;
    __DSPRegs[5] = tmp;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (__AID_Callback && !__AID_Active) {
        __AID_Active = TRUE;
        if (__CallbackStack) {
            __AICallbackStackSwitch(__AID_Callback);
        } else {
            __AID_Callback();
        }
        __AID_Active = FALSE;
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

static asm void __AICallbackStackSwitch(__REGISTER void* cb) {
    fralloc
    lis r5, __OldStack@ha
    addi r5, r5, __OldStack@l
    stw r1, 0x0(r5)
    lis r5, __CallbackStack@ha
    addi r5, r5, __CallbackStack@l
    lwz r1, 0x0(r5)
    subi r1, r1, 0x8
    mtlr cb
    blrl
    lis r5, __OldStack@ha
    addi r5, r5, __OldStack@l
    lwz r1, 0x0(r5)
    frfree
    blr
}

void __AI_SRC_INIT(void) {
    OSTime rising_32khz = 0;
    OSTime rising_48khz = 0;
    OSTime diff = 0;
    OSTime t1 = 0;
    OSTime temp;
    u32 temp0;
    u32 temp1;
    u32 done = 0;
    u32 volume = 0;
    u32 Init_Cnt = 0;
    u32 walking = 0;

    walking = 0;
    Init_Cnt = 0;
    temp = 0;

#if DEBUG
    __AIprofile.t_start = OSGetTime();
#endif

    while (!done) {
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 5, 1);
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 1, 0);
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_START);
        temp0 = __AIRegs[2] & 0x7FFFFFFF;
        while (temp0 == (__AIRegs[2] & 0x7FFFFFFF)) {
        }
        rising_32khz = OSGetTime();
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 1, 1);
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_START);
        temp1 = __AIRegs[2] & 0x7FFFFFFF;
        while (temp1 == (__AIRegs[2] & 0x7FFFFFFF)) {
        }
        rising_48khz = OSGetTime();
        diff = rising_48khz - rising_32khz;
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 1, 0);
        OLD_SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_STOP);
        if (diff < bound_32KHz - buffer) {
            temp = min_wait;
            done = 1;
            Init_Cnt++;
        } else if (diff >= bound_32KHz + buffer && diff < bound_48KHz - buffer) {
            temp = max_wait;
            done = 1;
            Init_Cnt++;
        } else {
            done = 0;
            walking = 1;
            Init_Cnt++;
        }
    }
    while (rising_48khz + temp > OSGetTime()) {
    }
#if DEBUG
    __AIprofile.t_end = OSGetTime();
#endif
}

STRUCT_TIMELOG* __ai_src_get_time(void) {
#if DEBUG
    return &__AIprofile;
#else
    return NULL;
#endif
}
