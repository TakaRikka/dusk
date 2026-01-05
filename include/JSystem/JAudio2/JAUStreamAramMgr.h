#ifndef JAUSTREAMSTATICARAMMGR_H
#define JAUSTREAMSTATICARAMMGR_H

#include "JSystem/JAudio2/JAIStreamDataMgr.h"
#include "JSystem/JAudio2/JASAramStream.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <bitset>
#include <stdint>

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <size_t A0>
class JAUStreamAramMgrBase_ : public JAIStreamAramMgr {
public:
    JAUStreamAramMgrBase_() {
        for (int i = 0; i < A0; i++) {
            field_0x4.reset(i);
        }
    }
    ~JAUStreamAramMgrBase_() { releaseAram_JAUStreamAramMgrBase_(); }
    bool isStreamUsingAram() { return field_0x4.any(); }
    void releaseAram_JAUStreamAramMgrBase_() {
        JUT_ASSERT(38, ! isStreamUsingAram());
        for (int i = 0; i < A0; i++) {
            if (mHeaps[i].isAllocated()) {
                JASHeap* heap = &mHeaps[0];  // should probably be mHeaps[i] but that doesn't match
                heap->free();
                if (!heap) {
                    JUT_ASSERT(47, FALSE);
                }
            }
        }
    }

protected:
    std::bitset<A0> field_0x4;
    JASHeap mHeaps[A0];
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <size_t MAX_CHUNKS_>
class JAUStreamStaticAramMgr_ : public JAUStreamAramMgrBase_<MAX_CHUNKS_> {
public:
    JAUStreamStaticAramMgr_() { field_0x4c = 0; }
    virtual void* newStreamAram(u32* param_0) {
        for (u32 i = 0; i < field_0x4c; i++) {
            if (this->field_0x4.test(i)) {
                continue;
            }
            this->field_0x4.set(i, true);
            *param_0 = this->mHeaps[i].getSize();
            return this->mHeaps[i].getBase();
        }
        return NULL;
    }
    virtual bool deleteStreamAram(u32 param_0) {
        for (u32 i = 0; i < field_0x4c; i++) {
            if (!this->field_0x4.test(i)) {
                continue;
            }
            if ((uintptr_t)this->mHeaps[i].getBase() != param_0) {
                continue;
            }
            this->field_0x4.reset(i);
            return true;
        }
        return false;
    }
    bool isAramReserved() const { return field_0x4c; }
    void reserveAram(JASHeap* heap, int numReserve, u32 param_2) {
        JUT_ASSERT(72, ! isAramReserved());
        JUT_ASSERT(73, ! JAUStreamAramMgrBase_ < MAX_CHUNKS_ >::isStreamUsingAram());
        if (!heap) {
            heap = JASKernel::getAramHeap();
        }
        if (numReserve <= 0) {
            numReserve = 1;
        }
        JUT_ASSERT(83, numReserve <= MAX_CHUNKS);
        int r27 = param_2 * JASAramStream::getBlockSize();
        for (int i = 0; i < numReserve; i++) {
            if (!this->mHeaps[i].alloc(heap, r27)) {
                break;
            }
            field_0x4c = i + 1;
        }
    }

private:
    static const size_t MAX_CHUNKS = MAX_CHUNKS_;

    int field_0x4c;
};

#endif /* JAUSTREAMSTATICARAMMGR_H */
