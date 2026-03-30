#ifndef Z2WAVEARCLOADER_H
#define Z2WAVEARCLOADER_H

#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JAudio2/JASWaveInfo.h"

class Z2WaveArcLoader : public JAWWindow {
public:
    Z2WaveArcLoader();
    virtual ~Z2WaveArcLoader();

    virtual void onDraw(JAWGraphContext*);

    void checkWaveBank();
    void checkWaveArc();

    virtual void onKeyUp(const JUTGamePad&);
    virtual void onKeyDown(const JUTGamePad&);
    virtual void onKeyLeft(const JUTGamePad&);
    virtual void onKeyRight(const JUTGamePad&);
    virtual void onTrigA(const JUTGamePad&);
    virtual void onTrigB(const JUTGamePad&);
    virtual void onTrigZ(const JUTGamePad&);

    /* 0x3F0 */ JASWaveBank* mpWaveBank;
    /* 0x3F4 */ JASWaveArc* mpWaveArc;
    /* 0x3F8 */ u32 mWaveUsedSize;
    /* 0x3FC */ int mTotalUsedSize;
    /* 0x400 */ u8 mBankNo;
    /* 0x404 */ u32 mArcCount;
    /* 0x408 */ u32 field_0x408;
    /* 0x40C */ u32 field_0x40c;
    /* 0x410 */ u32 field_0x410;
    /* 0x414 */ u8 field_0x414;
    /* 0x415 */ u8 field_0x415;
    /* 0x416 */ bool mIsLoadTail;
};

#endif /* Z2WAVEARCLOADER_H */
