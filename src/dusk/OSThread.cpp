// OSThread.cpp - PC implementation of GameCube OSThread API
// Maps GameCube cooperative threading to native OS threads via std::thread.
// The OSThread struct layout is preserved so game code can read its fields.
// A side-table stores the native std::thread and synchronization primitives.

#include <dolphin/dolphin.h>
#include <dolphin/os.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <atomic>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <memory>

#include "JSystem/JKernel/JKRHeap.h"

// ============================================================================
// Side-table: native thread data per OSThread
// ============================================================================

struct PCThreadData {
    std::thread nativeThread;
    std::mutex mtx;
    std::condition_variable cv;
    void* (*func)(void*);
    void* param;
    bool started   = false;
    bool suspended = false;
};

// Lazy-initialized to avoid DLL static init crashes (used before DllMain completes)
static std::mutex& GetThreadDataMutex() {
    static std::mutex mtx;
    return mtx;
}
static std::unordered_map<OSThread*, std::unique_ptr<PCThreadData>>& GetThreadDataMap() {
    static std::unordered_map<OSThread*, std::unique_ptr<PCThreadData>> map;
    return map;
}

// Side-table for OSThreadQueue -> condition_variable (for OSSleepThread/OSWakeupThread)
static std::mutex& GetQueueCvMutex() {
    static std::mutex mtx;
    return mtx;
}
static std::unordered_map<OSThreadQueue*, std::unique_ptr<std::condition_variable>>& GetQueueCvMap() {
    static std::unordered_map<OSThreadQueue*, std::unique_ptr<std::condition_variable>> map;
    return map;
}

static std::condition_variable& GetQueueCV(OSThreadQueue* queue) {
    std::lock_guard<std::mutex> lock(GetQueueCvMutex());
    auto& map = GetQueueCvMap();
    auto it = map.find(queue);
    if (it == map.end()) {
        auto result = map.emplace(queue, std::make_unique<std::condition_variable>());
        return *result.first->second;
    }
    return *it->second;
}

// ============================================================================
// Thread-local current thread pointer
// ============================================================================

static thread_local OSThread* tls_currentThread = nullptr;

// ============================================================================
// Global state
// ============================================================================

static OSThread  sDefaultThread;
static u8        sDefaultStack[64 * 1024];
static u32       sDefaultStackEnd = OS_THREAD_STACK_MAGIC;

OSThreadQueue __OSActiveThreadQueue;

// Global interrupt mutex (coarse-grained lock replacing interrupt disable)
// Lazy-initialized to avoid DLL static init crashes
static std::recursive_mutex& GetInterruptMutex() {
    static std::recursive_mutex mtx;
    return mtx;
}
static thread_local int sInterruptLockCount = 0;

// Scheduler suspend count
static std::atomic<s32> sSchedulerSuspendCount{0};

// Active thread count
static std::atomic<s32> sActiveThreadCount{0};

// Switch thread callback
static OSSwitchThreadCallback sSwitchThreadCallback = nullptr;

// ============================================================================
// Internal helpers
// ============================================================================

// Linked list macros for the active thread queue
static void EnqueueActive(OSThread* thread) {
    OSThread* prev = __OSActiveThreadQueue.tail;
    if (prev == nullptr) {
        __OSActiveThreadQueue.head = thread;
    } else {
        prev->linkActive.next = thread;
    }
    thread->linkActive.prev = prev;
    thread->linkActive.next = nullptr;
    __OSActiveThreadQueue.tail = thread;
}

static void DequeueActive(OSThread* thread) {
    OSThread* next = thread->linkActive.next;
    OSThread* prev = thread->linkActive.prev;
    if (next == nullptr) {
        __OSActiveThreadQueue.tail = prev;
    } else {
        next->linkActive.prev = prev;
    }
    if (prev == nullptr) {
        __OSActiveThreadQueue.head = next;
    } else {
        prev->linkActive.next = next;
    }
    thread->linkActive.next = nullptr;
    thread->linkActive.prev = nullptr;
}

