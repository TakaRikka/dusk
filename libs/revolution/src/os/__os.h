#ifndef _REVOLUTION_OS_INTERNAL_H_
#define _REVOLUTION_OS_INTERNAL_H_

#include <revolution/os.h>
#include <revolution/esp.h>

#ifdef __cplusplus
extern "C" {
#endif

// OS
extern char* __OSExceptionNames[17]; // D ONLY

u32 __OSIsDebuggerPresent(void);
void __OSPSInit(void);
void __OSGetIOSRev(OSIOSRev* rev);

// OSAlarm
void __OSInitAlarm(void);
void __OSCancelInternalAlarms(void* userdata);

// OSAlloc
extern volatile int __OSCurrHeap;

// OSAudioSystem
void __OSInitAudioSystem(void);
void __OSStopAudioSystem(void);

// OSCache
void __OSCacheInit(void);

// OSContext
void __OSContextInit(void);

// OSError
void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

// OSExec
void __OSGetExecParams(OSExecParams* params);
void __OSSetExecParams(const OSExecParams* params, OSExecParams* addr);
void __OSBootDolSimple(u32 doloffset, u32 restartCode, void* regionStart, void* regionEnd, BOOL argsUseDefault, s32 argc, char** argv);
void __OSBootDol(u32 doloffset, u32 restartCode, const char** argv);
void __OSLaunchMenu();
extern u32 __OSNextPartitionType;

// OSInterrupt
extern void __RAS_OSDisableInterrupts_begin(void);
extern void __RAS_OSDisableInterrupts_end(void);

extern u64 __OSSpuriousInterrupts; // D ONLY
extern char* __OSInterruptNames[33]; // D ONLY
extern char* __OSPIErrors[8]; // D ONLY

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler);
__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt);
void __OSInterruptInit(void);
OSInterruptMask __OSMaskInterrupts(OSInterruptMask global);
OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask global);
void __OSDispatchInterrupt(__OSException exception, OSContext* context);
void __OSModuleInit(void);

// OSIpc
void __OSInitIPCBuffer(void);

// OSLaunch
void __OSRelaunchTitle(u32 resetCode);

// OSMemory
void __OSInitMemoryProtection(void);
void __OSRestoreCodeExecOnMEM1(u32 param_0);

// OSMutex
void __OSUnlockAllMutex(OSThread* thread);
int __OSCheckDeadLock(OSThread* thread);
int __OSCheckMutexes(OSThread* thread);

// OSNet
void __OSInitNet(void);

// OSPlayTime
void __OSInitPlayTime(void);
s32 __OSGetPlayTime(ESTicketView* ticket, __OSPlayTimeType* type, u32* playTime);
BOOL __OSWriteExpiredFlagIfSet(void);

// OSPlayRecord
void __OSStartPlayRecord(void);

// OSReset
void __OSShutdownDevices(u32 event);
int __OSCallShutdownFunctions(BOOL final, u32 event);
void __OSReturnToMenuForError(void);
void __OSHotResetForError(void);

// OSRtc
int __OSGetRTC(u32* rtc);
int __OSSetRTC(u32 rtc);
void __OSInitSram(void);
OSSram* __OSLockSram(void);
OSSramEx* __OSLockSramEx(void);
int __OSUnlockSram(BOOL commit);
int __OSUnlockSramEx(BOOL commit);
int __OSSyncSram(void);
int __OSCheckSram(void);
int __OSReadROM(void* buffer, s32 length, s32 offset);
int __OSReadROMAsync(void* buffer, s32 length, s32 offset, void (*callback)());
u8 __OSGetBootMode(void);
void __OSSetBootMode(u8 ntd);
BOOL __OSGetRTCFlags(u32* flags);
BOOL __OSClearRTCFlags(void);

// OSSync
extern void __OSSystemCallVectorStart();
extern void __OSSystemCallVectorEnd();

void __OSInitSystemCall(void);

// OSStateTM
void __OSInitSTM(void);
BOOL __OSWriteExpiredFlag(void);
int __OSSetVIForceDimming(BOOL isEnabled, u32 yShift, u32 xShift);
s32 __OSUnRegisterStateEvent(void);
void __OSShutdownToSBY(void);
void __OSHotReset(void);

// OSThread
void __OSThreadInit(void);
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 priority);
void __OSReschedule(void);

// OSTime
void __OSSetTime(OSTime time);
OSTime __OSGetSystemTime();
void __OSSetTick(__REGISTER OSTick newTicks);
OSTime __OSTimeToSystemTime(OSTime time);

// ppc_eabi_init
__declspec(section ".init") asm void __init_hardware(void);
__declspec(section ".init") asm void __flush_cache(void* address, unsigned int size);
void __init_user(void);
void _ExitProcess(void);

// start
DECL_WEAK void InitMetroTRK_BBA();

__declspec(section ".init") void __start(void);

__declspec(section ".init") extern void __start(void);
__declspec(section ".init") void __copy_rom_section(void* dst, const void* src, u32 size);
__declspec(section ".init") void __init_bss_section(void* dst, u32 size);
__declspec(section ".init") extern void __init_registers(void);
__declspec(section ".init") extern void __init_data(void);

// time.dolphin
OSTime __get_clock(void);
u32 __get_time(void);
int __to_gm_time(void);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_OS_INTERNAL_H_
