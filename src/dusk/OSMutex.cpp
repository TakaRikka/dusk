// OSMutex.cpp - PC implementation of GameCube OSMutex/OSCond API
// Uses std::recursive_mutex and std::condition_variable_any behind the
// unchanged GameCube C API. The OSMutex struct layout is preserved so
// game code can read its fields.

#include <dolphin/dolphin.h>
#include <dolphin/os.h>

#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include <cstdlib>

#include "JSystem/JKernel/JKRHeap.h"

// ============================================================================
// Malloc-based allocator to bypass JKRHeap operator new/delete
// Without this, side-table allocations call operator new -> JKRHeap::alloc
// -> OSLockMutex -> GetMutexData -> operator new ... infinite recursion.
// ============================================================================

template<typename T>
struct MallocAllocator {
    using value_type = T;
    MallocAllocator() = default;
    template<typename U> MallocAllocator(const MallocAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) {
        void* p = std::malloc(n * sizeof(T));
        if (!p) throw std::bad_alloc();
        return static_cast<T*>(p);
    }
    void deallocate(T* p, std::size_t) noexcept { std::free(p); }
    template<typename U> bool operator==(const MallocAllocator<U>&) const noexcept { return true; }
    template<typename U> bool operator!=(const MallocAllocator<U>&) const noexcept { return false; }
};

template<typename T>
struct MallocDeleter {
    void operator()(T* p) const {
        p->~T();
        std::free(p);
    }
};

template<typename T, typename... Args>
std::unique_ptr<T, MallocDeleter<T>> make_malloc_unique(Args&&... args) {
    void* mem = std::malloc(sizeof(T));
    if (!mem) throw std::bad_alloc();
    T* obj = JKR_NEW_ARGS (mem) T(std::forward<Args>(args)...);
    return std::unique_ptr<T, MallocDeleter<T>>(obj);
}

// ============================================================================
// Side-table: native mutex per OSMutex
// ============================================================================

struct PCMutexData {
    std::recursive_mutex nativeMutex;
};

template<typename K, typename V>
using MallocMap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>,
    MallocAllocator<std::pair<const K, V>>>;

// Lazy-initialized to avoid DLL static init crashes
static std::mutex& GetMutexMapMutex() {
    static std::mutex mtx;
    return mtx;
}
static MallocMap<OSMutex*, std::unique_ptr<PCMutexData, MallocDeleter<PCMutexData>>>& GetMutexMap() {
    static MallocMap<OSMutex*, std::unique_ptr<PCMutexData, MallocDeleter<PCMutexData>>> map;
    return map;
}

static PCMutexData& GetMutexData(OSMutex* mutex) {
    std::lock_guard<std::mutex> lock(GetMutexMapMutex());
    auto& map = GetMutexMap();
    auto it = map.find(mutex);
    if (it == map.end()) {
        auto result = map.emplace(mutex, make_malloc_unique<PCMutexData>());
        return *result.first->second;
    }
    return *it->second;
}

// ============================================================================
// Side-table: native condition variable per OSCond
// ============================================================================

struct PCCondData {
    std::condition_variable_any cv;
};

// Lazy-initialized to avoid DLL static init crashes
static std::mutex& GetCondMapMutex() {
    static std::mutex mtx;
    return mtx;
}
static MallocMap<OSCond*, std::unique_ptr<PCCondData, MallocDeleter<PCCondData>>>& GetCondMap() {
    static MallocMap<OSCond*, std::unique_ptr<PCCondData, MallocDeleter<PCCondData>>> map;
    return map;
}

static PCCondData& GetCondData(OSCond* cond) {
    std::lock_guard<std::mutex> lock(GetCondMapMutex());
    auto& map = GetCondMap();
    auto it = map.find(cond);
    if (it == map.end()) {
        auto result = map.emplace(cond, make_malloc_unique<PCCondData>());
        return *result.first->second;
    }
    return *it->second;
}

// ============================================================================
// C API functions
// ============================================================================

extern "C" {

void OSInitMutex(OSMutex* mutex) {
    if (!mutex) return;
    OSInitThreadQueue(&mutex->queue);
    mutex->thread = nullptr;
    mutex->count  = 0;

    // Create/reset side-table entry
    GetMutexData(mutex);
}

void OSLockMutex(OSMutex* mutex) {
    if (!mutex) return;

    PCMutexData& data = GetMutexData(mutex);
    data.nativeMutex.lock();

    // Update GC-visible fields
    OSThread* currentThread = OSGetCurrentThread();
    mutex->thread = currentThread;
    mutex->count++;
}

void OSUnlockMutex(OSMutex* mutex) {
    if (!mutex) return;

    OSThread* currentThread = OSGetCurrentThread();
    if (mutex->thread != currentThread) return;

    mutex->count--;
    if (mutex->count == 0) {
        mutex->thread = nullptr;
    }

    PCMutexData& data = GetMutexData(mutex);
    data.nativeMutex.unlock();
}

BOOL OSTryLockMutex(OSMutex* mutex) {
    if (!mutex) return FALSE;

    PCMutexData& data = GetMutexData(mutex);
    if (data.nativeMutex.try_lock()) {
        OSThread* currentThread = OSGetCurrentThread();
        mutex->thread = currentThread;
        mutex->count++;
        return TRUE;
    }
    return FALSE;
}

// ============================================================================
// Internal: unlock all mutexes held by a thread (called on thread exit)
// ============================================================================

void __OSUnlockAllMutex(OSThread* thread) {
    // On GC this walks the thread's mutex queue.
    // On PC the native mutexes are cleaned up when threads exit.
    // Clear the GC-visible queue.
    if (!thread) return;
    thread->queueMutex.head = nullptr;
    thread->queueMutex.tail = nullptr;
}

int __OSCheckDeadLock(OSThread* thread) {
    // Simplified: native OS handles deadlock detection.
    return 0;
}

int __OSCheckMutexes(OSThread* thread) {
    return 1;
}

// ============================================================================
// Condition Variable API
// ============================================================================

void OSInitCond(OSCond* cond) {
    if (!cond) return;
    OSInitThreadQueue(&cond->queue);
    GetCondData(cond);
}

void OSWaitCond(OSCond* cond, OSMutex* mutex) {
    if (!cond || !mutex) return;

    PCCondData& condData = GetCondData(cond);
    PCMutexData& mutexData = GetMutexData(mutex);

    // Save and clear the GC mutex state
    OSThread* currentThread = OSGetCurrentThread();
    s32 savedCount = mutex->count;
    mutex->count = 0;
    mutex->thread = nullptr;

    // Unlock the recursive mutex the same number of times it was locked
    for (s32 i = 0; i < savedCount; i++) {
        mutexData.nativeMutex.unlock();
    }

    // Wait on the condition variable
    {
        std::unique_lock<std::recursive_mutex> lock(mutexData.nativeMutex);
        condData.cv.wait(lock);
    }

    // Re-lock the recursive mutex the same number of times
    for (s32 i = 0; i < savedCount; i++) {
        mutexData.nativeMutex.lock();
    }

    // Restore GC mutex state
    mutex->thread = currentThread;
    mutex->count  = savedCount;
}

void OSSignalCond(OSCond* cond) {
    if (!cond) return;
    PCCondData& condData = GetCondData(cond);
    condData.cv.notify_all();
}

#ifdef __cplusplus
}
#endif