// Thread entry wrapper - runs on the new std::thread
static void ThreadEntryWrapper(OSThread* thread, PCThreadData* data) {
    // Set thread-local pointer
    tls_currentThread = thread;

    // Set context pointers for this thread
    OSClearContext(&thread->context);
    OSSetCurrentContext(&thread->context);

    thread->state = OS_THREAD_STATE_RUNNING;

    // Call the actual thread function
    void* result = data->func(data->param);

    // Thread returned - equivalent to OSExitThread
    thread->val   = result;
    thread->state = OS_THREAD_STATE_MORIBUND;
}

// ============================================================================
// C API functions
// ============================================================================

extern "C" {

void __OSThreadInit(void) {
    memset(&sDefaultThread, 0, sizeof(OSThread));

    sDefaultThread.state    = OS_THREAD_STATE_RUNNING;
    sDefaultThread.attr     = OS_THREAD_ATTR_DETACH;
    sDefaultThread.priority = 16;
    sDefaultThread.base     = 16;
    sDefaultThread.suspend  = 0;
    sDefaultThread.val      = (void*)(intptr_t)-1;
    sDefaultThread.mutex    = nullptr;
    sDefaultThread.queue    = nullptr;

    OSInitThreadQueue(&sDefaultThread.queueJoin);
    sDefaultThread.queueMutex.head = sDefaultThread.queueMutex.tail = nullptr;
    sDefaultThread.link.next = sDefaultThread.link.prev = nullptr;
    sDefaultThread.linkActive.next = sDefaultThread.linkActive.prev = nullptr;

    // Stack pointers (JKRThread reads these)
    sDefaultThread.stackBase = sDefaultStack + sizeof(sDefaultStack);
    sDefaultThread.stackEnd  = &sDefaultStackEnd;
    sDefaultStackEnd = OS_THREAD_STACK_MAGIC;

    OSClearContext(&sDefaultThread.context);

    sDefaultThread.error = 0;
    sDefaultThread.specific[0] = nullptr;
    sDefaultThread.specific[1] = nullptr;

    // Set as current thread for main thread
    tls_currentThread = &sDefaultThread;

    // Active queue
    OSInitThreadQueue(&__OSActiveThreadQueue);
    EnqueueActive(&sDefaultThread);
    sActiveThreadCount = 1;

    OSReport("[PC-OSThread] Thread system initialized (multi-threaded mode)\n");
}

// ============================================================================
// Thread Queue
// ============================================================================

void OSInitThreadQueue(OSThreadQueue* queue) {
    if (queue) {
        queue->head = queue->tail = nullptr;
    }
}

// ============================================================================
// Current Thread
// ============================================================================

OSThread* OSGetCurrentThread(void) {
    // Lazy-init for main thread if __OSThreadInit hasn't been called yet
    if (tls_currentThread == nullptr) {
        __OSThreadInit();
    }
    return tls_currentThread;
}

// ============================================================================
// Thread Creation
// ============================================================================

int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param,
                   void* stack, u32 stackSize, OSPriority priority, u16 attr) {
    if (!thread) return 0;
    if (priority < OS_PRIORITY_MIN || priority > OS_PRIORITY_MAX) return 0;

    // Ensure thread system is initialized
    OSGetCurrentThread();

    memset(thread, 0, sizeof(OSThread));

    thread->state    = OS_THREAD_STATE_READY;
    thread->attr     = attr & 1u;
    thread->base     = priority;
    thread->priority = priority;
    thread->suspend  = 1;  // Created suspended (GC behavior)
    thread->val      = (void*)(intptr_t)-1;
    thread->mutex    = nullptr;

    OSInitThreadQueue(&thread->queueJoin);
    thread->queueMutex.head = thread->queueMutex.tail = nullptr;
    thread->link.next = thread->link.prev = nullptr;
    thread->linkActive.next = thread->linkActive.prev = nullptr;

    // Stack (stack points to TOP on GameCube)
    thread->stackBase = (u8*)stack;
    thread->stackEnd  = (u32*)((uintptr_t)stack - stackSize);
    *thread->stackEnd = OS_THREAD_STACK_MAGIC;

    OSClearContext(&thread->context);

    thread->error = 0;
    thread->specific[0] = nullptr;
    thread->specific[1] = nullptr;

    // Create side-table entry (but don't start the thread yet)
    {
        auto data = std::make_unique<PCThreadData>();
        data->func  = func;
        data->param = param;

        std::lock_guard<std::mutex> lock(GetThreadDataMutex());
        GetThreadDataMap()[thread] = std::move(data);
    }

    // Add to active queue
    EnqueueActive(thread);
    sActiveThreadCount++;

    OSReport("[PC-OSThread] Created thread %p (priority=%d, stackSize=%u)\n",
             thread, priority, stackSize);
    return 1;
}

