#include <revolution.h>
#include <revolution/os.h>

#include "__os.h"

#define ENQUEUE_THREAD(thread, queue, link)       \
    do {                                          \
        OSThread* __prev = (queue)->tail; \
        if (__prev == NULL) {                     \
            (queue)->head = (thread);             \
        } else {                                  \
            __prev->link.next = (thread);         \
        }                                         \
        (thread)->link.prev = __prev;             \
        (thread)->link.next = 0;                  \
        (queue)->tail = (thread);                 \
    } while(0);

#define DEQUEUE_THREAD(thread, queue, link)             \
    do {                                                \
        OSThread* __next = (thread)->link.next; \
        OSThread* __prev = (thread)->link.prev; \
        if (__next == NULL) {                           \
            (queue)->tail = __prev;                     \
        } else {                                        \
            __next->link.prev = __prev;                 \
        }                                               \
        if (__prev == NULL) {                           \
            (queue)->head = __next;                     \
        } else {                                        \
            __prev->link.next = __next;                 \
        }                                               \
    } while(0);

#define ENQUEUE_THREAD_PRIO(thread, queue, link)       \
    do {                                               \
        OSThread* __prev;                      \
        OSThread* __next;                      \
        for(__next = (queue)->head; __next             \
          && (__next->priority <= (thread)->priority); \
                __next = __next->link.next) ;          \
                                                       \
        if (__next == NULL) {                          \
            ENQUEUE_THREAD(thread, queue, link);       \
        } else {                                       \
            (thread)->link.next = __next;              \
            __prev = __next->link.prev;                \
            __next->link.prev = (thread);              \
            (thread)->link.prev = __prev;              \
            if (__prev == NULL) {                      \
                (queue)->head = (thread);              \
            } else {                                   \
                __prev->link.next = (thread);          \
            }                                          \
        }                                              \
    } while(0);

#define DEQUEUE_HEAD(thread, queue, link)             \
    do {                                              \
        OSThread* __next = thread->link.next; \
        if (__next == NULL) {                         \
            (queue)->tail = 0;                        \
        } else {                                      \
            __next->link.prev = 0;                    \
        }                                             \
        (queue)->head = __next;                       \
    } while(0);

// defined in linkscript
extern u8 _stack_end[];
extern u8 _stack_addr[];

static OSThreadQueue RunQueue[32];
static OSThread IdleThread;
static OSThread DefaultThread;
static OSContext IdleContext;
static volatile u32 RunQueueBits;
static volatile int RunQueueHint;
static volatile s32 Reschedule;

#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ALIGN8(val) (((val) + 0x7) & ~0x7)

// prototypes
static void OSInitMutexQueue(OSMutexQueue* queue);
static void __OSSwitchThread(OSThread* nextThread);
static int __OSIsThreadActive(OSThread* thread);
static void SetRun(OSThread* thread);
static void UnsetRun(OSThread* thread);
static OSThread* SetEffectivePriority(OSThread* thread, s32 priority);
static void UpdatePriority(OSThread* thread);
static OSThread* SelectThread(int yield);
static int CheckThreadQueue(OSThreadQueue* queue);
static int IsMember(OSThreadQueue* queue, OSThread* thread);
static void OSSetCurrentThread(OSThread* thread);

static void DefaultSwitchThreadCallback(OSThread* from, OSThread* to) {}
static OSSwitchThreadCallback SwitchThreadCallback = DefaultSwitchThreadCallback;

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback callback) {
    OSSwitchThreadCallback prev;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    prev = SwitchThreadCallback;

    SwitchThreadCallback = callback ? callback : DefaultSwitchThreadCallback;
    OSRestoreInterrupts(enabled);

    return prev == DefaultSwitchThreadCallback ? NULL : prev;
}

