#ifndef JKRASSERTHEAP_H
#define JKRASSERTHEAP_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAssertHeap : public JKRHeap {
protected:
    JKRAssertHeap(void*, u32, JKRHeap*, bool);
    virtual ~JKRAssertHeap();

public:
    /* vt[04] */ virtual u32 getHeapType(void);            /* override */
    /* vt[05] */ virtual bool check(void);                 /* override */
    /* vt[06] */ virtual bool dump_sort(void);             /* override */
    /* vt[07] */ virtual bool dump(void);                  /* override */
    /* vt[08] */ virtual void do_destroy(void);            /* override */
    /* vt[19] */ virtual s32 do_changeGroupID(u8) { JUT_ASSERT(41, 0&& "illegal changeGroupID()"); return 0; }
    /* vt[20] */ virtual u8 do_getCurrentGroupId(void) { return 0; }
    /* vt[09] */ virtual void* do_alloc(u32, int alignment) { UNUSED(alignment); JUT_ASSERT(47, 0&& "illegal alloc"); return NULL; }
    /* vt[10] */ virtual void do_free(void*) { JUT_ASSERT(51, 0&& "illegal free"); }
    /* vt[11] */ virtual void do_freeAll(void) { JUT_ASSERT(53, 0&& "illegal freeAll()"); }
    /* vt[12] */ virtual void do_freeTail(void) { JUT_ASSERT(55, 0&& "illegal freeTail()"); }
    /* vt[13] */ virtual void do_fillFreeArea(void) {}
    /* vt[14] */ virtual s32 do_resize(void*, u32) { JUT_ASSERT(61, 0&& "illegal resize"); return 0; }
    /* vt[15] */ virtual s32 do_getSize(void*) { return 0; }
    /* vt[16] */ virtual s32 do_getFreeSize(void) { return 0; }
    /* vt[17] */ virtual void* do_getMaxFreeBlock(void) { return 0; }
    /* vt[18] */ virtual s32 do_getTotalFreeSize(void) { return 0; }


public:
    static JKRAssertHeap* create(JKRHeap*);
};

#endif /* JKRASSERTHEAP_H */
