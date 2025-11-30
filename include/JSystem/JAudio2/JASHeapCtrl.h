#ifndef JASHEAPCTRL_H
#define JASHEAPCTRL_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <dolphin/os.h>
#include <dolphin/os.h>

class JASDisposer;
class JKRHeap;
class JKRSolidHeap;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASHeap {
public:
    JASHeap(JASDisposer* param_0 = NULL);
    void initRootHeap(void*, u32);
    bool alloc(JASHeap*, u32);
    bool allocTail(JASHeap*, u32);
    bool free();
    void insertChild(JASHeap*, JASHeap*, void*, u32, bool);
    JASHeap* getTailHeap();
    u32 getTailOffset();
    u32 getCurOffset();

    void* getBase() { return mBase; }
    bool isAllocated() { return mBase; }
    u32 getSize() { return mSize; }

    /* 0x00 */ JSUTree<JASHeap> mTree;
    /* 0x1C */ OSMutex mMutex;
    /* 0x34 */ JASDisposer* mDisposer;
    /* 0x38 */ u8* mBase;
    /* 0x3C */ u32 mSize;
    /* 0x40 */ JASHeap* field_0x40;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASGenericMemPool {
    JASGenericMemPool();
    ~JASGenericMemPool();
    void newMemPool(u32, int);
    void* alloc(u32);
    void free(void*, u32);

    /* 0x00 */ void* field_0x0;
    /* 0x04 */ u32 freeMemCount;
    /* 0x08 */ u32 totalMemCount;
    /* 0x0C */ u32 usedMemCount;

};

namespace JASThreadingModel {
    template <typename A0>
    struct InterruptsDisable {
        struct Lock {
            Lock(const A0& param_0) { field_0x0 = OSDisableInterrupts(); }
            ~Lock() { OSRestoreInterrupts(field_0x0); }

            BOOL field_0x0;
        };
    };

    template <typename A0>
    struct ObjectLevelLockable : public OSMutex {
        ObjectLevelLockable() {
            OSInitMutex(this);
        }

        struct Lock {
            Lock(A0 const& mutex) {
                mMutex = (A0*)&mutex;
                OSLockMutex(mMutex);
            }

            ~Lock() {
                OSUnlockMutex(mMutex);
            }

            A0* mMutex;
        };
    };
};  // namespace JASThreadingModel

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename T>
class JASMemPool : public JASGenericMemPool {
public:
    void newMemPool(int param_0) { JASGenericMemPool::newMemPool(sizeof(T), param_0); }

    void* alloc(u32 n) {
        JUT_ASSERT(182, n == sizeof(T));
        return JASGenericMemPool::alloc(n);
    }

    void free(void* ptr, u32 n) {
        JUT_ASSERT(187, n == sizeof(T));
        JASGenericMemPool::free(ptr, n);
    }
};

namespace JASKernel { JKRHeap* getSystemHeap(); };

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<u32 ChunkSize, template<typename> class T>
class JASMemChunkPool : public T<JASMemChunkPool<ChunkSize, T> >::ObjectLevelLockable {
    struct MemoryChunk {
        MemoryChunk(MemoryChunk* nextChunk) {
            mNextChunk = nextChunk;
            mUsedSize = 0;
            mChunks = 0;
        }

        bool checkArea(const void* ptr) const {
            return (u8*)this + 0xc <= (u8*)ptr && (u8*)ptr < (u8*)this + (ChunkSize + 0xc);
        }

        MemoryChunk* getNextChunk() {
            return mNextChunk;
        }

        void* alloc(u32 size) {
            u8* rv = mBuffer + mUsedSize;
            mUsedSize += size;
            mChunks++;
            return rv;
        }

        void free(void* mem) {
            mChunks--;
        }

        bool isEmpty() const {
            return mChunks == 0;
        }

        void setNextChunk(MemoryChunk* chunk) {
            mNextChunk = chunk;
        }

        u32 getFreeSize() {
            return ChunkSize - mUsedSize;
        }

        void revive() {
            mUsedSize = 0;
        }

        MemoryChunk* mNextChunk;
        u32 mUsedSize;
        u32 mChunks;
        u8 mBuffer[ChunkSize];
    };
public:
    JASMemChunkPool() {
        field_0x18 = NULL;
        bool ret = createNewChunk();
        JUT_ASSERT(320, ret);
    }

