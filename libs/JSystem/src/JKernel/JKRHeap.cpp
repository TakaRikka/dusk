/**
 * JKRHeap.cpp
 * JSystem Heap Framework
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRHeap.h"

#if TARGET_PC
#include <algorithm>
#endif

#include <cassert>

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"
#ifdef __MWERKS__
#include <stdint.h>
#else
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#endif
#include <cstring>
#include <dolphin/os.h>
#include "os_report.h"

#if DEBUG
u8 JKRValue_DEBUGFILL_NOTUSE = 0xFD;
u8 JKRValue_DEBUGFILL_NEW = 0xCD;
u8 JKRValue_DEBUGFILL_DELETE = 0xDD;
#endif

bool JKRHeap::sDefaultFillFlag = true;

JKRHeap* JKRHeap::sSystemHeap;

#if TARGET_PC
// JSystem normally has a thread switch callback to track the correct heap.
// We can't do this as we're (currently) using true OS threads. So use a true thread local.
// On Linux/GCC, thread_local in a shared library requires global-dynamic TLS model
// (the default local-exec is incompatible with -fPIC). MSVC and macOS/Clang handle this automatically.
#if defined(__GNUC__) && !defined(__clang__) && !defined(_MSC_VER)
#define TLS_GLOBAL_DYNAMIC __attribute__((tls_model("global-dynamic")))
#else
#define TLS_GLOBAL_DYNAMIC
#endif
static thread_local TLS_GLOBAL_DYNAMIC JKRHeap* sCurrentHeap;
#else
JKRHeap* JKRHeap::sCurrentHeap;
#endif

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

#if TARGET_PC
    memset(mName, 0, sizeof(mName));
#endif
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

    OSReport("[JKRHeap] initArena: Lo=%p Hi=%p Size=0x%X\n", arenaLo, arenaHi,
             (uintptr_t)arenaHi - (uintptr_t)arenaLo);

    if (arenaLo == arenaHi)
        return false;

#ifdef TARGET_PC
    // PC: Simple arena setup without GameCube-specific memory management
    arenaLo = (void*)ALIGN_NEXT((uintptr_t)arenaLo, 0x20);
    arenaHi = (void*)ALIGN_PREV((uintptr_t)arenaHi, 0x20);

    mCodeStart = nullptr;
    mCodeEnd = nullptr;
    mUserRamStart = arenaLo;
    mUserRamEnd = arenaHi;
    mMemorySize = (uintptr_t)arenaHi - (uintptr_t)arenaLo;

    *memory = (char*)arenaLo;
    *size = (uintptr_t)arenaHi - (uintptr_t)arenaLo;
    return true;
#else
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
#endif
}

#if PLATFORM_WII || PLATFORM_SHIELD
bool JKRHeap::initArena2(char** memory, u32* size, int maxHeaps) {
    void* arenaLo = OSGetMEM2ArenaLo();
    void* arenaHi = OSGetMEM2ArenaHi();
#if !PLATFORM_GCN
    OSReport("original arenaLo = %p arenaHi = %p\n", arenaLo, arenaHi);
#endif
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
    uintptr_t maxFreeBlock = (uintptr_t)getMaxFreeBlock();
    uintptr_t ptrOffset = (alignment - 1) & alignment - (maxFreeBlock & (MEM_BLOCK_SIZE - 1));
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

void JKRHeap::dispose_subroutine(uintptr_t begin, uintptr_t end) {
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

#if TARGET_PC
[[nodiscard]]
static void* fallback_alloc(size_t size, size_t align, bool log=true) {
    if (log) {
        auto curHeap = JKRHeap::getCurrentHeap();
        const char* name = "<null>";
        if (curHeap != nullptr) {
            name = curHeap->getName();
        }

        OSReport(
            "[NEW] JKRHeap (%s) FULL! Fallback to malloc for size %u\n",
            name, (unsigned)size);
    }

    if (align == 0) {
        align = alignof(max_align_t);
    }

    assert((align & (align - 1)) == 0 && "Alignment must be a power of two");

#if _WIN32
    // aligned_alloc() is not available on Windows.
    // NOTE: We always use _aligned_malloc(), even for allocs <= max_align_t,
    // because otherwise we can't tell in operator delete() whether a pointer needs to be freed with
    // _aligned_free() or regular free().
    return _aligned_malloc(size, align);
#else
    // aligned_alloc() requires size be a multiple of align. So ensure it is.
    size = ALIGN_NEXT(size, align);
    return aligned_alloc(align, size);
#endif

}
#endif

#if !TARGET_PC
void* operator new(size_t size) {
    return JKRHeap::alloc(size, 4, NULL);
}
#else
void* operator new(size_t size JKR_HEAP_TOKEN_PARAM) {
    if (sCurrentHeap == NULL) {
        return fallback_alloc(size, 0, false);
    }
    void* mem = JKRHeap::alloc(size, alignof(max_align_t), NULL);
    if (mem == nullptr) {
        return fallback_alloc(size, 0, true);
    }
    return mem;
}
#endif

#if !TARGET_PC
void* operator new(size_t size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}
#else
void* operator new(size_t size JKR_HEAP_TOKEN_PARAM, int alignment) {
    void* mem = JKRHeap::alloc(size, alignment, nullptr);
    if (mem == nullptr) {
        return fallback_alloc(size, abs(alignment), true);
    }
    return mem;
}
#endif

void* operator new(size_t size JKR_HEAP_TOKEN_PARAM, JKRHeap* heap, int alignment) {
    void* mem = JKRHeap::alloc(size, alignment, heap);
#if TARGET_PC
    if (mem == nullptr) {
        return fallback_alloc(size, abs(alignment), true);
    }
#endif
    return mem;
}

#if !TARGET_PC
void* operator new[](size_t size) {
    return JKRHeap::alloc(size, 4, NULL);
}
#else
void* operator new[](size_t JKR_HEAP_TOKEN_PARAM) {
    OSPanic(__FILE__, __LINE__, "Allocation should go through JKR_NEW_ARRAY instead");
}
#endif

#if !TARGET_PC
void* operator new[](size_t size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}
#else
void* operator new[](size_t JKR_HEAP_TOKEN_PARAM, int) {
    OSPanic(__FILE__, __LINE__, "Allocation should go through JKR_NEW_ARRAY instead");
}
#endif

void* operator new[](size_t JKR_HEAP_TOKEN_PARAM, JKRHeap*, int) {
    OSPanic(__FILE__, __LINE__, "Allocation should go through JKR_NEW_ARRAY instead");
}

#if !TARGET_PC
void operator delete(void* ptr) {
    JKRHeap::free(ptr, NULL);
}
#else
void operator delete(void* ptr JKR_HEAP_TOKEN_PARAM) {
    if (ptr == NULL)
        return;
    JKRHeap* heap = JKRHeap::findFromRoot(ptr);
    if (heap == NULL) {
#if !_WIN32
        free(ptr);
#else
        _aligned_free(ptr);
#endif
        return;
    }
    JKRHeap::free(ptr, heap);
}
#endif

#if !TARGET_PC
void operator delete[](void* ptr) {
    JKRHeap::free(ptr, NULL);
}
#else
void operator delete[](void* ptr JKR_HEAP_TOKEN_PARAM) {
    if (ptr == NULL)
        return;
    JKRHeap* heap = JKRHeap::findFromRoot(ptr);
    if (heap == NULL) {
#if !_WIN32
        free(ptr);
#else
        _aligned_free(ptr);
#endif
        return;
    }
    JKRHeap::free(ptr, heap);
}
#endif

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

#if TARGET_PC
void JKRHeap::setCurrentHeap(JKRHeap* heap) {
    sCurrentHeap = heap;
}

JKRHeap* JKRHeap::getCurrentHeap() {
    return sCurrentHeap;
}

void JKRHeap::setName(const char* name) {
    size_t len = strlen(name);
    strncpy(mName, name, sizeof(mName) - 1);
    mName[sizeof(mName) - 1] = '\0';
}

void JKRHeap::setNamef(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(mName, sizeof(mName), fmt, args);
    va_end(args);
}

const char* JKRHeap::getName() const {
    return mName;
}
#endif
