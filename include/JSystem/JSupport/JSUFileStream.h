#ifndef JSUFILESTREAM_H
#define JSUFILESTREAM_H

#include "JSystem/JSupport/JSURandomInputStream.h"

class JKRFile;

/**
* @ingroup jsystem-jsupport
* 
*/
class JSUFileInputStream : public JSURandomInputStream {
public:
    // TODO: fix return values
    JSUFileInputStream(JKRFile*);
    u32 readData(void*, s32);
    s32 seekPos(s32, JSUStreamSeekFrom);
    s32 getLength() const;
    s32 getPosition() const;

private:
    /* 0x08 */ JKRFile* mFile;
    /* 0x0C */ s32 mPosition;
};  // Size = 0x10

#endif /* JSUFILESTREAM_H */