    bool createNewChunk() {
        bool uVar2;
        if (field_0x18 != NULL && field_0x18->isEmpty()) {
            field_0x18->revive();
            uVar2 = 1;
        } else {
            MemoryChunk* pMVar4 = field_0x18;
            field_0x18 = new (JASKernel::getSystemHeap(), 0) MemoryChunk(pMVar4);
            if (field_0x18 != NULL) {
                uVar2 = 1;
            } else {
                JUT_WARN(428, "%s", "Not enough JASSystemHeap");
                field_0x18 = new (JKRHeap::getSystemHeap(), 0) MemoryChunk(pMVar4);
                if (field_0x18 != NULL) {
                    uVar2 = 1;
                } else {
                    field_0x18 = pMVar4;
                    uVar2 = 0;
                }
            }
        }
        return uVar2;
    }

    void* alloc(u32 size) {
        typename T<JASMemChunkPool<ChunkSize, T> >::Lock lock(*this);
        u32 freeSize = field_0x18->getFreeSize();
        if (freeSize < size) {
            if (ChunkSize < size) {
                return NULL;
            }
            if (createNewChunk() == 0) {
                return NULL;
            }
        }
        return field_0x18->alloc(size);
    }

    void free(void* ptr) {
        typename T<JASMemChunkPool<ChunkSize, T> >::Lock lock(*this);
        MemoryChunk* chunk = field_0x18;
        MemoryChunk* prevChunk = NULL;
        while (chunk != NULL) {
            if (chunk->checkArea(ptr)) {
                chunk->free(ptr);

                if (chunk != field_0x18 && chunk->isEmpty()) {
                    MemoryChunk* nextChunk = chunk->getNextChunk();
                    delete chunk;
                    prevChunk->setNextChunk(nextChunk);
                }
                return;
            }
            prevChunk = chunk;
            chunk = chunk->getNextChunk();
        }

        JUT_PANIC(362, "Cannnot free for JASMemChunkPool");
    }

    /* 0x18 */ MemoryChunk* field_0x18;
};

namespace JASKernel {
    void setupRootHeap(JKRSolidHeap*, u32);
    JKRHeap* getSystemHeap();
    JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>* getCommandHeap();
    void setupAramHeap(u32, u32);
    JASHeap* getAramHeap();

    extern JASHeap audioAramHeap;
    extern u32 sAramBase;
    extern JKRHeap* sSystemHeap;
    extern JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>* sCommandHeap;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename T>
class JASPoolAllocObject {
public:
    static void* operator new(size_t n) {
        JASMemPool<T>* memPool = getMemPool_();
        return memPool->alloc(sizeof(T));
    }
    static void* operator new(size_t n, void* ptr) {
        return ptr;
    }
    static void operator delete(void* ptr, size_t n) {
        JASMemPool<T>* memPool_ = getMemPool_();
        memPool_->free(ptr, sizeof(T));
    }
    static void newMemPool(int param_0) {
        JASMemPool<T>* memPool_ = getMemPool_();
        memPool_->newMemPool(param_0);
    }

private:
    static JASMemPool<T>* getMemPool_() {
        static JASMemPool<T> memPool_;
        return &memPool_;
    }
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename T>
class JASMemPool_MultiThreaded : public JASGenericMemPool {
public:
    void newMemPool(int param_0) {
        typename JASThreadingModel::InterruptsDisable<JASMemPool_MultiThreaded<T> >::Lock lock(*this);
        JASGenericMemPool::newMemPool(sizeof(T), param_0);
    }

    void* alloc(size_t count) {
        typename JASThreadingModel::InterruptsDisable<JASMemPool_MultiThreaded<T> >::Lock lock(*this);
        return JASGenericMemPool::alloc(count);
    }

    void free(void* ptr, u32 param_1) {
        typename JASThreadingModel::InterruptsDisable<JASMemPool_MultiThreaded<T> >::Lock lock(*this);
        JASGenericMemPool::free(ptr, param_1);
    }
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename T>
class JASPoolAllocObject_MultiThreaded {
public:
    static void* operator new(size_t n) {
        JASMemPool_MultiThreaded<T>* memPool_ = getMemPool();
        return memPool_->alloc(sizeof(T));
    }
    static void* operator new(size_t n, void* ptr) {
        return ptr;
    }
    static void operator delete(void* ptr, size_t n) {
        JASMemPool_MultiThreaded<T>* memPool_ = getMemPool();
        memPool_->free(ptr, sizeof(T));
    }

    static void newMemPool(int n) {
        getMemPool()->newMemPool(n);
    }

private:
    static JASMemPool_MultiThreaded<T>* getMemPool() {
        static JASMemPool_MultiThreaded<T> memPool_;
        return &memPool_;
    }
};

extern JKRSolidHeap* JASDram;

#endif /* JASHEAPCTRL_H */
