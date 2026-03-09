#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "JSystem/JKernel/JKRDisposer.h"
#include <os.h>
#include "global.h"
#include <new>
#include <cstdint>

class JKRHeap;
typedef void (*JKRErrorHandler)(void*, u32, int);

extern u8 JKRValue_DEBUGFILL_NOTUSE;
extern u8 JKRValue_DEBUGFILL_NEW;
extern u8 JKRValue_DEBUGFILL_DELETE;

extern s32 fillcheck_dispcount;
extern bool data_8074A8D0_debug;

#if BIT_64
#define MEM_BLOCK_SIZE 0x20
#else
#define MEM_BLOCK_SIZE 0x10
#endif

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRHeap : public JKRDisposer {
public:
    typedef void (*JKRAllocCallback)(u32, int, JKRHeap*, void*);
    typedef void (*JKRFreeCallback)(void*, JKRHeap*);

    class TState {
    public:
        /* 0x00 */ u32 mUsedSize;
        /* 0x04 */ u32 mCheckCode;
        /* 0x08 */ u32 mBuf;
        /* 0x0C */ u32 field_0xc;
        /* 0x10 */ JKRHeap* mHeap;
        /* 0x14 */ u32 mId;

    public:
        u32 getUsedSize() const { return mUsedSize; }
        u32 getCheckCode() const { return mCheckCode; }
        JKRHeap* getHeap() const { return mHeap; }
        u32 getId() const { return mId; }
    };

public:
    JKRHeap(u32 size, JKRHeap* parent, bool errorFlag);
    JKRHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);
    virtual ~JKRHeap();

    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();
    void destroy();

    void* alloc(u32 size, int alignment);
    void free(void* ptr);
    void freeAll();
    void freeTail();
    s32 resize(void* ptr, u32 size);
    s32 getSize(void* ptr);
    s32 getFreeSize();
    void* getMaxFreeBlock();
    s32 getTotalFreeSize();
    s32 changeGroupID(u8 newGroupId);
    u8 getCurrentGroupId();
    u32 getMaxAllocatableSize(int alignment);

    JKRHeap* find(void* ptr) const;
    JKRHeap* findAllHeap(void* ptr) const;

    void dispose_subroutine(uintptr_t start, uintptr_t end);
    bool dispose(void* ptr, u32 size);
    void dispose(void* begin, void* end);
    void dispose();

    bool setErrorFlag(bool errorFlag);
    bool isSubHeap(JKRHeap* heap) const;
    void* getAltAramStartAdr();

    /* vt[03] */ virtual void callAllDisposer();
    /* vt[04] */ virtual u32 getHeapType() = 0;
    /* vt[05] */ virtual bool check() = 0;
    /* vt[06] */ virtual bool dump_sort();
    /* vt[07] */ virtual bool dump() = 0;
    /* vt[08] */ virtual void do_destroy() = 0;
    /* vt[09] */ virtual void* do_alloc(u32 size, int alignment) = 0;
    /* vt[10] */ virtual void do_free(void* ptr) = 0;
    /* vt[11] */ virtual void do_freeAll() = 0;
    /* vt[12] */ virtual void do_freeTail() = 0;
    /* vt[13] */ virtual void do_fillFreeArea() = 0;
    /* vt[14] */ virtual s32 do_resize(void* ptr, u32 size) = 0;
    /* vt[15] */ virtual s32 do_getSize(void* ptr) = 0;
    /* vt[16] */ virtual s32 do_getFreeSize() = 0;
    /* vt[17] */ virtual void* do_getMaxFreeBlock() = 0;
    /* vt[18] */ virtual s32 do_getTotalFreeSize() = 0;
    /* vt[19] */ virtual s32 do_changeGroupID(u8 newGroupID);
    /* vt[20] */ virtual u8 do_getCurrentGroupId();
    /* vt[21] */ virtual void state_register(JKRHeap::TState* p, u32 id) const;
    /* vt[22] */ virtual bool state_compare(JKRHeap::TState const& r1, JKRHeap::TState const& r2) const;
    /* vt[23] */ virtual void state_dump(JKRHeap::TState const& p) const;

    void setDebugFill(bool debugFill) { mDebugFill = debugFill; }
    bool getDebugFill() const { return mDebugFill; }
    void* getStartAddr() { return (void*)mStart; }
    void* getEndAddr() { return (void*)mEnd; }
    u32 getSize() const { return mSize; }
    bool getErrorFlag() const { return mErrorFlag; }
    void callErrorHandler(void* heap, u32 size, int alignment) {
        if (mErrorHandler) {
            (*mErrorHandler)(heap, size, alignment);
        }
    }

    JKRHeap* getParent() { return mChildTree.getParent()->getObject(); }

    JSUTree<JKRHeap>& getHeapTree() { return mChildTree; }
    void appendDisposer(JKRDisposer* disposer) { mDisposerList.append(&disposer->mLink); }
    void removeDisposer(JKRDisposer* disposer) { mDisposerList.remove(&disposer->mLink); }
    void lock() const { OSLockMutex(const_cast<OSMutex*>(&mMutex)); }
    void unlock() const { OSUnlockMutex(const_cast<OSMutex*>(&mMutex)); }
    u32 getHeapSize() { return mSize; }

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    /* 0x18 */ OSMutex mMutex;
    /* 0x30 */ u8* mStart;
    /* 0x34 */ u8* mEnd;
    /* 0x38 */ u32 mSize;
    /* 0x3C */ bool mDebugFill;
    /* 0x3D */ bool mCheckMemoryFilled;
    /* 0x3E */ u8 mAllocationMode;  // EAllocMode?
    /* 0x3F */ u8 mGroupId;
    /* 0x40 */ JSUTree<JKRHeap> mChildTree;
    /* 0x5C */ JSUList<JKRDisposer> mDisposerList;
    /* 0x68 */ bool mErrorFlag;
    /* 0x69 */ bool mInitFlag;