void __OSThreadInit() {
    OSThread* thread = &DefaultThread;
    OSPriority prio;

    thread->state = OS_THREAD_STATE_RUNNING;
    thread->attr = 1;
    thread->priority = thread->base = 0x10;
    thread->suspend = 0;
    thread->val = (void*)-1; // wut
    thread->mutex = 0;

    OSInitThreadQueue(&thread->queueJoin);
#if DEBUG
    OSInitMutexQueue(&thread->queueMutex);
#else
    thread->queueMutex.head = thread->queueMutex.tail = 0; // it got inlined? cant reproduce the inline...
#endif

    ASSERTLINE(372, PPCMfmsr() & MSR_FP);

    __gUnkThread1 = thread;
    OSClearContext(&thread->context);
    OSSetCurrentContext(&thread->context);
    thread->stackBase = (u8*)&_stack_addr;
    thread->stackEnd = (u32*)&_stack_end;
    *(u32*)thread->stackEnd = OS_THREAD_STACK_MAGIC;
    OSSetCurrentThread(thread);
    OSClearStack(0);
    RunQueueBits = 0;
    RunQueueHint = 0;

    for (prio = 0; prio <= 31; prio++) {
        OSInitThreadQueue(&RunQueue[prio]);
    }
    OSInitThreadQueue(&__OSActiveThreadQueue);

    ENQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);

    OSClearContext(&IdleContext);
    Reschedule = 0;
}

void OSSetCurrentThread(OSThread* thread) {
    SwitchThreadCallback(__OSCurrentThread, thread);
    __OSCurrentThread = thread;
}

#if DEBUG
static void OSInitMutexQueue(OSMutexQueue* queue) {
    queue->head = queue->tail = 0;
}
#endif

void OSInitThreadQueue(OSThreadQueue* queue) {
    queue->head = queue->tail = 0;
}

OSThread* OSGetCurrentThread() {
    return __OSCurrentThread;
}

static void __OSSwitchThread(OSThread* nextThread) {
    OSSetCurrentThread(nextThread);
    OSSetCurrentContext(&nextThread->context);
    OSLoadContext(&nextThread->context);
}

