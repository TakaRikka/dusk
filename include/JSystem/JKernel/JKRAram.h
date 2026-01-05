#ifndef JKRARAM_H
#define JKRARAM_H

#include "JSystem/JKernel/JKRAramHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRThread.h"

class JKRHeap;
class JKRAMCommand;
class JKRAramBlock;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAram : public JKRThread {
private:
    JKRAram(u32, u32, s32);
    virtual ~JKRAram();

    /* vt[03] */ void* run(void); /* override */

public:
    u32 getAudioMemory() const { return mAudioMemoryPtr; }
    u32 getAudioMemSize() const { return mAudioMemorySize; }
    u32 getGraphMemory() const { return mGraphMemoryPtr; }
    u32 getGraphMemSize() const { return mGraphMemorySize; }
//private:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRThread
    /* 0x7C */ u32 mAudioMemoryPtr;
    /* 0x80 */ u32 mAudioMemorySize;
    /* 0x84 */ u32 mGraphMemoryPtr;
    /* 0x88 */ u32 mGraphMemorySize;
    /* 0x8C */ u32 mAramMemoryPtr;
    /* 0x90 */ u32 mAramMemorySize;
    /* 0x94 */ JKRAramHeap* mAramHeap;
    /* 0x98 */ u32 mStackArray[3];

public:
    static JKRAram* create(u32, u32, s32, s32, s32);
    static void checkOkAddress(u8*, u32, JKRAramBlock*, u32);
    static void changeGroupIdIfNeed(u8*, int);
    static JKRAramBlock* mainRamToAram(u8*, u32, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);
    static u8* aramToMainRam(u32, u8*, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);
    static void dump(void);

    static JKRAram* getManager() { return sAramObject; }
    static JKRAramHeap* getAramHeap() { return sAramObject->mAramHeap; }
    static JSUList<JKRAMCommand>& getCommandList() { return sAramCommandList; }

    static u8 decideAramGroupId(int groupId) {
        u8 finalGroupId;
        if (groupId < 0) {
            finalGroupId = getAramHeap()->getCurrentGroupID();
        } else {
            finalGroupId = groupId;
        }
        return finalGroupId;
    }

    static u32 getSZSBufferSize() { return sSZSBufferSize; }
    static void setSZSBufferSize(u32 size) { sSZSBufferSize = size; }

    static OSMessageQueue sMessageQueue;

private:
    static JKRAram* sAramObject;
    static u32 sSZSBufferSize;
    static OSMessage sMessageBuffer[4];
    static JSUList<JKRAMCommand> sAramCommandList;
};

inline JKRAramBlock* JKRAllocFromAram(u32 size, JKRAramHeap::EAllocMode allocMode) {
    return JKRAram::getAramHeap()->alloc(size, allocMode);
}

inline void JKRFreeToAram(JKRAramBlock* block) {
    JKRAram::getAramHeap()->free(block);
}

inline u8* JKRAramToMainRam(u32 p1, u8* p2, u32 p3, JKRExpandSwitch p4, u32 p5, JKRHeap* p6,
                             int p7, u32* p8) {
    return JKRAram::aramToMainRam(p1, p2, p3, p4, p5, p6, p7, p8);
}

inline JKRAramBlock* JKRMainRamToAram(u8 *buf, u32 bufSize, u32 alignedSize, JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap *heap, int id, u32 *pSize) {
    return JKRAram::mainRamToAram(buf, bufSize, alignedSize, expandSwitch, fileSize, heap, id, pSize);
}

inline u32 JKRGetAramTotalFreeSize() {
    return JKRAram::getAramHeap()->getTotalFreeSize();
}

inline u32 JKRGetAramFreeSize() {
    return JKRAram::getAramHeap()->getFreeSize();
}

inline u32 JKRGetAramUsedSize(u8 param_0) {
    return JKRAram::getAramHeap()->getUsedSize(param_0);
}

// void JKRDecompressFromAramToMainRam(u32, void*, u32, u32, u32, u32*);

#endif /* JKRARAM_H */
