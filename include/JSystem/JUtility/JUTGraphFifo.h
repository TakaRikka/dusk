#ifndef JUTGRAPHFIFO_H
#define JUTGRAPHFIFO_H

#include "dolphin/gx.h"

/**
* @ingroup jsystem-jutility
* 
*/
class JUTGraphFifo {
public:
    JUTGraphFifo(u32);

    virtual ~JUTGraphFifo();

    void getGpStatus() {
        GXGetGPStatus((GXBool*)&mGpStatus[0], (GXBool*)&mGpStatus[1], (GXBool*)&mGpStatus[2],
                      (GXBool*)&mGpStatus[3], (GXBool*)&mGpStatus[4]);
    }

    bool isGPActive() {
        getGpStatus();
        return mGpStatus[2] == false;
    }

    void save() { GXSaveCPUFifo(this->mFifo); }

    static JUTGraphFifo* sCurrentFifo;
    static bool mGpStatus[5];

private:
    /* 0x04 */ GXFifoObj* mFifo;
    /* 0x08 */ void* mBase;
    /* 0x0C */ u32 mSize;
    /* 0x10 */ u8 field_0x10[0xC];
};

inline JUTGraphFifo* JUTCreateFifo(u32 bufSize) { return new JUTGraphFifo(bufSize); }

#endif /* JUTGRAPHFIFO_H */