#if TARGET_PC
    char mName[32];
#endif

public:
    static bool initArena(char** memory, u32* size, int maxHeaps);
    static bool initArena2(char** memory, u32* size, int maxHeaps);
    static void* alloc(u32 size, int alignment, JKRHeap* heap);
    static void free(void* ptr, JKRHeap* heap);
    static s32 resize(void* ptr, u32 size, JKRHeap* heap);
    static s32 getSize(void* ptr, JKRHeap* heap);
    static JKRHeap* findFromRoot(void* ptr);

    static void copyMemory(void* dst, void* src, u32 size);
    static void fillMemory(void* dst, u32 size, u8 value);  // NOTE: never used
    static bool checkMemoryFilled(void* src, u32 size, u8 value);

    static JKRErrorHandler setErrorHandler(JKRErrorHandler errorHandler);
    static void fillMemory(u8*, u32, u8);
    static bool checkMemoryFilled(u8*, u32, u8);

    static bool isDefaultDebugFill() { return sDefaultFillFlag; }
    static void setDefaultDebugFill(bool status) { sDefaultFillFlag = status; }
    static void* getCodeStart(void) { return mCodeStart; }
    static void* getCodeEnd(void) { return mCodeEnd; }
    static void* getUserRamStart(void) { return mUserRamStart; }
    static void* getUserRamEnd(void) { return mUserRamEnd; }
    static u32 getMemorySize(void) { return mMemorySize; }
    static JKRHeap* getRootHeap() { return sRootHeap; }

    static JKRHeap* getRootHeap2() { return sRootHeap2; }

    static JKRHeap* getSystemHeap() { return sSystemHeap; }
#if TARGET_PC
    static JKRHeap* getCurrentHeap();
#else
    static JKRHeap* getCurrentHeap() { return sCurrentHeap; }
#endif
    static void setSystemHeap(JKRHeap* heap) { sSystemHeap = heap; }
#if TARGET_PC
    static void setCurrentHeap(JKRHeap* heap);
#else
    static void setCurrentHeap(JKRHeap* heap) { sCurrentHeap = heap; }
#endif

    static void setState_u32ID_(TState* state, u32 id) { state->mId = id; }
    static void setState_uUsedSize_(TState* state, u32 usedSize) { state->mUsedSize = usedSize; }
    static void setState_u32CheckCode_(TState* state, u32 checkCode) {
        state->mCheckCode = checkCode;
    }
    static void* getState_buf_(TState* state) { return &state->mBuf; }

    static void* mCodeStart;
    static void* mCodeEnd;
    static void* mUserRamStart;
    static void* mUserRamEnd;
    static u32 mMemorySize;
    static JKRAllocCallback sAllocCallback;
    static JKRFreeCallback sFreeCallback;

    static bool sDefaultFillFlag;

    static JKRHeap* sRootHeap;

    static JKRHeap* sRootHeap2;

    static JKRHeap* sSystemHeap;
#if !TARGET_PC // Hide sCurrentHeap, we need to make it thread local.
    static JKRHeap* sCurrentHeap;
#endif

    static JKRErrorHandler mErrorHandler;

#if TARGET_PC
    void setName(const char* name);
    const char* getName() const;

#define JKRHEAP_NAME(heap, name) (heap)->setName(name)
#else
#define JKRHEAP_NAME(heap, name)
#endif
};

#if TARGET_PC
enum class JKRHeapToken {
    Dummy
};

inline void* operator new(size_t, JKRHeapToken, void* where) {
    return where;
}

inline void* operator new[](size_t, JKRHeapToken, void* where) {
    return where;
}

