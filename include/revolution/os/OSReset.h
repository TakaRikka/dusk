#ifndef _REVOLUTION_OSRESET_H_
#define _REVOLUTION_OSRESET_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_RESET_RESTART  0
#define OS_RESET_HOTRESET 1
#define OS_RESET_SHUTDOWN 2

typedef struct OSShutdownFunctionInfo OSShutdownFunctionInfo;
typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head;
    OSShutdownFunctionInfo* tail;
} OSShutdownFunctionQueue;

typedef BOOL (*OSResetFunction)(BOOL, u32);

struct OSShutdownFunctionInfo {
    OSResetFunction func;
    u32 priority;
    OSShutdownFunctionInfo* next;
    OSShutdownFunctionInfo* prev;
};

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info);
void OSUnregisterShutdownFunction(OSShutdownFunctionInfo* info);
void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu);
u32 OSGetResetCode();
u32 OSSetBootDol(u32 dolOffset);
void OSReturnToMenu(void);

#define OSIsRestart()   \
    ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif
