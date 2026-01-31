#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <dolphin/ar.h>

JASHeap::JASHeap(JASDisposer* disposer) : mTree(this) {
    mDisposer = disposer;
    mBase = NULL;
    mSize = 0;
    field_0x40 = 0;
    OSInitMutex(&mMutex);
}

void JASHeap::initRootHeap(void* param_0, u32 param_1) {
    JUT_ASSERT(97, ! isAllocated());
    JASMutexLock lock(&mMutex);
    mBase = (u8*)OSRoundUp32B(param_0);
    field_0x40 = NULL;
    mSize = param_1 - (u32(mBase) - u32(param_0));
}

bool JASHeap::alloc(JASHeap* mother, u32 param_1) {
    JUT_ASSERT(120, mother != NULL);
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        OS_REPORT("[JASHeap::alloc] すでにヒープは確保されています。初期化してからにしてください。\n");
        return 0;
    }
    if (!mother->isAllocated()) {
        return 0;
    }
    param_1 = OSRoundUp32B(param_1);
    u32 local_28 = mother->getCurOffset();
    u32 local_2c = mother->getTailOffset();
    if (local_28 + param_1 <= local_2c) {
        mother->insertChild(this, mother->getTailHeap(), mother->mBase + local_28, param_1, false);
        return 1;
    }
    s32 r27 = -1;
    u8* r29 = mother->mBase;
    bool local_43 = false;
    JASHeap* local_30 = NULL;
    void* local_34;
    JSUTreeIterator<JASHeap> it;
    for (it = mother->mTree.getFirstChild(); it != mother->mTree.getEndChild(); it++) {
        if (r29 >= mother->mBase + local_2c) {
            break;
        }
        u32 local_3c = u32(it->mBase) - u32(r29);
        if (local_3c >= param_1 && local_3c < r27) {
            local_30 = *it;
            local_34 = r29;
            r27 = local_3c;
            local_43 = true;
        }
        u32 r25 = it->mSize;
        r29 = (u8*)it->mBase + r25;
    }
    if (r29 != mother->mBase && r29 < mother->mBase + local_2c) {
        u32 local_40 = mother->mBase + mother->mSize - r29;
        if (local_40 >= param_1 && local_40 < r27) {
            local_30 = NULL;
            local_34 = r29;
            r27 = local_40;
            local_43 = true;
        }
    }
    if (!local_43) {
        OS_REPORT("[JASHeap::alloc] マザーメモリが足りないので確保できません。\n");
        return 0;
    }
    mother->insertChild(this, local_30, local_34, param_1, false);
    return 1;
}

bool JASHeap::allocTail(JASHeap* mother, u32 size) {
    JUT_ASSERT(208, mother != NULL);
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        OS_REPORT("[JASHeap::alloc] すでにヒープは確保されています。初期化してからにしてください。\n");
        return false;
    }
    if (!mother->isAllocated()) {
        return false;
    }
    u32 aligned_size = (size + 0x1f) & ~0x1f;
    u32 cur_offset = mother->getCurOffset();
    u32 tail_offset = mother->getTailOffset();
    if (cur_offset + aligned_size > tail_offset) {
        return false;
    }
    mother->insertChild(this, mother->getTailHeap(), mother->mBase + tail_offset - aligned_size,
                        aligned_size, true);
    return true;
}

bool JASHeap::free() {
    JASMutexLock lock(&mMutex);
    if (!isAllocated()) {
        return false;
    }
    JSUTreeIterator<JASHeap> stack_20;
    for (JSUTreeIterator<JASHeap> it(mTree.getFirstChild()); it != mTree.getEndChild(); it = stack_20) {
        stack_20 = it;
        ++stack_20;
        it->free();
    }
    JSUTree<JASHeap>* parentTree = mTree.getParent();
    if (parentTree) {
        JASHeap* parentHeap = parentTree->getObject();
        if (parentHeap->field_0x40 == this) {
            JSUTreeIterator<JASHeap> stack_28(mTree.getPrevChild());
            if (stack_28 != mTree.getEndChild()) {
                parentHeap->field_0x40 = *stack_28;
            } else {
                parentHeap->field_0x40 = NULL;
            }
        }
        parentTree->removeChild(&mTree);
    }
    mBase = NULL;
    field_0x40 = NULL;
    mSize = 0;
    if (mDisposer) {
        mDisposer->onDispose();
    }
    return true;
}

u32 JASHeap::getTotalFreeSize() const {
    JASMutexLock lock(const_cast<OSMutex*>(&mMutex));
    u8* r28 = mBase;
    u32 r29 = 0;
    for (JSUTreeIterator<JASHeap> it = mTree.getFirstChild(); it != mTree.getEndChild(); it++) {
        r29 += it->mBase - r28;
        u32 r26 = it->mSize;
        r28 = it->mBase + r26;
    }
    r29 += mBase + mSize - r28;
    return r29;
}

u32 JASHeap::getFreeSize() const {
    JASMutexLock lock(const_cast<OSMutex*>(&mMutex));
    u8* r27 = mBase;
    u32 r29 = 0;
    u32 r28;
    for (JSUTreeIterator<JASHeap> it = mTree.getFirstChild(); it != mTree.getEndChild(); it++) {
        r28 = it->mBase - r27;
        if (r28 > r29) {
            r29 = r28;
        }
        u32 r25 = it->mSize;
        r27 = it->mBase + r25;
    }
    r28 = mBase + mSize - r27;
    if (r28 > r29) {
        r29 = r28;
    }
    return r29;
}