#define JKR_NEW new (JKRHeapToken::Dummy)
#define JKR_NEW_ARGS(...) new (JKRHeapToken::Dummy, __VA_ARGS__ )
#define JKR_DELETE(expr) jkrDelete(expr)
#define JKR_DELETE_ARRAY(expr) jkrDeleteArray(expr)
#define JKR_HEAP_TOKEN , JKRHeapToken::Dummy
#define JKR_HEAP_TOKEN_PARAM , JKRHeapToken
#else
#define JKR_NEW new
#define JKR_NEW_ARGS(...) new (__VA_ARGS__ )
#define JKR_DELETE(expr) delete (expr)
#define JKR_DELETE_ARRAY(expr) delete[] (expr)
#define JKR_HEAP_TOKEN
#define JKR_HEAP_TOKEN_PARAM
#endif

void* operator new(size_t size JKR_HEAP_TOKEN_PARAM);
void* operator new(size_t size JKR_HEAP_TOKEN_PARAM, int alignment);
void* operator new(size_t size JKR_HEAP_TOKEN_PARAM, JKRHeap* heap, int alignment);

void* operator new[](size_t size JKR_HEAP_TOKEN_PARAM);
void* operator new[](size_t size JKR_HEAP_TOKEN_PARAM, int alignment);
void* operator new[](size_t size JKR_HEAP_TOKEN_PARAM, JKRHeap* heap, int alignment);

void operator delete(void* ptr JKR_HEAP_TOKEN_PARAM);
void operator delete[](void* ptr JKR_HEAP_TOKEN_PARAM);

#if TARGET_PC
template<typename T>
void jkrDelete(T* ptr) {
    if (ptr == nullptr) {
        return;
    }
    ptr->~T();
    operator delete(ptr, JKRHeapToken::Dummy);
}

template<>
inline void jkrDelete(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    operator delete(ptr, JKRHeapToken::Dummy);
}

static void jkrDeleteArray(void* ptr) {
    operator delete[](ptr, JKRHeapToken::Dummy);
}
#endif

void JKRDefaultMemoryErrorRoutine(void* heap, u32 size, int alignment);

inline void* JKRAllocFromHeap(JKRHeap* heap, u32 size, int alignment) {
    return JKRHeap::alloc(size, alignment, heap);
}

inline void* JKRAllocFromSysHeap(u32 size, int alignment) {
    return JKRHeap::getSystemHeap()->alloc(size, alignment);
}

inline void JKRFreeToHeap(JKRHeap* heap, void* ptr) {
    JKRHeap::free(ptr, heap);
}

inline void JKRFreeToSysHeap(void* ptr) {
    JKRHeap::getSystemHeap()->free(ptr);
}

inline void JKRFree(void* ptr) {
    JKRHeap::free(ptr, NULL);
}

inline void JKRFillMemory(u8* dst, u32 size, u8 val) {
    JKRHeap::fillMemory(dst, size, val);
}

inline JKRHeap* JKRGetSystemHeap() {
    return JKRHeap::getSystemHeap();
}

inline JKRHeap* JKRGetCurrentHeap() {
    return JKRHeap::getCurrentHeap();
}

inline JKRHeap* JKRSetCurrentHeap(JKRHeap* heap) {
    return heap->becomeCurrentHeap();
}

inline u32 JKRGetMemBlockSize(JKRHeap* heap, void* block) {
    return JKRHeap::getSize(block, heap);
}

inline u32 JKRGetFreeSize(JKRHeap* heap) {
    return heap->getFreeSize();
}

inline void* JKRAlloc(u32 size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}

inline s32 JKRResizeMemBlock(JKRHeap* heap, void* ptr, u32 size) {
    return JKRHeap::resize(ptr, size, heap);
}

inline JKRHeap* JKRFindHeap(void* ptr) {
    return JKRHeap::findFromRoot(ptr);
}

inline JKRHeap* JKRGetRootHeap() {
    return JKRHeap::getRootHeap();
}

inline JKRErrorHandler JKRSetErrorHandler(JKRErrorHandler errorHandler) {
    return JKRHeap::setErrorHandler(errorHandler);
}

inline bool JKRSetErrorFlag(JKRHeap* heap, bool flag) {
    return heap->setErrorFlag(flag);
}

inline JKRHeap* JKRGetRootHeap2() {
    return JKRHeap::getRootHeap2();
}

#if DEBUG
inline void JKRSetDebugFillNotuse(u8 status) { JKRValue_DEBUGFILL_NOTUSE = status; }
inline void JKRSetDebugFillNew(u8 status) { JKRValue_DEBUGFILL_NEW = status; }
inline void JKRSetDebugFillDelete(u8 status) { JKRValue_DEBUGFILL_DELETE = status; }
#endif

#endif /* JKRHEAP_H */
