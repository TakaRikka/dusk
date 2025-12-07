#ifndef _DOLPHIN_OS_H_
#define _DOLPHIN_OS_H_

#include <stdio.h>

#ifdef __REVOLUTION_SDK__
#include <revolution/os.h>
#else
#include <dolphin/types.h>
#include <dolphin/gx/GXStruct.h>
void OSReportInit(void);
void OSSwitchFiberEx(u32, u32, u32, u32, u32, u32);
void OSVAttention(const char* fmt, va_list args);

#ifdef __cplusplus
extern "C" {
#endif

typedef s64 OSTime;
typedef u32 OSTick;

#include <dolphin/os/OSAlloc.h>
#include <dolphin/os/OSCache.h>
#include <dolphin/os/OSContext.h>
#include <dolphin/os/OSInterrupt.h>
#include <dolphin/os/OSModule.h>
#include <dolphin/os/OSThread.h>
#include <dolphin/os/OSMutex.h>
#include <dolphin/os/OSFont.h>
#include <dolphin/os/OSReset.h>
#include <dolphin/os/OSResetSW.h>
#include <dolphin/os/OSError.h>
#include <dolphin/os/OSTime.h>
#include <dolphin/os/OSSerial.h>
#include <dolphin/os/OSRtc.h>
#include <dolphin/os/OSMessage.h>
#include <dolphin/os/OSException.h>
#include <dolphin/os/OSAlarm.h>
#include <dolphin/os/OSDC.h>
#include <dolphin/os/OSIC.h>
#include <dolphin/os/OSLC.h>
#include <dolphin/os/OSL2.h>
#include <dolphin/os/OSReboot.h>
#include <dolphin/os/OSExec.h>
#include <dolphin/os/OSMemory.h>
#include <dolphin/os/OSSemaphore.h>
#include <dolphin/os/OSUtf.h>
#include <dolphin/os/OSTimer.h>

// private macro, maybe shouldn't be defined here?
#define OFFSET(addr, align) (((u32)(addr) & ((align)-1)))

#define DOLPHIN_ALIGNMENT 32

// Upper words of the masks, since UIMM is only 16 bits
#define OS_CACHED_REGION_PREFIX   0x8000
#define OS_UNCACHED_REGION_PREFIX 0xC000
#define OS_PHYSICAL_MASK 0x3FFF

#define OS_BASE_CACHED   (OS_CACHED_REGION_PREFIX << 16)
#define OS_BASE_UNCACHED (OS_UNCACHED_REGION_PREFIX << 16)

#ifdef __MWERKS__
u32 __OSPhysicalMemSize AT_ADDRESS(OS_BASE_CACHED | 0x0028);
volatile int __OSTVMode AT_ADDRESS(OS_BASE_CACHED | 0x00CC);
OSThreadQueue __OSActiveThreadQueue AT_ADDRESS(OS_BASE_CACHED | 0x00DC);
OSThread* __OSCurrentThread AT_ADDRESS(OS_BASE_CACHED | 0x00E4);
u32 __OSSimulatedMemSize AT_ADDRESS(OS_BASE_CACHED | 0x00F0);
u32 __OSBusClock AT_ADDRESS(OS_BASE_CACHED | 0x00F8);
u32 __OSCoreClock AT_ADDRESS(OS_BASE_CACHED | 0x00FC);
volatile u16 __OSDeviceCode AT_ADDRESS(OS_BASE_CACHED | 0x30E6);
u16 __OSWirelessPadFixMode AT_ADDRESS(OS_BASE_CACHED | 0x30E0);

// unknowns
OSThread* __gUnkThread1 AT_ADDRESS(OS_BASE_CACHED | 0x00D8);
int __gUnknown800030C0[2] AT_ADDRESS(OS_BASE_CACHED | 0x30C0);
u8 __gUnknown800030E3 AT_ADDRESS(OS_BASE_CACHED | 0x30E3);
#else
#define __OSBusClock  (*(u32 *)(OS_BASE_CACHED | 0x00F8))
#define __OSCoreClock (*(u32 *)(OS_BASE_CACHED | 0x00FC))
#endif

#define OS_BUS_CLOCK   __OSBusClock
#define OS_CORE_CLOCK  __OSCoreClock
#define OS_TIMER_CLOCK (OS_BUS_CLOCK/4)

#define OSTicksToSeconds(ticks)      ((ticks)   / (OS_TIMER_CLOCK))
#define OSTicksToMilliseconds(ticks) ((ticks)   / (OS_TIMER_CLOCK/1000))
#define OSTicksToMicroseconds(ticks) ((ticks) * 8 / (OS_TIMER_CLOCK/125000))
#define OSSecondsToTicks(sec)        ((sec) * (OS_TIMER_CLOCK))
#define OSMillisecondsToTicks(msec)  ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSNanosecondsToTicks(nsec)   (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)
#define OSMicrosecondsToTicks(usec)  (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)

u32 OSGetConsoleType(void);
void OSInit(void);
void OSRegisterVersion(const char* id);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void OSSetArenaHi(void* newHi);
void OSSetArenaLo(void* newLo);
void* OSAllocFromArenaLo(u32 size, u32 align);
void* OSAllocFromArenaHi(u32 size, u32 align);

u32 OSGetPhysicalMemSize(void);

void __OSPSInit(void);
void __OSFPRInit(void);
u32 __OSGetDIConfig(void);

void OSDefaultExceptionHandler(__OSException exception, OSContext* context);

typedef struct OSCalendarTime {
    /* 0x00 */ int sec;
    /* 0x04 */ int min;
    /* 0x08 */ int hour;
    /* 0x0C */ int mday;
    /* 0x10 */ int mon;
    /* 0x14 */ int year;
    /* 0x18 */ int wday;
    /* 0x1C */ int yday;
    /* 0x20 */ int msec;
    /* 0x24 */ int usec;
} OSCalendarTime;

#include <dolphin/dvd.h>
typedef struct OSBootInfo_s {
    DVDDiskID DVDDiskID;
    u32 magic;
    u32 version;
    u32 memorySize;
    u32 consoleType;
    void* arenaLo;
    void* arenaHi;
    void* FSTLocation;
    u32 FSTMaxLength;
} OSBootInfo;

typedef struct OSStopwatch {
    char* name;
    u32 hits;
    OSTime total;
    OSTime min;
    OSTime max;
    OSTime last;
    BOOL running;
    u32 _padding;
} OSStopwatch;

void OSInitStopwatch(OSStopwatch* sw, char* name);
void OSStartStopwatch(OSStopwatch* sw);
void OSStopStopwatch(OSStopwatch* sw);
OSTime OSCheckStopwatch(OSStopwatch* sw);
void OSResetStopwatch(OSStopwatch* sw);
void OSDumpStopwatch(OSStopwatch* sw);

OSTick OSGetTick(void);
OSTime OSGetTime(void);
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
BOOL OSEnableInterrupts(void);
BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);