// ============================================================================
// Resume / Suspend
// ============================================================================
/*
s32 OSResumeThread(OSThread* thread) {
    if (!thread) return 0;

    s32 prevSuspend = thread->suspend;
    if (thread->suspend > 0) {
        thread->suspend--;
    }

    if (thread->suspend == 0) {
        std::lock_guard<std::mutex> lock(GetThreadDataMutex());
        auto it = GetThreadDataMap().find(thread);
        if (it != GetThreadDataMap().end()) {
            PCThreadData* data = it->second.get();
            if (!data->started) {
                // First resume: launch the native thread
                data->started = true;
                data->suspended = false;
                data->nativeThread = std::thread(ThreadEntryWrapper, thread, data);
                data->nativeThread.detach();
                OSReport("[PC-OSThread] Started thread %p\n", thread);
            } else if (data->suspended) {
                // Resume from suspension: signal the CV
                data->suspended = false;
                data->cv.notify_one();
            }
        }
    }

    return prevSuspend;
}

s32 OSSuspendThread(OSThread* thread) {
    if (!thread) return 0;

    s32 prevSuspend = thread->suspend;
    thread->suspend++;

    if (prevSuspend == 0) {
        std::lock_guard<std::mutex> lock(GetThreadDataMutex());
        auto it = GetThreadDataMap().find(thread);
        if (it != GetThreadDataMap().end()) {
            PCThreadData* data = it->second.get();
            if (data->started) {
                data->suspended = true;
                // The thread must check its suspended flag and wait
            }
        }
    }

    return prevSuspend;
}
*/

// ============================================================================
// Resume / Suspend
// ============================================================================

s32 OSResumeThread(OSThread* thread) {
    if (!thread)
        return 0;

    s32 prevSuspend = thread->suspend;
    if (thread->suspend > 0) {
        thread->suspend--;
    }

    // Only wake up if suspend count drops to 0
    if (thread->suspend == 0) {
        PCThreadData* data = nullptr;

        // Lock the global map to safely retrieve our thread data pointer
        {
            std::lock_guard<std::mutex> mapLock(GetThreadDataMutex());
            auto it = GetThreadDataMap().find(thread);
            if (it != GetThreadDataMap().end()) {
                data = it->second.get();
            }
        }

        if (data) {
            // Lock the specific thread mutex to safely modify state and notify
            std::unique_lock<std::mutex> threadLock(data->mtx);

            if (!data->started) {
                // First resume: launch the native thread
                data->started = true;
                data->suspended = false;

                // Unlock before launching to avoid potential deadlocks in thread initialization
                threadLock.unlock();

                data->nativeThread = std::thread(ThreadEntryWrapper, thread, data);
                data->nativeThread.detach();
                OSReport("[PC-OSThread] Started thread %p\n", thread);
            } else {
                // Resume from suspension: signal the condition variable
                // IMPORTANT: Set suspended to false BEFORE notifying to pass the wait predicate
                data->suspended = false;
                data->cv.notify_all();
            }
        }
    }

    return prevSuspend;
}

