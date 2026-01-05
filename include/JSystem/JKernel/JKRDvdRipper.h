#ifndef JKRDVDRIPPER_H
#define JKRDVDRIPPER_H

#include "JSystem/JKernel/JKRCompression.h"
#include "JSystem/JSupport/JSUList.h"

enum JKRExpandSwitch {
    EXPAND_SWITCH_UNKNOWN0 = 0,
    EXPAND_SWITCH_UNKNOWN1 = 1,
    EXPAND_SWITCH_UNKNOWN2 = 2,
};

struct SYaz0Header {
    u32 signature;
    u32 length;
};

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRDMCommand {
    JKRDMCommand();
    ~JKRDMCommand();
};

class JKRHeap;
class JKRDvdFile;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRDvdRipper {
public:
    static JSUList<JKRDMCommand> sDvdAsyncList;
    static u32 sSZSBufferSize;
    static bool errorRetry;

    enum EAllocDirection {
        UNKNOWN_EALLOC_DIRECTION = 0,
        ALLOC_DIRECTION_FORWARD = 1,
        ALLOC_DIRECTION_BACKWARD = 2,
    };

    static void setSZSBufferSize(u32 size) { sSZSBufferSize = size; }

    static void* loadToMainRAM(char const*, u8*, JKRExpandSwitch, u32, JKRHeap*, EAllocDirection,
                               u32, JKRCompression*, u32*);
    static void* loadToMainRAM(s32, u8*, JKRExpandSwitch, u32, JKRHeap*, EAllocDirection, u32,
                               JKRCompression*, u32*);
    static void* loadToMainRAM(JKRDvdFile*, u8*, JKRExpandSwitch, u32, JKRHeap*, EAllocDirection,
                               u32, JKRCompression*, u32*);

    static bool isErrorRetry(void) { return errorRetry; }
    inline static u32 getSZSBufferSize() { return sSZSBufferSize; }
};

// void JKRDecompressFromDVD(JKRDvdFile*, void*, u32, u32, u32, u32, u32*);


inline void* JKRDvdToMainRam(JKRDvdFile* file, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength,
                             JKRHeap* heap, JKRDvdRipper::EAllocDirection allocDirection,
                             u32 offset, JKRCompression* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(file, dst, expandSwitch, dstLength, heap, allocDirection,
                                       offset, compression, returnSize);
}

inline void* JKRDvdToMainRam(s32 entryNum, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength,
                             JKRHeap* heap, JKRDvdRipper::EAllocDirection allocDirection,
                             u32 offset, JKRCompression* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(entryNum, dst, expandSwitch, dstLength, heap, allocDirection,
                                       offset, compression, returnSize);
}

inline void* JKRDvdToMainRam(const char* name, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength,
                             JKRHeap* heap, JKRDvdRipper::EAllocDirection allocDirection,
                             u32 offset, JKRCompression* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(name, dst, expandSwitch, dstLength, heap, allocDirection,
                                       offset, compression, returnSize);
}

#endif /* JKRDVDRIPPER_H */
