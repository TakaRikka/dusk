#ifndef JKRARAMSTREAM_H
#define JKRARAMSTREAM_H

#include "JSystem/JKernel/JKRThread.h"

class JSUFileInputStream;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAramStreamCommand {
public:
    enum Type {
        UNKNOWN = 0,
        READ = 1,
        WRITE = 2,
    };

    JKRAramStreamCommand();

public:
    /* 0x00 */ Type mType;
    /* 0x04 */ u32 mAddress;
    /* 0x08 */ u32 mSize;
    /* 0x0C */ u32 field_0x0c;
    /* 0x10 */ JSUFileInputStream* mStream;
    /* 0x14 */ u32 mOffset;
    /* 0x18 */ u32* mReturnSize;
    /* 0x1C */ u8* mTransferBuffer;
    /* 0x20 */ u32 mTransferBufferSize;
    /* 0x24 */ JKRHeap* mHeap;
    /* 0x28 */ bool mAllocatedTransferBuffer;
    /* 0x29 */ u8 padding_0x29[3];
    /* 0x2C */ u32 field_0x2c;
    /* 0x30 */ OSMessageQueue mMessageQueue;
    /* 0x50 */ OSMessage mMessage;
    /* 0x54 */ u32 field_0x54;
    /* 0x58 */ u32 field_0x58;
};

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAramStream : public JKRThread {
private:
    JKRAramStream(s32);
    virtual ~JKRAramStream();

    /* vt[03] */ void* run(void); /* override */

public:
    static JKRAramStream* create(s32);

    static s32 readFromAram(void);
    static s32 writeToAram(JKRAramStreamCommand*);
    static JKRAramStreamCommand* write_StreamToAram_Async(JSUFileInputStream*, u32, u32, u32, u32*);
    static JKRAramStreamCommand* sync(JKRAramStreamCommand*, BOOL);
    static void setTransBuffer(u8*, u32, JKRHeap*);

private:
    static JKRAramStream* sAramStreamObject;
    static OSMessage sMessageBuffer[4];
    static OSMessageQueue sMessageQueue;

    static u8* transBuffer;
    static u32 transSize;
    static JKRHeap* transHeap;
};

inline JKRAramStream* JKRCreateAramStreamManager(s32 priority) {
    return JKRAramStream::create(priority);
}

inline JKRAramStreamCommand* JKRStreamToAram_Async(JSUFileInputStream *stream, u32 addr, u32 size, u32 offset, void (*callback)(u32), u32* returnSize) {
    return JKRAramStream::write_StreamToAram_Async(stream, addr, size, offset, returnSize);
}

inline JKRAramStreamCommand* JKRStreamToAram_Sync(JKRAramStreamCommand* command, BOOL isNonBlocking) {
    return JKRAramStream::sync(command, isNonBlocking);
}

inline void JKRSetAramTransferBuffer(u8* buffer, u32 bufferSize, JKRHeap* heap) {
    JKRAramStream::setTransBuffer(buffer, bufferSize, heap);
}

inline void JKRResetAramTransferBuffer() {
    JKRAramStream::setTransBuffer(NULL, 0, NULL);
}

#endif /* JKRARAMSTREAM_H */