BOOL OSIsThreadSuspended(OSThread* thread) {
    if (thread->suspend > 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL OSIsThreadTerminated(OSThread* thread) {
    return (thread->state == OS_THREAD_STATE_MORIBUND || thread->state == 0) ? TRUE : FALSE;
}

static BOOL __OSIsThreadActive(OSThread* thread) {
    OSThread* active;

    if (thread->state == 0) {
        return FALSE;
    }
    
    for (active = __OSActiveThreadQueue.head; active; active = active->linkActive.next) {
        if (thread == active) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 OSDisableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule++;
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSEnableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule--;
    OSRestoreInterrupts(enabled);
    return count;
}

static void SetRun(OSThread* thread) {
    ASSERTLINE(569, !IsSuspended(thread->suspend));
    ASSERTLINE(570, thread->state == OS_THREAD_STATE_READY);
    
    ASSERTLINE(572, OS_PRIORITY_MIN <= thread->priority && thread->priority <= OS_PRIORITY_MAX);

    thread->queue = &RunQueue[thread->priority];

    ENQUEUE_THREAD(thread, thread->queue, link);
    
    RunQueueBits |= 1 << (OS_PRIORITY_MAX - thread->priority);
    RunQueueHint = 1;
}

static void UnsetRun(OSThread* thread) {
    OSThreadQueue* queue;

    ASSERTLINE(593, thread->state == OS_THREAD_STATE_READY);
    
    ASSERTLINE(595, OS_PRIORITY_MIN <= thread->priority && thread->priority <= OS_PRIORITY_MAX);
    ASSERTLINE(596, thread->queue == &RunQueue[thread->priority]);

    queue = thread->queue;

    DEQUEUE_THREAD(thread, queue, link);

    if (!queue->head) {
        RunQueueBits &= ~(1 << (OS_PRIORITY_MAX - thread->priority));
    }
    thread->queue = NULL;
}

s32 __OSGetEffectivePriority(OSThread* thread) {
    s32 priority = thread->base;
    OSMutex* mutex;
    OSThread* blocked;
    
    for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
        blocked = mutex->queue.head;
        if (blocked && blocked->priority < priority) {
            priority = blocked->priority;
        }
    }
    return priority;
}

static OSThread* SetEffectivePriority(OSThread* thread, s32 priority) {
    ASSERTLINE(647, !IsSuspended(thread->suspend));

    switch(thread->state) {
    case OS_THREAD_STATE_READY:
        UnsetRun(thread);
        thread->priority = priority;
        SetRun(thread);
        break;
    case OS_THREAD_STATE_WAITING:
        DEQUEUE_THREAD(thread, thread->queue, link);
        thread->priority = priority;

        ENQUEUE_THREAD_PRIO(thread, thread->queue, link);

        if (thread->mutex) {
            ASSERTLINE(662, thread->mutex->thread);
            return thread->mutex->thread;
        }
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = 1;
        thread->priority = priority;
        break;
    }
    return 0;
}

static void UpdatePriority(OSThread* thread) {
    s32 priority;

    while (1) {
        if(thread->suspend > 0) {
            break;
        }
        priority = __OSGetEffectivePriority(thread);
        if (thread->priority == priority) {
            break;
        }
        thread = SetEffectivePriority(thread, priority);
        if (thread == 0) {
            break;
        }
    }
}

void __OSPromoteThread(OSThread* thread, s32 priority) {
    while (1) {
        if (thread->suspend > 0 || thread->priority <= priority) {
            break;
        }
        thread = SetEffectivePriority(thread, priority);
        if (thread == 0) {
            break;
        }
    }
}

static OSThread* SelectThread(int yield) {
    OSContext* currentContext;
    OSThread* currentThread;
    OSThread* nextThread;
    OSPriority priority;
    OSThreadQueue* queue;

    if (Reschedule > 0) {
        return NULL;
    }

    currentContext = OSGetCurrentContext();
    currentThread = OSGetCurrentThread();

    if (currentContext != &currentThread->context) {
        return NULL;
    }

    if (currentThread) {
        if (currentThread->state == 2) {
            if (yield == 0) {
                priority = __cntlzw(RunQueueBits);
                if (currentThread->priority <= priority)
                    return NULL;
            }
            currentThread->state = OS_THREAD_STATE_READY;
            SetRun(currentThread);
        }
        if (!(currentThread->context.state & 2) && (OSSaveContext(&currentThread->context) != 0)) {
            return NULL;
        }
    }

    if (RunQueueBits == 0) {
        OSSetCurrentThread(NULL);
        OSSetCurrentContext(&IdleContext);
        do {
            OSEnableInterrupts();
            while (RunQueueBits == 0) ;
            OSDisableInterrupts();
        } while (RunQueueBits == 0);
        OSClearContext(&IdleContext);
    }

    RunQueueHint = 0;
    priority = __cntlzw(RunQueueBits);

    ASSERTLINE(823, OS_PRIORITY_MIN <= priority && priority <= OS_PRIORITY_MAX);

    queue = &RunQueue[priority];
    nextThread = queue->head;

    DEQUEUE_HEAD(nextThread, queue, link);

    ASSERTLINE(826, nextThread->priority == priority);

    if (!queue->head) {
        RunQueueBits &= ~(1 << (OS_PRIORITY_MAX - priority));
    }
    nextThread->queue = 0;
    nextThread->state = OS_THREAD_STATE_RUNNING;
    __OSSwitchThread(nextThread);
    return nextThread;
}

void __OSReschedule(void) {
    if (RunQueueHint != 0) {
        SelectThread(0);
    }
}

void OSYieldThread(void) {
    BOOL enabled = OSDisableInterrupts();

    SelectThread(1);
    OSRestoreInterrupts(enabled);
}

int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, OSPriority priority, u16 attr) {
    BOOL enabled;
    u32 sp;
    int i;

    ASSERTMSGLINE(910, ((priority >= OS_PRIORITY_MIN) && (priority <= OS_PRIORITY_MAX)), "OSCreateThread(): priority out of range (0 <= priority <= 31).");

    // why check this for an assert just to check it again right after?
    if ((priority < OS_PRIORITY_MIN) || (priority > OS_PRIORITY_MAX)) {
        return 0;
    }

    thread->state = OS_THREAD_STATE_READY;
    thread->attr = attr & 1U;
    thread->base = priority;
    thread->priority = priority;
    thread->suspend = 1;
    thread->val = (void*)-1;
    thread->mutex = 0;
    OSInitThreadQueue(&thread->queueJoin);
#if DEBUG
    OSInitMutexQueue(&thread->queueMutex);
#else
    OSInitThreadQueue((void*)&thread->queueMutex); // why
#endif
    sp = (u32)stack;
    sp &= ~7;
    sp -= 8;
    ((u32*)sp)[0] = 0; 
    ((u32*)sp)[1] = 0;
    OSInitContext(&thread->context, (u32)func, sp);
    thread->context.lr = (u32)&OSExitThread;
    thread->context.gpr[3] = (u32)param;
    thread->stackBase = stack;
    thread->stackEnd = (void*)((unsigned int)stack - stackSize);
    *thread->stackEnd = OS_THREAD_STACK_MAGIC;
    thread->error = 0;
    for (i = 0; i < 2; i++) {
        thread->specific[i] = NULL;
    }
    enabled = OSDisableInterrupts();

    if (__OSErrorTable[16] != NULL) {
        thread->context.srr1 |= 0x900;
        thread->context.state |= 1;
        thread->context.fpscr = (__OSFpscrEnableBits & 0xf8) | 4;
        for (i = 0; i < 32; ++i) {
            *(u64*)&thread->context.fpr[i] = (u64)0xffffffffffffffffLL;
            *(u64*)&thread->context.psf[i] = (u64)0xffffffffffffffffLL;
        }
    }

    ASSERTMSG1LINE(964, __OSIsThreadActive(thread) == 0L, "OSCreateThread(): thread %p is still active.", thread);

    ENQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);

    OSRestoreInterrupts(enabled);
    return 1;
}