#define OS_CONSOLE_MASK        0xF0000000
#define OS_CONSOLE_RETAIL      0x00000000
#define OS_CONSOLE_DEVELOPMENT 0x10000000
#define OS_CONSOLE_TDEV        0x20000000

#define OS_CONSOLE_RETAIL4     0x00000004
#define OS_CONSOLE_RETAIL3     0x00000003
#define OS_CONSOLE_RETAIL2     0x00000002
#define OS_CONSOLE_RETAIL1     0x00000001
#define OS_CONSOLE_TDEVHW4     0x20000007
#define OS_CONSOLE_TDEVHW3     0x20000006
#define OS_CONSOLE_TDEVHW2     0x20000005
#define OS_CONSOLE_TDEVHW1     0x20000004
#define OS_CONSOLE_DEVHW4      0x10000007
#define OS_CONSOLE_DEVHW3      0x10000006
#define OS_CONSOLE_DEVHW2      0x10000005
#define OS_CONSOLE_DEVHW1      0x10000004
#define OS_CONSOLE_MINNOW      0x10000003
#define OS_CONSOLE_ARTHUR      0x10000002
#define OS_CONSOLE_PC_EMULATOR 0x10000001
#define OS_CONSOLE_EMULATOR    0x10000000

#define OS_SOUND_MODE_MONO   0
#define OS_SOUND_MODE_STEREO 1

u32 OSGetSoundMode(void);
void OSSetSoundMode(u32 mode);

DECL_WEAK void OSReport(const char* msg, ...);
DECL_WEAK void OSVReport(const char* msg, va_list list);
DECL_WEAK void OSPanic(const char* file, int line, const char* msg, ...);
void OSFatal(GXColor fg, GXColor bg, const char* msg);

// do these belong here?
DECL_WEAK void OSAttention(const char* msg, ...);
DECL_WEAK void OSReport_Error(const char* fmt, ...);
DECL_WEAK void OSReport_FatalError(const char* fmt, ...);
DECL_WEAK void OSReport_System(const char* fmt, ...);
DECL_WEAK void OSReport_Warning(const char* fmt, ...);
DECL_WEAK void OSReportDisable(void);
DECL_WEAK void OSReportEnable(void);
DECL_WEAK void OSReportForceEnableOff(void);
DECL_WEAK void OSReportForceEnableOn(void);
DECL_WEAK void OSVReport(const char* format, va_list list);

