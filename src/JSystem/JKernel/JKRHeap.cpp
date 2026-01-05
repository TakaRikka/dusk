/**
 * JKRHeap.cpp
 * JSystem Heap Framework
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"
#include <stdint>
#include <string>

#if DEBUG
u8 JKRValue_DEBUGFILL_NOTUSE = 0xFD;
u8 JKRValue_DEBUGFILL_NEW = 0xCD;
u8 JKRValue_DEBUGFILL_DELETE = 0xDD;
#endif

bool JKRHeap::sDefaultFillFlag = true;

JKRHeap* JKRHeap::sSystemHeap;

JKRHeap* JKRHeap::sCurrentHeap;

JKRHeap* JKRHeap::sRootHeap;

#if PLATFORM_WII || PLATFORM_SHIELD
JKRHeap* JKRHeap::sRootHeap2;
#endif

JKRErrorHandler JKRHeap::mErrorHandler;

static bool data_80451380;

JKRHeap::JKRHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag)
    : JKRDisposer(), mChildTree(this), mDisposerList() {
    OSInitMutex(&mMutex);
    mSize = size;
    mStart = (u8*)data;
    mEnd = (u8*)data + size;

    if (parent == NULL) {
        becomeSystemHeap();
        becomeCurrentHeap();
    } else {
        parent->mChildTree.appendChild(&mChildTree);

        if (sSystemHeap == sRootHeap) {
            becomeSystemHeap();
        }

        if (sCurrentHeap == sRootHeap) {
            becomeCurrentHeap();
        }
    }

    mErrorFlag = errorFlag;
    if (mErrorFlag == true && mErrorHandler == NULL) {
        mErrorHandler = JKRDefaultMemoryErrorRoutine;
    }

    mDebugFill = sDefaultFillFlag;
    mCheckMemoryFilled = data_80451380;
    mInitFlag = false;
}

JKRHeap::~JKRHeap() {
    mChildTree.getParent()->removeChild(&mChildTree);
    JSUTree<JKRHeap>* nextRootHeap = sRootHeap->mChildTree.getFirstChild();

    if (sCurrentHeap == this) {
        sCurrentHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();
    }

    if (sSystemHeap == this) {
        sSystemHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();
    }
}

void* JKRHeap::mCodeStart;

void* JKRHeap::mCodeEnd;

void* JKRHeap::mUserRamStart;

void* JKRHeap::mUserRamEnd;

u32 JKRHeap::mMemorySize;

JKRHeap::JKRAllocCallback JKRHeap::sAllocCallback;

JKRHeap::JKRFreeCallback JKRHeap::sFreeCallback;

bool JKRHeap::initArena(char** memory, u32* size, int maxHeaps) {
    void* arenaLo = OSGetArenaLo();
    void* arenaHi = OSGetArenaHi();
    OS_REPORT("original arenaLo = %p arenaHi = %p\n", arenaLo, arenaHi);
    if (arenaLo == arenaHi)
        return false;

    arenaLo = OSInitAlloc(arenaLo, arenaHi, maxHeaps);
    arenaLo = (void*)ALIGN_NEXT((uintptr_t)arenaLo, 0x20);
    arenaHi = (void*)ALIGN_PREV((uintptr_t)arenaHi, 0x20);

    OSBootInfo* codeStart = (OSBootInfo*)OSPhysicalToCached(0);
    mCodeStart = codeStart;
    mCodeEnd = arenaLo;

    mUserRamStart = arenaLo;
    mUserRamEnd = arenaHi;
    mMemorySize = codeStart->memorySize;

    OSSetArenaLo(arenaHi);
    OSSetArenaHi(arenaHi);

    *memory = (char*)arenaLo;
    *size = (uintptr_t)arenaHi - (uintptr_t)arenaLo;
    return true;
}

#if PLATFORM_WII || PLATFORM_SHIELD
bool JKRHeap::initArena2(char** memory, u32* size, int maxHeaps) {
    void* arenaLo = OSGetMEM2ArenaLo();
    void* arenaHi = OSGetMEM2ArenaHi();
    OS_REPORT("original arenaLo = %p arenaHi = %p\n", arenaLo, arenaHi);
    if (arenaLo == arenaHi) {
        return false;
    }
    arenaLo = (void*)0x91100000;
    arenaHi = (void*)ALIGN_PREV(uintptr_t(arenaHi), 32);
    OSSetMEM2ArenaLo(arenaHi);
    OSSetMEM2ArenaHi(arenaHi);
    *memory = (char*)arenaLo;
    *size = uintptr_t(arenaHi) - uintptr_t(arenaLo);
    return true;
}
#endif

JKRHeap* JKRHeap::becomeSystemHeap() {
    JKRHeap* prev = sSystemHeap;
    sSystemHeap = this;
    return prev;
}

JKRHeap* JKRHeap::becomeCurrentHeap() {
    JKRHeap* prev = sCurrentHeap;
    sCurrentHeap = this;
    return prev;
}

void JKRHeap::destroy() {
    do_destroy();
}

static void dummy1(JKRHeap* heap) {
    JUT_ASSERT(0, heap != 0);
}

void* JKRHeap::alloc(u32 size, int alignment, JKRHeap* heap) {
    if (heap != NULL) {
        return heap->alloc(size, alignment);
    }

    if (sCurrentHeap != NULL) {
        return sCurrentHeap->alloc(size, alignment);
    }

    return NULL;
}

void* JKRHeap::alloc(u32 size, int alignment) {
    if (mInitFlag) {
        JUT_WARN(393, "alloc %x byte in heap %x", size, this);
    }
    void* mem = do_alloc(size, alignment);
#if DEBUG
    if (sAllocCallback) {
        sAllocCallback(size, alignment, this, mem);
    }
#endif
    return mem;
}

void JKRHeap::free(void* ptr, JKRHeap* heap) {
    if (!heap) {
        heap = findFromRoot(ptr);
        if (!heap)
            return;
    }

    heap->free(ptr);
}

void JKRHeap::free(void* ptr) {
    if (mInitFlag) {
        JUT_WARN(441, "free %x in heap %x", ptr, this);
    }
#if DEBUG
    if (sFreeCallback) {
        sFreeCallback(ptr, this);
    }
#endif
    do_free(ptr);
}

void JKRHeap::callAllDisposer() {
    JSUListIterator<JKRDisposer> iterator;

    while ((iterator = mDisposerList.getFirst()) != mDisposerList.getEnd()) {
        iterator->~JKRDisposer();
    }
}

void JKRHeap::freeAll() {
    if (mInitFlag) {
        JUT_WARN(493, "freeAll in heap %x", this);
    }
    do_freeAll();
}

void JKRHeap::freeTail() {
    if (mInitFlag) {
        JUT_WARN(507, "freeTail in heap %x", this);
    }
    do_freeTail();
}

static void dummy2() {
    OS_REPORT("fillFreeArea in heap %x");
}

s32 JKRHeap::resize(void* ptr, u32 size, JKRHeap* heap) {
    if (!heap) {
        heap = findFromRoot(ptr);
        if (!heap)
            return -1;
    }

    return heap->resize(ptr, size);
}

s32 JKRHeap::resize(void* ptr, u32 size) {
    if (mInitFlag) {
        JUT_WARN(567, "resize block %x into %x in heap %x", ptr, size, this);
    }
    return do_resize(ptr, size);
}

s32 JKRHeap::getSize(void* ptr, JKRHeap* heap) {
    if (!heap) {
        heap = findFromRoot(ptr);
        if (!heap)
            return -1;
    }

    return heap->getSize(ptr);
}

s32 JKRHeap::getSize(void* ptr) {
    return do_getSize(ptr);
}

s32 JKRHeap::getFreeSize() {
    return do_getFreeSize();
}

void* JKRHeap::getMaxFreeBlock() {
    return do_getMaxFreeBlock();
}

s32 JKRHeap::getTotalFreeSize() {
    return do_getTotalFreeSize();
}

s32 JKRHeap::changeGroupID(u8 groupID) {
    if (mInitFlag) {
        JUT_WARN(646, "change heap ID into %x in heap %x", groupID, this);
    }
    return do_changeGroupID(groupID);
}

u8 JKRHeap::getCurrentGroupId() {
    return do_getCurrentGroupId();
}

u32 JKRHeap::getMaxAllocatableSize(int alignment) {
    u32 maxFreeBlock = (uintptr_t)getMaxFreeBlock();
    u32 ptrOffset = (alignment - 1) & alignment - (maxFreeBlock & 0xf);
    return ~(alignment - 1) & (getFreeSize() - ptrOffset);
}

JKRHeap* JKRHeap::findFromRoot(void* ptr) {
    if (sRootHeap == NULL) {
        return NULL;
    }

    if (sRootHeap->mStart <= ptr && ptr < sRootHeap->mEnd) {
        return sRootHeap->find(ptr);
    }
#if PLATFORM_WII || PLATFORM_SHIELD
    if (sRootHeap2->mStart <= ptr && ptr < sRootHeap2->mEnd) {
        return sRootHeap2->find(ptr);
    }
#endif
    return sRootHeap->findAllHeap(ptr);
}

JKRHeap* JKRHeap::find(void* memory) const {
    if (mStart <= memory && memory < mEnd) {
        if (mChildTree.getNumChildren() != 0) {
            for (JSUTreeIterator<JKRHeap> iterator(mChildTree.getFirstChild());
                 iterator != mChildTree.getEndChild(); ++iterator)
            {
                JKRHeap* result = iterator->find(memory);
                if (result) {
                    return result;
                }
            }
        }

        return const_cast<JKRHeap*>(this);
    }

    return NULL;
}

JKRHeap* JKRHeap::findAllHeap(void* ptr) const {
    if (mChildTree.getNumChildren() != 0) {
        for (JSUTreeIterator<JKRHeap> iterator(mChildTree.getFirstChild());
             iterator != mChildTree.getEndChild(); ++iterator)
        {
            JKRHeap* result = iterator->findAllHeap(ptr);

            if (result) {
                return result;
            }
        }
    }

    if (mStart <= ptr && ptr < mEnd) {
        return const_cast<JKRHeap*>(this);
    }

    return NULL;
}

void JKRHeap::dispose_subroutine(u32 begin, u32 end) {
    JSUListIterator<JKRDisposer> next_iterator((JSULink<JKRDisposer>*)NULL);
    JSUListIterator<JKRDisposer> it = mDisposerList.getFirst();
    while (it != mDisposerList.getEnd()) {
        JKRDisposer* disposer = it.getObject();

        if ((void*)begin <= disposer && disposer < (void*)end) {
            it->~JKRDisposer();

            if (next_iterator == JSUListIterator<JKRDisposer>((JSULink<JKRDisposer>*)NULL)) {
                it = mDisposerList.getFirst();
                continue;
            }
            it = next_iterator;
            it++;
            continue;
        }
        next_iterator = it;
        it++;
    }
}

bool JKRHeap::dispose(void* ptr, u32 size) {
    dispose_subroutine((uintptr_t)ptr, (uintptr_t)ptr + size);
    return false;
}

void JKRHeap::dispose(void* begin, void* end) {
    dispose_subroutine((uintptr_t)begin, (uintptr_t)end);
}

void JKRHeap::dispose() {
    JSUListIterator<JKRDisposer> iterator;
    while ((iterator = mDisposerList.getFirst()) != mDisposerList.getEnd()) {
        iterator->~JKRDisposer();
    }
}

void JKRHeap::copyMemory(void* dst, void* src, u32 size) {
    u32 count = (size + 3) / 4;

    u32* dst_32 = (u32*)dst;
    u32* src_32 = (u32*)src;
    while (count-- > 0) {
        *dst_32++ = *src_32++;
    }
}

void JKRDefaultMemoryErrorRoutine(void* heap, u32 size, int alignment) {
    OS_REPORT("Error: Cannot allocate memory %d(0x%x)byte in %d byte alignment from %08x\n", size,
             size, alignment, heap);
#if PLATFORM_GCN
    JUTException::panic(__FILE__, 831, "abort\n");
#else
    JUTException::panic(__FILE__, 912, "abort\n");
#endif
}

bool JKRHeap::setErrorFlag(bool errorFlag) {
    bool prev = mErrorFlag;
    mErrorFlag = errorFlag;
    return prev;
}

JKRErrorHandler JKRHeap::setErrorHandler(JKRErrorHandler errorHandler) {
    JKRErrorHandler prev = mErrorHandler;

    mErrorHandler = !errorHandler ? JKRDefaultMemoryErrorRoutine : errorHandler;

    return prev;
}

void JKRHeap::fillMemory(u8* dst, u32 size, u8 val) {
    uintptr_t ptr = uintptr_t(dst);
    memset(dst, val, size);
    DCFlushRange((void*)ALIGN_PREV(ptr, 32), ALIGN_NEXT(size, 32));
}

bool JKRHeap::checkMemoryFilled(u8* mem, u32 size, u8 val) {
    void* ptr = mem;
    bool result = true;
    for (int i = 0; i < size; i++) {
        if (val == mem[i]) {
            continue;
        }
        result = false;
        if (fillcheck_dispcount <= 0) {
            continue;
        }
        fillcheck_dispcount--;
        JUT_WARN(999, "**** checkMemoryFilled:\n address %08x size %x:\n (%08x = %02x)\n", mem, size, mem + i, mem[i]);
        if (data_8074A8D0_debug) {
            break;
        }
    }
    return result;
}

bool JKRHeap::isSubHeap(JKRHeap* heap) const {
    if (!heap)
        return false;

    if (mChildTree.getNumChildren() != 0) {
        for (JSUTreeIterator<JKRHeap> iterator = mChildTree.getFirstChild(); iterator != mChildTree.getEndChild();
             ++iterator)
        {
            if (iterator.getObject() == heap) {
                return true;
            }

            if (iterator.getObject()->isSubHeap(heap)) {
                return true;
            }
        }
    }

    return false;
}

void* operator new(size_t size) {
    return JKRHeap::alloc(size, 4, NULL);
}

void* operator new(size_t size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}

void* operator new(size_t size, JKRHeap* heap, int alignment) {
    return JKRHeap::alloc(size, alignment, heap);
}

void* operator new[](size_t size) {
    return JKRHeap::alloc(size, 4, NULL);
}

void* operator new[](size_t size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}

void* operator new[](size_t size, JKRHeap* heap, int alignment) {
    return JKRHeap::alloc(size, alignment, heap);
}

void operator delete(void* ptr) {
    JKRHeap::free(ptr, NULL);
}

void operator delete[](void* ptr) {
    JKRHeap::free(ptr, NULL);
}

s32 fillcheck_dispcount = 100;
bool data_8074A8D0_debug = true;

void JKRHeap::state_register(JKRHeap::TState* p, u32 id) const {
    JUT_ASSERT(1213, p != NULL);
    JUT_ASSERT(1214, p->getHeap() == this);
}

bool JKRHeap::state_compare(const JKRHeap::TState& r1, const JKRHeap::TState& r2) const {
    JUT_ASSERT(1222, r1.getHeap() == r2.getHeap());
    return r1.getCheckCode() == r2.getCheckCode();
}

void JKRHeap::state_dump(const JKRHeap::TState& p) const {
    JUT_LOG(1246, "check-code : 0x%08x", p.getCheckCode());
    JUT_LOG(1247, "id         : 0x%08x", p.getId());
    JUT_LOG(1248, "used size  : %u", p.getUsedSize());
}

void* ARALT_AramStartAdr = (void*)0x90000000;

void* JKRHeap::getAltAramStartAdr() { return ARALT_AramStartAdr; }

s32 JKRHeap::do_changeGroupID(u8 param_0) {
    return 0;
}

u8 JKRHeap::do_getCurrentGroupId() {
    return 0;
}
