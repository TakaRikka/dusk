#ifndef _DOLPHIN_OSRESET_H_
#define _DOLPHIN_OSRESET_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_RESET_RESTART  0
#define OS_RESET_HOTRESET 1
#define OS_RESET_SHUTDOWN 2

typedef struct OSResetFunctionInfo OSResetFunctionInfo;
typedef struct OSResetFunctionQueue {
    OSResetFunctionInfo* head;
    OSResetFunctionInfo* tail;
} OSResetFunctionQueue;

typedef BOOL (*OSResetFunction)(BOOL);

struct OSResetFunctionInfo {
    OSResetFunction func;
    u32 priority;
    OSResetFunctionInfo* next;
    OSResetFunctionInfo* prev;
};

void OSRegisterResetFunction(OSResetFunctionInfo* info);
void OSUnregisterResetFunction(OSResetFunctionInfo* info);
void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu);
u32 OSGetResetCode();
u32 OSSetBootDol(u32 dolOffset);

#ifdef __cplusplus
}
#endif

#endif