#if DEBUG
#define OS_REPORT(...) OSReport(__VA_ARGS__)
#define OS_WARNING(...) OSReport_Warning(__VA_ARGS__)
#define OS_REPORT_ERROR(...) OSReport_Error(__VA_ARGS__)
#define OS_PANIC(line, msg) OSPanic(__FILE__, line, msg)
#else
#define OS_REPORT(...)
#define OS_WARNING(...)
#define OS_REPORT_ERROR(...)
#define OS_PANIC(...)
#endif

extern u8 __OSReport_disable;
extern u8 __OSReport_Error_disable;
extern u8 __OSReport_Warning_disable;
extern u8 __OSReport_System_disable;
extern u8 __OSReport_enable;

#define OSRoundUp32B(x)   (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

void* OSPhysicalToCached(u32 paddr);
void* OSPhysicalToUncached(u32 paddr);
u32 OSCachedToPhysical(void* caddr);
u32 OSUncachedToPhysical(void* ucaddr);
void* OSCachedToUncached(void* caddr);
void* OSUncachedToCached(void* ucaddr);

#if !DEBUG
#define OSPhysicalToCached(paddr)    ((void*) ((u32)(OS_BASE_CACHED   + (u32)(paddr))))
#define OSPhysicalToUncached(paddr)  ((void*) ((u32)(OS_BASE_UNCACHED + (u32)(paddr))))
#define OSCachedToPhysical(caddr)    ((u32)   ((u32)(caddr)  - OS_BASE_CACHED))
#define OSUncachedToPhysical(ucaddr) ((u32)   ((u32)(ucaddr) - OS_BASE_UNCACHED))
#define OSCachedToUncached(caddr)    ((void*) ((u8*)(caddr)  + (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#define OSUncachedToCached(ucaddr)   ((void*) ((u8*)(ucaddr) - (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#endif

// unsorted externs
extern OSTime __OSGetSystemTime(void);
DECL_WEAK extern int __OSIsGcam;
extern OSExecParams __OSRebootParams;
extern OSTime __OSStartTime;
extern int __OSInIPL;

// helper for assert line numbers in different revisions
#if SDK_REVISION < 1
    #define LINE(l0, l1, l2) (l0)
#elif SDK_REVISION < 2
    #define LINE(l0, l1, l2) (l1)
#else
    #define LINE(l0, l1, l2) (l2)
#endif

#if DEBUG
#define ASSERTLINE(line, cond) \
    ((cond) || (OSPanic(__FILE__, line, "Failed assertion " #cond), 0))

#define ASSERTMSGLINE(line, cond, msg) \
    ((cond) || (OSPanic(__FILE__, line, msg), 0))

// This is dumb but we dont have a Metrowerks way to do variadic macros in the macro to make this done in a not scrubby way.
#define ASSERTMSG1LINE(line, cond, msg, arg1) \
    ((cond) || (OSPanic(__FILE__, line, msg, arg1), 0))
    
#define ASSERTMSG2LINE(line, cond, msg, arg1, arg2) \
    ((cond) || (OSPanic(__FILE__, line, msg, arg1, arg2), 0))

#define ASSERTMSGLINEV(line, cond, ...) \
    ((cond) || (OSPanic(__FILE__, line, __VA_ARGS__), 0))

#else
#define ASSERTLINE(line, cond) (void)0
#define ASSERTMSGLINE(line, cond, msg) (void)0
#define ASSERTMSG1LINE(line, cond, msg, arg1) (void)0
#define ASSERTMSG2LINE(line, cond, msg, arg1, arg2) (void)0
#define ASSERTMSGLINEV(line, cond, ...) (void)0
#endif
    
#define ASSERT(cond) ASSERTLINE(__LINE__, cond)

inline s16 __OSf32tos16(__REGISTER f32 inF) {
    __REGISTER s16 out;
    u32 tmp;
    __REGISTER u32* tmpPtr = &tmp;
    // clang-format off
#ifdef __MWERKS__
    asm {
        psq_st inF, 0(tmpPtr), 0x1, 5
        lha out, 0(tmpPtr)
    }
#endif
    // clang-format on

    return out;
}

inline void OSf32tos16(f32* f, s16* out) {
    *out = __OSf32tos16(*f);
}

inline u8 __OSf32tou8(__REGISTER f32 inF) {
    __REGISTER u8 out;
    u32 tmp;
    __REGISTER u32* tmpPtr = &tmp;
    // clang-format off
#ifdef __MWERKS__
    asm {
        psq_st inF, 0(tmpPtr), 0x1, 2
        lbz out, 0(tmpPtr)
    }
#endif
    // clang-format on

    return out;
}

inline void OSf32tou8(f32* f, u8* out) {
    *out = __OSf32tou8(*f);
}

static inline void OSInitFastCast(void) {
    // clang-format off
#ifdef __MWERKS__
    asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3
        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    }
#endif
    // clang-format on
}

#ifdef __cplusplus
}
#endif

#endif
#endif