void JASHeap::insertChild(JASHeap* heap, JASHeap* next, void* param_2, u32 param_3, bool param_4) {
    JUT_ASSERT(513, heap != NULL);
    JUT_ASSERT(514, next == NULL || &mTree == next->mTree.getParent());
    JASMutexLock lock(&mMutex);
    if (!param_4) {
        JSUTreeIterator<JASHeap> it;
        if (!next) {
            it = mTree.getLastChild();
        } else {
            it = next->mTree.getPrevChild();
        }
        JASHeap* r24 = it != mTree.getEndChild() ? it.getObject() : NULL;
        if (field_0x40 == r24) {
            field_0x40 = heap;
        }
    }
    heap->mBase = (u8*)param_2;
    heap->mSize = param_3;
    heap->field_0x40 = NULL;
    mTree.insertChild(&next->mTree, &heap->mTree);
}

JASHeap* JASHeap::getTailHeap() {
    int r30 = 0;
    JSUTreeIterator<JASHeap> it;
    JASMutexLock lock(&mMutex);
    if (!field_0x40) {
        it = mTree.getFirstChild();
    } else {
        it = field_0x40->mTree.getNextChild();
    }
    if (it == mTree.getEndChild()) {
        return NULL;
    }
    return it.getObject();
}

u32 JASHeap::getTailOffset() {
    u32 offset = 0;
    JASMutexLock lock(&mMutex);
    JASHeap* heap = getTailHeap();
    if (heap == NULL) {
        offset = mSize;
    } else {
        offset = heap->mBase - mBase;
    }
    return offset;
}

u32 JASHeap::getCurOffset() {
    u32 offset = 0;
    JASMutexLock lock(&mMutex);
    if (field_0x40 == NULL) {
        offset = 0;
    } else {
        offset = field_0x40->mBase + field_0x40->mSize - mBase;
    }
    return offset;
}

JASGenericMemPool::JASGenericMemPool() {
    field_0x0 = NULL;
    freeMemCount = 0;
    totalMemCount = 0;
    usedMemCount = 0;
}

JASGenericMemPool::~JASGenericMemPool() {
    void* chunk = field_0x0;
    while (chunk != NULL) {
        void* next_chunk = *(void**)chunk;
        delete[] chunk;
        chunk = next_chunk;
    }
}

JKRSolidHeap* JASDram;

// TODO: What is this and Where does it go?
struct TNextOnFreeList {
    u8 pad[4];
};  // Size: 0x4

void JASGenericMemPool::newMemPool(u32 n, int param_1) {
    JUT_ASSERT(734, n >= sizeof(TNextOnFreeList));
    void* runner;
    for (int i = 0; i < param_1; i++) {
        runner = new (JASDram, 0) u8[n];
        JUT_ASSERT(739, runner);
        *(void**)runner = field_0x0;
        field_0x0 = runner;
    }
    freeMemCount += param_1;
    totalMemCount += param_1;
}

void* JASGenericMemPool::alloc(u32 param_0) {
    if (field_0x0 == NULL) {
        return NULL;
    }
    void* chunk = field_0x0;
    field_0x0 = *(void**)chunk;
    freeMemCount--;
    if (usedMemCount < totalMemCount - freeMemCount) {
        usedMemCount = totalMemCount - freeMemCount;
    }
    return chunk;
}

void JASGenericMemPool::free(void* ptr, u32 param_1) {
    if (!ptr) {
        return;
    }
    void* chunk = ptr;
    *(void**)chunk = field_0x0;
    field_0x0 = chunk;
    freeMemCount++;
}

u32 JASKernel::sAramBase;

JKRHeap* JASKernel::sSystemHeap;

JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>* JASKernel::sCommandHeap;

void JASKernel::setupRootHeap(JKRSolidHeap* heap, u32 size) {
    JUT_ASSERT(784, heap);
    sSystemHeap = JKRExpHeap::create(size, heap, false);
    JUT_ASSERT(787, sSystemHeap);
    sCommandHeap = new (heap, 0) JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>();
    JUT_ASSERT(790, sCommandHeap);
    JASDram = heap;
}

JKRHeap* JASKernel::getSystemHeap() {
    return JASKernel::sSystemHeap;
}

JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>* JASKernel::getCommandHeap() {
    return JASKernel::sCommandHeap;
}

JASHeap JASKernel::audioAramHeap;

void JASKernel::setupAramHeap(u32 param_0, u32 param_1) {
#if !PLATFORM_GCN
    OSReport("setupAramHeap %x, %x, %x\n", param_0, ARGetBaseAddress(), param_1);
    param_0 = ARGetBaseAddress();
#endif
    sAramBase = param_0;
    audioAramHeap.initRootHeap((void*)sAramBase, param_1);
}

JASHeap* JASKernel::getAramHeap() {
    return &audioAramHeap;
}

u32 JASKernel::getAramFreeSize() {
    return audioAramHeap.getFreeSize();
}

u32 JASKernel::getAramSize() {
    return audioAramHeap.getSize();
}