void OSExitThread(void* val) {
    BOOL enabled = OSDisableInterrupts();
    OSThread* currentThread = OSGetCurrentThread();

    ASSERTMSGLINE(989, currentThread, 
        "OSExitThread(): current thread does not exist.");
    ASSERTMSGLINE(991, currentThread->state == OS_THREAD_STATE_RUNNING, 
        "OSExitThread(): current thread is not running.");
    ASSERTMSGLINE(993, __OSIsThreadActive(currentThread) != 0, 
        "OSExitThread(): current thread is not active.");

    OSClearContext(&currentThread->context);
    if (currentThread->attr & 1) {
        DEQUEUE_THREAD(currentThread, &__OSActiveThreadQueue, linkActive);
        currentThread->state = 0;
    } else {
        currentThread->state = 8;
        currentThread->val = val;
    }
    __OSUnlockAllMutex(currentThread);
    OSWakeupThread(&currentThread->queueJoin);
    RunQueueHint = 1;
#if DEBUG
    __OSReschedule();
#else
    if (RunQueueHint != 0) {
        SelectThread(0);
    }
#endif
    OSRestoreInterrupts(enabled);
}

void OSCancelThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();

    ASSERTMSG1LINE(1031, __OSIsThreadActive(thread) != 0, 
        "OSExitThread(): thread %p is not active.", thread);

    __OSCancelInternalAlarms(thread);

    switch(thread->state) {
    case OS_THREAD_STATE_READY:
        if (thread->suspend <= 0) {
            UnsetRun(thread);
        }
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = 1;
        break;
    case OS_THREAD_STATE_WAITING:
        DEQUEUE_THREAD(thread, thread->queue, link);
        thread->queue = 0;
        if ((thread->suspend <= 0) && (thread->mutex)) {
            ASSERTLINE(1053, thread->mutex->thread);
            UpdatePriority(thread->mutex->thread);
        }
        break;
    default:
        OSRestoreInterrupts(enabled);
        return;
    }
    OSClearContext(&thread->context);
    if (thread->attr & 1) {
        DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
        thread->state = 0;
    } else {
        thread->state = 8;
    }
    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->queueJoin);
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

