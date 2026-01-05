#ifndef JKRARAMBLOCK_H
#define JKRARAMBLOCK_H

#include "JSystem/JSupport/JSUList.h"

class JKRAramHeap;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAramBlock {
public:
    JKRAramBlock(u32, u32, u32, u8, bool);
    virtual ~JKRAramBlock();

    JKRAramBlock* allocHead(u32, u8, JKRAramHeap*);
    JKRAramBlock* allocTail(u32, u8, JKRAramHeap*);

    u32 getAddress() const { return mAddress; }

    u32 getSize() const { return mSize; }

    u32 getFreeSize() const { return mFreeSize; }

    bool isTempMemory() const { return mIsTempMemory; }

    void newGroupID(u8 groupId) { mGroupId = groupId; }

public:
    /* 0x00 */  // vtable
    /* 0x04 */ JSULink<JKRAramBlock> mBlockLink;
    /* 0x14 */ u32 mAddress;
    /* 0x18 */ u32 mSize;
    /* 0x1C */ u32 mFreeSize;
    /* 0x20 */ u8 mGroupId;
    /* 0x21 */ bool mIsTempMemory;
    /* 0x22 */ u8 padding[2];
};

#endif /* JKRARAMBLOCK_H */
