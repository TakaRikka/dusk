#ifndef _DOLPHIN_OSTHREAD_H_
#define _DOLPHIN_OSTHREAD_H_

#include <dolphin/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s32 OSPriority;

typedef struct OSThread OSThread;
typedef struct OSMutex OSMutex;
typedef struct OSThreadQueue OSThreadQueue;
typedef struct OSMutexQueue OSMutexQueue;
typedef struct OSThreadLink OSThreadLink;
typedef struct OSMutexLink OSMutexLink;

struct OSThreadQueue {
    OSThread* head;
    OSThread* tail;
};

struct OSThreadLink {
    OSThread* next;
    OSThread* prev;
};

struct OSMutexQueue {
    OSMutex* head;
    OSMutex* tail;
};

struct OSMutexLink {
    OSMutex* next;
    OSMutex* prev;
};

struct OSThread {
    /* 0x000 */ OSContext context;   
    /* 0x2C8 */ u16 state;     
    /* 0x2CA */ u16 attr;      
    /* 0x2CC */ s32 suspend;   
    /* 0x2D0 */ OSPriority priority;  
    /* 0x2D4 */ OSPriority base;
    /* 0x2D8 */ void* val;                          
    /* 0x2DC */ OSThreadQueue* queue;     
    /* 0x2E0 */ OSThreadLink link;                    
    /* 0x2E8 */ OSThreadQueue queueJoin;              
    /* 0x2F0 */ OSMutex* mutex;     
    /* 0x2F4 */ OSMutexQueue queueMutex;              
    /* 0x2FC */ OSThreadLink linkActive;             
    /* 0x304 */ u8* stackBase; 
    /* 0x308 */ u32* stackEnd;  
    /* 0x30C */ s32 error;
    /* 0x310 */ void* specific[2];
};

enum OS_THREAD_STATE {
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_WAITING = 4,
    OS_THREAD_STATE_MORIBUND = 8,
};

#define OS_PRIORITY_MIN 0  // highest
#define OS_PRIORITY_MAX 31 // lowest
#define OS_PRIORITY_IDLE OS_PRIORITY_MAX

#define OS_THREAD_SPECIFIC_MAX 2

#define OS_THREAD_ATTR_DETACH 0x0001u

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

typedef void (*OSSwitchThreadCallback)(OSThread*, OSThread*);
typedef void (*OSIdleFunction)(void*);

void OSInitThreadQueue(OSThreadQueue* queue);
void OSSleepThread(OSThreadQueue* queue);
void OSWakeupThread(OSThreadQueue* queue);
s32 OSSuspendThread(OSThread* thread);
s32 OSResumeThread(OSThread* thread);
OSThread* OSGetCurrentThread(void);
s32 OSEnableScheduler(void);
s32 OSDisableScheduler(void);
void OSCancelThread(OSThread* thread);
void OSClearStack(u8 val);
BOOL OSIsThreadSuspended(OSThread* thread);
BOOL OSIsThreadTerminated(OSThread* thread);
void OSYieldThread(void);
int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, OSPriority priority, u16 attr);
void OSExitThread(void* val);
int OSJoinThread(OSThread* thread, void* val);
void OSDetachThread(OSThread* thread);
int OSSetThreadPriority(OSThread* thread, OSPriority priority);
s32 OSGetThreadPriority(OSThread* thread);
OSThread* OSSetIdleFunction(OSIdleFunction idleFunction, void* param, void* stack, u32 stackSize);
OSThread* OSGetIdleFunction(void);
s32 OSCheckActiveThreads(void);
void OSSetThreadSpecific(s32 index, void* ptr);
void* OSGetThreadSpecific(s32 index);

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback callback);

#define IsSuspended(suspend) (suspend > 0)

#ifdef __cplusplus
}
#endif

#endif
