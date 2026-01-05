#ifndef JKREXPHEAP_H
#define JKREXPHEAP_H

#include "JSystem/JKernel/JKRHeap.h"
#include <stdint>

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
        static CMemBlock* getBlock(void* data) { return (CMemBlock*)((uintptr_t)data + -0x10); }

    private:
        /* 0x0 */ u16 mMagic;
        /* 0x2 */ u8 mFlags;  // a|bbbbbbb a=temporary b=alignment
        /* 0x3 */ u8 mGroupId;
        /* 0x4 */ u32 size;
        /* 0x8 */ CMemBlock* mPrev;
        /* 0xC */ CMemBlock* mNext;
    };  // Size: 0x10
    friend class CMemBlock;

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
    /* vt[04] */ virtual u32 getHeapType();                                     /* override */
    /* vt[05] */ virtual bool check();                                          /* override */
    /* vt[06] */ virtual bool dump_sort();                                      /* override */
    /* vt[07] */ virtual bool dump();                                           /* override */
    /* vt[08] */ virtual void do_destroy();                                     /* override */
    /* vt[09] */ virtual void* do_alloc(u32 size, int alignment);               /* override */
    /* vt[10] */ virtual void do_free(void* ptr);                               /* override */
    /* vt[11] */ virtual void do_freeAll();                                     /* override */
    /* vt[12] */ virtual void do_freeTail();                                    /* override */
    /* vt[13] */ virtual void do_fillFreeArea();                                /* override */
    /* vt[14] */ virtual s32 do_resize(void* ptr, u32 size);                    /* override */
    /* vt[15] */ virtual s32 do_getSize(void* ptr);                             /* override */
    /* vt[16] */ virtual s32 do_getFreeSize();                                  /* override */
    /* vt[17] */ virtual void* do_getMaxFreeBlock();                            /* override */
    /* vt[18] */ virtual s32 do_getTotalFreeSize();                             /* override */
    /* vt[19] */ virtual s32 do_changeGroupID(u8 newGroupID);                   /* override */
    /* vt[20] */ virtual u8 do_getCurrentGroupId();                             /* override */
    /* vt[21] */ virtual void state_register(JKRHeap::TState* p, u32 id) const; /* override */
    /* vt[22] */ virtual bool state_compare(JKRHeap::TState const& r1,
                                            JKRHeap::TState const& r2) const; /* override */

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
};

inline JKRExpHeap* JKRCreateExpHeap(u32 size, JKRHeap* parent, bool errorFlag) {
    return JKRExpHeap::create(size, parent, errorFlag);
}

inline void JKRDestroyExpHeap(JKRExpHeap* heap) {
    heap->destroy();
}

#endif /* JKREXPHEAP_H */