int OSJoinThread(OSThread* thread, void* val) {
    BOOL enabled = OSDisableInterrupts();

    ASSERTMSG1LINE(1061, __OSIsThreadActive(thread) != 0, "OSJoinThread(): thread %p is not active.", thread);

    if (!(thread->attr & 1) && (thread->state != OS_THREAD_STATE_MORIBUND) && (thread->queueJoin.head == NULL)) {
        OSSleepThread(&thread->queueJoin);
        if (__OSIsThreadActive(thread) == 0) {
            OSRestoreInterrupts(enabled);
            return 0;
        }
    }
    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        if (val) {
            *(s32*)val = (s32)thread->val;
        }
        DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
        thread->state = 0;
        OSRestoreInterrupts(enabled);
        return 1;
    }
    OSRestoreInterrupts(enabled);
    return 0;
}

void OSDetachThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();

    ASSERTMSG1LINE(1160, __OSIsThreadActive(thread) != 0, "OSDetachThread(): thread %p is not active.", thread);
    
    thread->attr |= 1;
    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
        thread->state = 0;
    }
    OSWakeupThread(&thread->queueJoin);
    OSRestoreInterrupts(enabled);
}

s32 OSResumeThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();
    s32 suspendCount;

    ASSERTMSG1LINE(1189, __OSIsThreadActive(thread) != 0, "OSResumeThread(): thread %p is not active.", thread);
    ASSERTMSG1LINE(1191, thread->state != OS_THREAD_STATE_MORIBUND, "OSResumeThread(): thread %p is terminated.", thread);

    suspendCount = thread->suspend--;
    if (thread->suspend < 0) {
        thread->suspend = 0;
    } else if (thread->suspend == 0) {
        switch(thread->state) {
        case OS_THREAD_STATE_READY:
            thread->priority = __OSGetEffectivePriority(thread);
            SetRun(thread);
            break;
        case OS_THREAD_STATE_WAITING:
            ASSERTLINE(1206, thread->queue);
            DEQUEUE_THREAD(thread, thread->queue, link);
            thread->priority = __OSGetEffectivePriority(thread);
            ENQUEUE_THREAD_PRIO(thread, thread->queue, link);
            if (thread->mutex) {
                UpdatePriority(thread->mutex->thread);
            }
        }
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return suspendCount;
}

s32 OSSuspendThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();
    s32 suspendCount;

    ASSERTMSG1LINE(1240, __OSIsThreadActive(thread) != 0, "OSSuspendThread(): thread %p is not active.", thread);
    ASSERTMSG1LINE(1242, thread->state != OS_THREAD_STATE_MORIBUND, "OSSuspendThread(): thread %p is terminated.", thread);

    suspendCount = thread->suspend++;
    if (suspendCount == 0) {
        switch(thread->state) {
        case OS_THREAD_STATE_RUNNING:
            RunQueueHint = 1;
            thread->state = 1;
            break;
        case OS_THREAD_STATE_READY:
            UnsetRun(thread);
            break;
        case OS_THREAD_STATE_WAITING:
            DEQUEUE_THREAD(thread, thread->queue, link);
            thread->priority = 0x20;
            ENQUEUE_THREAD(thread, thread->queue, link);
            if (thread->mutex) {
                ASSERTLINE(1263, thread->mutex->thread); 
                UpdatePriority(thread->mutex->thread);
            }
            break;
        }
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return suspendCount;
}

