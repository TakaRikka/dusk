#ifndef JKREXPHEAP_H
#define JKREXPHEAP_H

#include "JSystem/JKernel/JKRHeap.h"
#include <stdint.h>

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRExpHeap : public JKRHeap {
public:
    enum EAllocMode {
        ALLOC_MODE_1 = 1,
    };

    class CMemBlock {
        friend class JKRExpHeap;

    public:
        void initiate(CMemBlock* prev, CMemBlock* next, u32 size, u8 groupId, u8 alignment);
        JKRExpHeap::CMemBlock* allocFore(u32 size, u8 groupId1, u8 alignment1, u8 groupId2,
                                         u8 alignment2);
        JKRExpHeap::CMemBlock* allocBack(u32 size, u8 groupId1, u8 alignment1, u8 groupId2,
                                         u8 alignment2);
        int free(JKRExpHeap* heap);
        static CMemBlock* getHeapBlock(void* ptr);

        void newGroupId(u8 groupId) { mGroupId = groupId; }
        bool isValid() const { return mMagic == 'HM'; }
        bool isTempMemBlock() const { return mFlags & 0x80; }
        int getAlignment() const { return mFlags & 0x7f; }
        void* getContent() const { return (void*)(this + 1); }
        CMemBlock* getPrevBlock() const { return mPrev; }
        CMemBlock* getNextBlock() const { return mNext; }
        u32 getSize() const { return size; }
        u8 getGroupId() const { return mGroupId; }
        static CMemBlock* getBlock(void* data) { return (CMemBlock*)((uintptr_t)data + -sizeof(CMemBlock)); }

    private:
        /* 0x0 */ u16 mMagic;
        /* 0x2 */ u8 mFlags;  // a|bbbbbbb a=temporary b=alignment
        /* 0x3 */ u8 mGroupId;
        /* 0x4 */ u32 size;
        /* 0x8 */ CMemBlock* mPrev;
        /* 0xC */ CMemBlock* mNext;
#if BIT_64
        // Ensure padded to 0x20 bytes on 64-bit
        void* _pad;
#endif
    };  // Size: 0x10
    friend class CMemBlock;

#if TARGET_PC
    static_assert(sizeof(CMemBlock) == MEM_BLOCK_SIZE);
#endif

protected:
    JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);
    virtual ~JKRExpHeap();

    void* allocFromHead(u32 size, int align);
    void* allocFromHead(u32 size);
    void* allocFromTail(u32 size, int align);
    void* allocFromTail(u32 size);
    void appendUsedList(CMemBlock* newblock);
    void setFreeBlock(CMemBlock* block, CMemBlock* prev, CMemBlock* next);
    void removeFreeBlock(CMemBlock* block);
    void removeUsedBlock(CMemBlock* block);
    void recycleFreeBlock(CMemBlock* block);
    void joinTwoBlocks(CMemBlock* block);

public:
    BOOL isEmpty();
    s32 getUsedSize(u8 groupId) const;
    s32 getTotalUsedSize(void) const;
    
    CMemBlock* getUsedFirst() { return mHeadUsedList; }
    void setAllocationMode(EAllocMode mode) {
        mAllocMode = mode;
    }

public:
    /* vt[04] */ virtual u32 getHeapType() DUSK_OVERRIDE;
    /* vt[05] */ virtual bool check() DUSK_OVERRIDE;
    /* vt[06] */ virtual bool dump_sort() DUSK_OVERRIDE;
    /* vt[07] */ virtual bool dump() DUSK_OVERRIDE;
    /* vt[08] */ virtual void do_destroy() DUSK_OVERRIDE;
    /* vt[09] */ virtual void* do_alloc(u32 size, int alignment) DUSK_OVERRIDE;
    /* vt[10] */ virtual void do_free(void* ptr) DUSK_OVERRIDE;
    /* vt[11] */ virtual void do_freeAll() DUSK_OVERRIDE;
    /* vt[12] */ virtual void do_freeTail() DUSK_OVERRIDE;
    /* vt[13] */ virtual void do_fillFreeArea() DUSK_OVERRIDE;
    /* vt[14] */ virtual s32 do_resize(void* ptr, u32 size) DUSK_OVERRIDE;
    /* vt[15] */ virtual s32 do_getSize(void* ptr) DUSK_OVERRIDE;
    /* vt[16] */ virtual s32 do_getFreeSize() DUSK_OVERRIDE;
    /* vt[17] */ virtual void* do_getMaxFreeBlock() DUSK_OVERRIDE;
    /* vt[18] */ virtual s32 do_getTotalFreeSize() DUSK_OVERRIDE;
    /* vt[19] */ virtual s32 do_changeGroupID(u8 newGroupID) DUSK_OVERRIDE;
    /* vt[20] */ virtual u8 do_getCurrentGroupId() DUSK_OVERRIDE;
    /* vt[21] */ virtual void state_register(JKRHeap::TState* p, u32 id) const DUSK_OVERRIDE;
    /* vt[22] */ virtual bool state_compare(JKRHeap::TState const& r1,
                                            JKRHeap::TState const& r2) const DUSK_OVERRIDE;

    /* 0x6C */ u8 mAllocMode;
    /* 0x6D */ u8 mCurrentGroupId;
    /* 0x6E */ bool field_0x6e;

private:
    /* 0x70 */ void* field_0x70;
    /* 0x74 */ u32 field_0x74;
    /* 0x78 */ CMemBlock* mHeadFreeList;
    /* 0x7C */ CMemBlock* mTailFreeList;
    /* 0x80 */ CMemBlock* mHeadUsedList;
    /* 0x84 */ CMemBlock* mTailUsedList;

public:
    static JKRExpHeap* createRoot(int maxHeaps, bool errorFlag);
    static JKRExpHeap* create(u32 size, JKRHeap* parent, bool errorFlag);
    static JKRExpHeap* create(void* ptr, u32 size, JKRHeap* parent, bool errorFlag);

    static s32 getUsedSize_(JKRExpHeap* heap) { return heap->mSize - heap->getTotalFreeSize(); }
    static void* getState_(TState* state) { return getState_buf_(state); }

#if TARGET_PC
    [[nodiscard]] CMemBlock* getFreeHead() { return mHeadFreeList; }
    [[nodiscard]] const CMemBlock* getFreeHead() const { return mHeadFreeList; }
    [[nodiscard]] CMemBlock* getUsedHead() { return mHeadUsedList; }
    [[nodiscard]] const CMemBlock* getUsedHead() const { return mHeadUsedList; }
#endif
};

inline JKRExpHeap* JKRCreateExpHeap(u32 size, JKRHeap* parent, bool errorFlag) {
    return JKRExpHeap::create(size, parent, errorFlag);
}

inline void JKRDestroyExpHeap(JKRExpHeap* heap) {
    heap->destroy();
}

#endif /* JKREXPHEAP_H */
