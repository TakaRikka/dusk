#ifndef JSURANDOMINPUTSTREAM_H_
#define JSURANDOMINPUTSTREAM_H_

#include "JSystem/JSupport/JSUInputStream.h"

/**
* @ingroup jsystem-jsupport
* 
*/
class JSURandomInputStream : public JSUInputStream {
public:
    JSURandomInputStream() {}
    virtual ~JSURandomInputStream() {}

    /* vt[3] */ virtual s32 getAvailable() const /* override */ {
        return getLength() - getPosition();
    }
    /* vt[4] */ virtual s32 skip(s32);            /* override */
    /* vt[5] */ virtual u32 readData(void*, s32) = 0;
    /* vt[6] */ virtual s32 getLength() const = 0;
    /* vt[7] */ virtual s32 getPosition() const = 0;
    /* vt[8] */ virtual s32 seekPos(s32, JSUStreamSeekFrom) = 0;

    // TODO: fix return types
    s32 align(s32);
    s32 peek(void*, s32);
    s32 seek(s32, JSUStreamSeekFrom);
};  // Size = 0x8

#endif
