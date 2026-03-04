#ifndef JSUMEMORYSTREAM_H
#define JSUMEMORYSTREAM_H

#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JSupport/JSURandomOutputStream.h"

/**
* @ingroup jsystem-jsupport
* 
*/
class JSUMemoryInputStream : public JSURandomInputStream {
public:
    JSUMemoryInputStream(const void* res, s32 size) { setBuffer(res, size); }

    virtual ~JSUMemoryInputStream() {}
    void setBuffer(void const*, s32);
    u32 readData(void*, s32);
    s32 seekPos(s32, JSUStreamSeekFrom);
    s32 getLength() const;
    s32 getPosition() const;

    void* getPointer() const {
        return (u8*)mBuffer + mPosition;
    }

private:
    /* 0x08 */ const void* mBuffer;
    /* 0x0C */ s32 mLength;
    /* 0x10 */ s32 mPosition;
};  // Size = 0x14

class JSUMemoryOutputStream : public JSURandomOutputStream {
public:
    JSUMemoryOutputStream() { setBuffer(NULL, 0); }
    JSUMemoryOutputStream(void* buffer, s32 len) { setBuffer(buffer, len); }

    void setBuffer(void* buffer, s32 len);

    virtual ~JSUMemoryOutputStream() {}
    virtual s32 writeData(const void*, s32);
    virtual s32 getLength() const;
    virtual s32 getPosition() const;
    virtual s32 seek(s32, JSUStreamSeekFrom);
    virtual s32 getAvailable() const;
    virtual s32 seekPos(s32, JSUStreamSeekFrom);

private:
    /* 0x08 */ void* mBuffer;
    /* 0x0C */ s32 mLength;
    /* 0x10 */ s32 mPosition;
};  // Size = 0x14

#endif /* JSUMEMORYSTREAM_H */
