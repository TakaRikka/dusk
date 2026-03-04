#ifndef Z2AUDIOCS_SPKTABLE_H
#define Z2AUDIOCS_SPKTABLE_H

#include "JSystem/JUtility/JUTAssert.h"
#include <revolution/types.h>
#include <revolution/wpad.h>

struct SpkTableParams {
    /* 0x00 */ u16 mWaveNum;
    /* 0x02 */ u8 field_0x02;
    /* 0x03 */ u8 mVolume;
    /* 0x04 */ u16 mMsec;
    /* 0x06 */ u8 pad_0x06[2];
};

class SpkTable {
public:
    SpkTable(void);
    void setResource(void* res);

    inline s32 getName(s32 num) {
        JUT_ASSERT(0x35, num >= 0);
        JUT_ASSERT(0x36, num < mNumOfSound);
        return *(mDataOffsets + num);
    }
    inline s32 getNumOfSound() const { return mNumOfSound; }
    inline bool isValid(void) const { return mIsInitialized; }
    inline SpkTableParams* getParams(s32 num) {
        JUT_ASSERT(46, num >= 0);
        JUT_ASSERT(47, num < mNumOfSound);
        return (SpkTableParams*)mEntryOffset + num;
    }

    // private:
    /* 0x00 */ bool mIsInitialized;
    /* 0x04 */ s32 mNumOfSound;
    /* 0x08 */ u32 mEntryOffset;
    /* 0x0C */ s32* mDataOffsets;
};

#endif /* Z2AUDIOCS_SPKTABLE_H */
