#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

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
    for (JSUTreeIterator<JASHeap> it = mother->mTree.getFirstChild(); it != mother->mTree.getEndChild(); it++) {
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
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
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
    JASMutexLock lock(&mMutex);
    JASHeap* heap = getTailHeap();
    u32 offset = !heap ? mSize : heap->mBase - mBase;
    return offset;
}

u32 JASHeap::getCurOffset() {
    JASMutexLock lock(&mMutex);
    u32 offset = !field_0x40 ? 0 : field_0x40->mBase + field_0x40->mSize - mBase;
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

void JASGenericMemPool::newMemPool(u32 size, int param_1) {
    for (int i = 0; i < param_1; i++) {
        void* chunk = new (JASDram, 0) u8[size];
        *(void**)chunk = field_0x0;
        field_0x0 = chunk;
    }
    freeMemCount += param_1;
    totalMemCount += param_1;
}

void* JASGenericMemPool::alloc(u32 param_0) {
    void* chunk = field_0x0;
    if (chunk == NULL) {
        return NULL;
    }
    field_0x0 = *(void**)chunk;
    freeMemCount--;
    if (usedMemCount < totalMemCount - freeMemCount) {
        usedMemCount = totalMemCount - freeMemCount;
    }
    return chunk;
}

void JASGenericMemPool::free(void* ptr, u32 param_1) {
    if (ptr != NULL) {
        *(void**)ptr = field_0x0;
        field_0x0 = ptr;
        freeMemCount++;
    }
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
    sAramBase = param_0;
    audioAramHeap.initRootHeap((void*)sAramBase, param_1);
}

JASHeap* JASKernel::getAramHeap() {
    return &audioAramHeap;
}
