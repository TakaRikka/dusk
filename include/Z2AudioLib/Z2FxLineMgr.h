#ifndef Z2FXLINEMGR_H
#define Z2FXLINEMGR_H

#include "JSystem/JAudio2/JASGadget.h"

class JKRArchive;
class JKRHeap;

struct Z2FxLineEditNode {};

struct Z2FxLineConfig {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ s16 field_0x8[8];
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1a;
    /* 0x1B */ u8 field_0x1b;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ s16 field_0x1e;
    /* 0x20 */ s16 field_0x20[8];
};  // Size: 0x30

struct Z2FxLineMgr : public JASGlobalInstance<Z2FxLineMgr> {
    Z2FxLineMgr();
    void initDataArc(JKRArchive* arc, JKRHeap* heap);
    void setLineID(s8 fxID, bool, bool);
    void setLine(Z2FxLineConfig* config, bool, bool);
    void setFxForceOff(bool);
    void setUnderWaterFx(bool isUnderWaterFx);
    void setSceneFx(s32 sceneNo);

    /* 0x00 */ Z2FxLineConfig* mConfig;
    /* 0x04 */ void* mFxLineBuffer[4];
    /* 0x14 */ s8 mLineID;
    /* 0x15 */ u8 mFxDataNum;
    /* 0x16 */ bool mSetUnderWaterFx;
    /* 0x18 */ Z2FxLineEditNode* mHIOEdit;
};  // Size: 0x1C

inline Z2FxLineMgr* Z2GetFxLineMgr() {
    return JASGlobalInstance<Z2FxLineMgr>::getInstance();
}

#endif /* Z2FXLINEMGR_H */