s32 OSSuspendThread(OSThread* thread) {
    if (!thread)
        return 0;

    s32 prevSuspend = thread->suspend;
    thread->suspend++;

    // If transitioning from running (0) to suspended (1)
    if (prevSuspend == 0) {
        PCThreadData* data = nullptr;

        // Lock the global map to find our thread data
        {
            std::lock_guard<std::mutex> mapLock(GetThreadDataMutex());
            auto it = GetThreadDataMap().find(thread);
            if (it != GetThreadDataMap().end()) {
                data = it->second.get();
            }
        }

        if (data && data->started) {
            std::unique_lock<std::mutex> threadLock(data->mtx);
            data->suspended = true;

            // FIX: If the thread is suspending ITSELF, we must block execution here.
            // This replicates the GameCube behavior where OSSuspendThread yields the CPU
            // immediately.
            if (thread == OSGetCurrentThread()) {
                // Block until 'suspended' becomes false (set by OSResumeThread)
                // The predicate protects against spurious wakeups.
                data->cv.wait(threadLock, [data] { return !data->suspended; });
            } else {
                // NOTE: Suspending *other* threads is difficult in C++ std::thread
                // without cooperative checkpoints or platform-specific hacks.
                // For now, we only set the flag. The target thread would need to check 'suspended'
                // periodically.
            }
        }
    }

    return prevSuspend;
}

// ============================================================================
// Sleep / Wakeup (thread queue based)
// ============================================================================

void OSSleepThread(OSThreadQueue* queue) {
    if (!queue) return;

    OSThread* currentThread = OSGetCurrentThread();
    if (!currentThread) return;

    currentThread->state = OS_THREAD_STATE_WAITING;
    currentThread->queue = queue;

    // Enqueue into the thread queue
    OSThread* prev = queue->tail;
    if (prev == nullptr) {
        queue->head = currentThread;
    } else {
        prev->link.next = currentThread;
    }
    currentThread->link.prev = prev;
    currentThread->link.next = nullptr;
    queue->tail = currentThread;

    // Wait on the condition variable for this queue
    std::condition_variable& cv = GetQueueCV(queue);
    std::unique_lock<std::mutex> lock(GetQueueCvMutex());
    cv.wait(lock, [currentThread]() {
        return currentThread->state != OS_THREAD_STATE_WAITING;
    });
}

void OSWakeupThread(OSThreadQueue* queue) {
    if (!queue) return;

    // Wake all threads in the queue
    OSThread* thread = queue->head;
    while (thread) {
        OSThread* next = thread->link.next;
        thread->state = OS_THREAD_STATE_READY;
        thread->link.next = nullptr;
        thread->link.prev = nullptr;
        thread->queue = nullptr;
        thread = next;
    }
    queue->head = queue->tail = nullptr;

    // Notify all waiters
    std::condition_variable& cv = GetQueueCV(queue);
    cv.notify_all();
}

// ============================================================================
// Exit / Cancel / Detach / Join
// ============================================================================

void OSExitThread(void* val) {
    OSThread* currentThread = OSGetCurrentThread();
    if (!currentThread) return;

    currentThread->val = val;

    if (currentThread->attr & OS_THREAD_ATTR_DETACH) {
        DequeueActive(currentThread);
        currentThread->state = 0;
    } else {
        currentThread->state = OS_THREAD_STATE_MORIBUND;
    }

    // Wake anyone waiting to join
    OSWakeupThread(&currentThread->queueJoin);
    sActiveThreadCount--;
}

void OSCancelThread(OSThread* thread) {
    if (!thread) return;

    if (thread->attr & OS_THREAD_ATTR_DETACH) {
        DequeueActive(thread);
        thread->state = 0;
    } else {
        thread->state = OS_THREAD_STATE_MORIBUND;
    }

    OSWakeupThread(&thread->queueJoin);
    sActiveThreadCount--;
}

void OSDetachThread(OSThread* thread) {
    if (!thread) return;
    thread->attr |= OS_THREAD_ATTR_DETACH;

    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        DequeueActive(thread);
        thread->state = 0;
    }
    OSWakeupThread(&thread->queueJoin);
}

int OSJoinThread(OSThread* thread, void* val) {
    if (!thread) return 0;

    if (!(thread->attr & OS_THREAD_ATTR_DETACH) &&
        thread->state != OS_THREAD_STATE_MORIBUND &&
        thread->queueJoin.head == nullptr) {
        OSSleepThread(&thread->queueJoin);
    }

    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        if (val) {
            *(s32*)val = (s32)(intptr_t)thread->val;
        }
        DequeueActive(thread);
        thread->state = 0;
        return 1;
    }
    return 0;
}

// ============================================================================
// Yield / Terminated / Active
// ============================================================================

