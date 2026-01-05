#ifndef JKRDVDARAMRIPPER_H
#define JKRDVDARAMRIPPER_H

#include "JSystem/JKernel/JKRDvdRipper.h"

class JKRAramBlock;
class JKRAramStreamCommand;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRADCommand {
public:
    JKRADCommand();
    ~JKRADCommand();

    /* 0x00 */ JSULink<JKRADCommand> mLink;
    /* 0x10 */ int field_0x10;
    /* 0x14 */ int field_0x14;
    /* 0x18 */ int field_0x18;
    /* 0x1C */ int field_0x1c;
    /* 0x20 */ int field_0x20;
    /* 0x24 */ int field_0x24;
    /* 0x28 */ JKRDvdFile* mDvdFile;
    /* 0x2C */ u32 mAddress;
    /* 0x30 */ JKRAramBlock* mBlock;
    /* 0x34 */ JKRExpandSwitch mExpandSwitch;
    /* 0x38 */ void (*mCallback)(u32);
    /* 0x3C */ u32 field_0x3c;
    /* 0x40 */ u32 field_0x40;
    /* 0x44 */ u32* field_0x44;
    /* 0x48 */ int field_0x48;
    /* 0x4C */ u8 field_0x4c;
    /* 0x50 */ JKRAramStreamCommand* mStreamCommand;
};

class JKRDvdFile;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRDvdAramRipper {
public:
    static JKRAramBlock* loadToAram(s32, u32, JKRExpandSwitch, u32, u32, u32*);
    static JKRAramBlock* loadToAram(JKRDvdFile*, u32, JKRExpandSwitch, u32, u32, u32*);
    static JKRADCommand* loadToAram_Async(JKRDvdFile*, u32, JKRExpandSwitch, void (*)(u32), u32,
                                          u32, u32*);
    static JKRADCommand* callCommand_Async(JKRADCommand*);
    static bool syncAram(JKRADCommand*, int);

    static u32 getSZSBufferSize() { return sSZSBufferSize; }
    static void setSZSBufferSize(u32 size) { sSZSBufferSize = size; }
    static bool isErrorRetry() { return errorRetry; }

    // TODO: fix type
    static JSUList<JKRADCommand> sDvdAramAsyncList;
    static u32 sSZSBufferSize;
    static bool errorRetry;
};

inline JKRAramBlock *JKRDvdToAram(s32 entrynum, u32 p2, JKRExpandSwitch expSwitch, u32 p4, u32 p5, u32 *p6) {
    return JKRDvdAramRipper::loadToAram(entrynum, p2, expSwitch, p4, p5, p6);
}

#endif /* JKRDVDARAMRIPPER_H */
