#ifndef JKRFILE_H
#define JKRFILE_H

#include "JSystem/JKernel/JKRDisposer.h"

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRFile : public JKRDisposer {
public:
    JKRFile() : mIsAvailable(false) {}
    virtual ~JKRFile() {}

    void read(void*, s32, s32);

    bool isAvailable() const { return mIsAvailable; }

public:
    /* vt[03] */ virtual bool open(const char*) = 0;
    /* vt[04] */ virtual void close() = 0;
    /* vt[05] */ virtual s32 readData(void*, s32, s32) = 0;
    /* vt[06] */ virtual s32 writeData(const void*, s32, s32) = 0;
    /* vt[07] */ virtual s32 getFileSize() const = 0;

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    /* 0x18 */ bool mIsAvailable;
    /* 0x19 */ u8 field_0x19[3];
};

#endif /* JKRFILE_H */
