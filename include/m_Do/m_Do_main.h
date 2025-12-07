#ifndef M_DO_M_DO_MAIN_H
#define M_DO_M_DO_MAIN_H

#include "JSystem/JKernel/JKRExpHeap.h"
#include <dolphin/os.h>

class JKRExpHeap;

void version_check();
s32 LOAD_COPYDATE(void*);

const int HeapCheckTableNum = 8;

class HeapCheck {
public:
    HeapCheck(JKRExpHeap* heap, const char* name, const char* jName) {
        mName = name;
        mJName = jName;
        setHeap(heap);
    }

    void CheckHeap1(void);
    s32 getUsedCount(void) const;
    void heapDisplay(void) const;

    u32& getUsedCountRef() { return mUsedCount; }
    u32& getTotalUsedSizeRef() { return mTotalUsedSize; }
    JKRExpHeap* getHeap() { return mHeap; }
    void setHeapSize(u32 i_size) { mTargetHeapSize = i_size; }
    s32 getMaxTotalUsedSize() const { return mMaxTotalUsedSize; }
    s32 getMaxTotalFreeSize() { return mMaxTotalFreeSize; }
    const char* getName() const { return mName; }
    const char* getJName() const { return mJName; }
    void saveRelBase() {
        mUsedCount = getUsedCount();
        mTotalUsedSize = mHeap->getTotalUsedSize();
    }

    void setHeap(JKRExpHeap* i_heap) {
        mHeap = i_heap;
        if (i_heap != NULL) {
            mTargetHeapSize = i_heap->getHeapSize();
        }
    }

    u32 getTargetHeapSize() const { return mTargetHeapSize; }

    u32 getRelUsedCount() const { return getUsedCount() - mUsedCount; }
    u32 getRelTotalUsedSize() const { return mHeap->getTotalUsedSize() - mTotalUsedSize; }

private:
    /* 0x00 */ const char* mName;
    /* 0x04 */ const char* mJName;
    /* 0x08 */ JKRExpHeap* mHeap;
    /* 0x0C */ s32 mMaxTotalUsedSize;
    /* 0x10 */ s32 mMaxTotalFreeSize;
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ s32 field_0x18;
    /* 0x1C */ u32 mTargetHeapSize;
    /* 0x20 */ u32 mUsedCount;
    /* 0x24 */ u32 mTotalUsedSize;
};

struct mDoMain {
#if DEBUG
    static int argument;
    static int e3menu_no;
    static u32 archiveHeapSize;
    static u32 gameHeapSize;
#endif

    static char COPYDATE_STRING[18];
    static u32 memMargin;
    static OSTime sPowerOnTime;
    static OSTime sHungUpTime;
    static s8 developmentMode;
};

#endif /* M_DO_M_DO_MAIN_H */