void OSYieldThread(void) {
    std::this_thread::yield();
}

BOOL OSIsThreadSuspended(OSThread* thread) {
    return (thread && thread->suspend > 0) ? TRUE : FALSE;
}

BOOL OSIsThreadTerminated(OSThread* thread) {
    if (!thread) return TRUE;
    return (thread->state == OS_THREAD_STATE_MORIBUND || thread->state == 0) ? TRUE : FALSE;
}

s32 OSCheckActiveThreads(void) {
    return sActiveThreadCount.load();
}

// ============================================================================
// Priority
// ============================================================================

int OSSetThreadPriority(OSThread* thread, OSPriority priority) {
    if (!thread) return 0;
    if (priority < OS_PRIORITY_MIN || priority > OS_PRIORITY_MAX) return 0;
    thread->base = priority;
    thread->priority = priority;
    return 1;
}

s32 OSGetThreadPriority(OSThread* thread) {
    if (!thread) return 16;
    return thread->base;
}

// ============================================================================
// Switch Thread Callback
// ============================================================================

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback callback) {
    OSSwitchThreadCallback prev = sSwitchThreadCallback;
    sSwitchThreadCallback = callback;
    return prev;
}

// ============================================================================
// Scheduler (atomic counter, no real effect with native OS threads)
// ============================================================================

s32 OSDisableScheduler(void) {
    return sSchedulerSuspendCount.fetch_add(1);
}

s32 OSEnableScheduler(void) {
    return sSchedulerSuspendCount.fetch_sub(1);
}

// ============================================================================
// Interrupts (global recursive mutex for mutual exclusion)
// ============================================================================

BOOL OSDisableInterrupts(void) {
    GetInterruptMutex().lock();
    sInterruptLockCount++;
    return (BOOL)(sInterruptLockCount > 1);  // TRUE if was already locked
}

BOOL OSRestoreInterrupts(BOOL level) {
    if (sInterruptLockCount > 0) {
        sInterruptLockCount--;
        GetInterruptMutex().unlock();
    }
    return level;
}

BOOL OSEnableInterrupts(void) {
    if (sInterruptLockCount > 0) {
        sInterruptLockCount--;
        GetInterruptMutex().unlock();
    }
    return FALSE;
}

// ============================================================================
// Idle function (stub on PC)
// ============================================================================

OSThread* OSSetIdleFunction(OSIdleFunction idleFunction, void* param, void* stack, u32 stackSize) {
    return nullptr;
}

OSThread* OSGetIdleFunction(void) {
    return nullptr;
}

// ============================================================================
// Thread-specific storage
// ============================================================================

void OSSetThreadSpecific(s32 index, void* ptr) {
    OSThread* thread = OSGetCurrentThread();
    if (thread && index >= 0 && index < OS_THREAD_SPECIFIC_MAX) {
        thread->specific[index] = ptr;
    }
}

void* OSGetThreadSpecific(s32 index) {
    OSThread* thread = OSGetCurrentThread();
    if (thread && index >= 0 && index < OS_THREAD_SPECIFIC_MAX) {
        return thread->specific[index];
    }
    return nullptr;
}

// ============================================================================
// Clear stack (minimal implementation)
// ============================================================================

void OSClearStack(u8 val) {
    // On PC we don't clear the stack - it's managed by the OS
}

// ============================================================================
// Internal functions used by OSMutex
// ============================================================================

s32 __OSGetEffectivePriority(OSThread* thread) {
    // On PC with native threads, priority inversion handling is simplified.
    // Just return the base priority.
    return thread ? thread->base : 16;
}

void __OSPromoteThread(OSThread* thread, s32 priority) {
    // Simplified: no real priority inheritance on PC
    if (thread && priority < thread->priority) {
        thread->priority = priority;
    }
}

void __OSReschedule(void) {
    // With native OS threads, rescheduling is handled by the OS.
    // Nothing to do here.
}

// ============================================================================
// Interrupt handler registration (stub)
// ============================================================================

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt,
                                             __OSInterruptHandler handler) {
    return nullptr;
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask mask) {
    return 0;
}

#ifdef __cplusplus
}
#endif