void OSSleepThread(OSThreadQueue* queue) {
    BOOL enabled = OSDisableInterrupts();
    OSThread* currentThread = OSGetCurrentThread();

    ASSERTMSGLINE(1296, currentThread, "OSSleepThread(): current thread does not exist.");
    ASSERTMSG1LINE(1298, __OSIsThreadActive(currentThread) != 0, "OSSleepThread(): current thread %p is not active.", currentThread);
    ASSERTMSG1LINE(1300, currentThread->state == OS_THREAD_STATE_RUNNING, "OSSleepThread(): current thread %p is not running.", currentThread);
    ASSERTMSG1LINE(1302, !(currentThread->suspend > 0), "OSSleepThread(): current thread %p is suspended.", currentThread);

    currentThread->state = OS_THREAD_STATE_WAITING;
    currentThread->queue = queue;
    ENQUEUE_THREAD_PRIO(currentThread, queue, link);
    RunQueueHint = 1;
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

void OSWakeupThread(OSThreadQueue* queue) {
    BOOL enabled = OSDisableInterrupts();

    while (queue->head) {
        OSThread* thread = queue->head;

        DEQUEUE_HEAD(thread, queue, link);

        ASSERTLINE(1331, __OSIsThreadActive(thread));
        ASSERTLINE(1332, thread->state != OS_THREAD_STATE_MORIBUND);
        ASSERTLINE(1333, thread->queue == queue);
        thread->state = OS_THREAD_STATE_READY;
        if (thread->suspend <= 0) {
            SetRun(thread);
        }
    }
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

int OSSetThreadPriority(OSThread* thread, s32 priority) {
    BOOL enabled;

    ASSERTMSGLINE(1359, (priority >= OS_PRIORITY_MIN) && (priority <= OS_PRIORITY_MAX), "OSSetThreadPriority(): priority out of range (0 <= priority <= 31).");

    if ((priority < OS_PRIORITY_MIN) || (priority > OS_PRIORITY_MAX)) {
        return 0;
    }
    enabled = OSDisableInterrupts();

    ASSERTMSG1LINE(1366, __OSIsThreadActive(thread) != 0, "OSSetThreadPriority(): thread %p is not active.", thread);
    ASSERTMSG1LINE(1368, thread->state != 8, "OSSetThreadPriority(): thread %p is terminated.", thread);

    if (thread->base != priority) {
        thread->base = priority;
        UpdatePriority(thread);
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return 1;
}

s32 OSGetThreadPriority(OSThread* thread) {
    return thread->base;
}

OSThread* OSSetIdleFunction(OSIdleFunction idleFunction, void* param, void* stack, u32 stackSize) {
    if (idleFunction) {
        if (IdleThread.state == 0) {
            OSCreateThread(&IdleThread, (void*)idleFunction, param, stack, stackSize, OS_PRIORITY_MAX, 1);
            OSResumeThread(&IdleThread);
            return &IdleThread;
        }
    } else if (IdleThread.state != 0) {
        OSCancelThread(&IdleThread);
    }
    return NULL;
}

OSThread* OSGetIdleFunction(void) {
    if (IdleThread.state != 0) {
        return &IdleThread;
    }
    return NULL;
}

static int CheckThreadQueue(OSThreadQueue* queue) {
    OSThread* thread;

    if ((queue->head != NULL) && (queue->head->link.prev != NULL)) {
        return 0;
    }
    if ((queue->tail != NULL) && (queue->tail->link.next != NULL)) {
        return 0;
    }
    thread = queue->head;
    while (thread) {
        if ((thread->link.next != NULL) && (thread != thread->link.next->link.prev)) {
            return 0;
        }
        if ((thread->link.prev != NULL) && (thread != thread->link.prev->link.next)) {
            return 0;
        }
        thread = thread->link.next;
    }
    return 1;
}

static BOOL IsMember(OSThreadQueue* queue, OSThread* thread) {
    OSThread* member = queue->head;

    while (member) {
        if (thread == member) {
            return TRUE;
        }
        member = member->link.next;
    }
    return FALSE;
}

// custom macro for OSCheckActiveThreads?
#define ASSERTREPORT(line, cond) \
    if (!(cond)) { \
        OSReport("OSCheckActiveThreads: Failed " #cond " in %d\n", line); \
        OSPanic(__FILE__, line, ""); \
    }

s32 OSCheckActiveThreads(void) {
    OSThread* thread;
    s32 prio;
    s32 cThread;
    BOOL enabled;

    cThread = 0;
    enabled = OSDisableInterrupts();
    
    for (prio = 0; prio <= OS_PRIORITY_MAX; prio++) {
        if (RunQueueBits & (1 << (OS_PRIORITY_MAX - prio))) {
            ASSERTREPORT(1522, RunQueue[prio].head != NULL && RunQueue[prio].tail != NULL);
        } else {
            ASSERTREPORT(1527, RunQueue[prio].head == NULL && RunQueue[prio].tail == NULL);
        }
        ASSERTREPORT(1529, CheckThreadQueue(&RunQueue[prio]));
    }

    ASSERTREPORT(1534, __OSActiveThreadQueue.head == NULL || __OSActiveThreadQueue.head->linkActive.prev == NULL);
    ASSERTREPORT(1536, __OSActiveThreadQueue.tail == NULL || __OSActiveThreadQueue.tail->linkActive.next == NULL);

    thread = __OSActiveThreadQueue.head;
    while (thread) {
        cThread++;
        ASSERTREPORT(1544, thread->linkActive.next == NULL || thread == thread->linkActive.next->linkActive.prev);
        ASSERTREPORT(1546, thread->linkActive.prev == NULL || thread == thread->linkActive.prev->linkActive.next);
        ASSERTREPORT(1549, *(thread->stackEnd) == OS_THREAD_STACK_MAGIC);
        ASSERTREPORT(1552, OS_PRIORITY_MIN <= thread->priority && thread->priority <= OS_PRIORITY_MAX+1);
        ASSERTREPORT(1553, 0 <= thread->suspend);
        ASSERTREPORT(1554, CheckThreadQueue(&thread->queueJoin));

        switch(thread->state) {
        case OS_THREAD_STATE_READY:
            if (thread->suspend <= 0) {
                ASSERTREPORT(1560, thread->queue == &RunQueue[thread->priority]);
                ASSERTREPORT(1561, IsMember(&RunQueue[thread->priority], thread));
                ASSERTREPORT(1562, thread->priority == __OSGetEffectivePriority(thread));
            }
            break;
        case OS_THREAD_STATE_RUNNING:
            ASSERTREPORT(1566, !IsSuspended(thread->suspend));
            ASSERTREPORT(1567, thread->queue == NULL);
            ASSERTREPORT(1568, thread->priority == __OSGetEffectivePriority(thread));
            break;
        case OS_THREAD_STATE_WAITING:
            ASSERTREPORT(1571, thread->queue != NULL);
            ASSERTREPORT(1572, CheckThreadQueue(thread->queue));
            ASSERTREPORT(1573, IsMember(thread->queue, thread));
            if (thread->suspend <= 0) {
                ASSERTREPORT(1576, thread->priority == __OSGetEffectivePriority(thread));
            } else {
                ASSERTREPORT(1580, thread->priority == 32);
            }
            ASSERTREPORT(1582, !__OSCheckDeadLock(thread));
            break;
        case OS_THREAD_STATE_MORIBUND:
            ASSERTREPORT(1586, thread->queueMutex.head == NULL && thread->queueMutex.tail == NULL);
            break;
        default:
            OSReport("OSCheckActiveThreads: Failed. unkown thread state (%d) of thread %p\n", thread->state, thread);
            OSPanic(__FILE__, 1592, "");
        }
        ASSERTREPORT(1597, __OSCheckMutexes(thread));
        thread = thread->linkActive.next;
    }
    OSRestoreInterrupts(enabled);
    return cThread;
}

void OSClearStack(u8 val) {
    __REGISTER u32 sp;
    __REGISTER u32* p;
    __REGISTER u32 pattern;
    
    pattern = (val << 24) | (val << 16) | (val << 8) | val;
    sp = OSGetStackPointer();
    for (p = __OSCurrentThread->stackEnd + 1; (u32)p < sp; ++p) {
        *p = pattern;
    }
}

void OSSetThreadSpecific(s32 index, void* ptr) {
    OSThread* thread;

    thread = __OSCurrentThread;
    ASSERTLINE(1573, 0 <= index && index < OS_THREAD_SPECIFIC_MAX);

    if (thread != 0 && index >= 0 && index < OS_THREAD_SPECIFIC_MAX) {
        thread->specific[index] = ptr;
    }
}

void* OSGetThreadSpecific(s32 index) {
    OSThread* thread;

    thread = __OSCurrentThread;
    ASSERTLINE(1584, 0 <= index && index < OS_THREAD_SPECIFIC_MAX);

    if (thread != 0 && index >= 0 && index < OS_THREAD_SPECIFIC_MAX) {
        return thread->specific[index];
    }

    return NULL;
}

#include "global.h"
extern u8 Debug_BBA_804516D0;
u8 Debug_BBA_804516D0 ATTRIBUTE_ALIGN(8);
