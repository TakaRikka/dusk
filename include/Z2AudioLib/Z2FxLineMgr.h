#ifndef Z2FXLINEMGR_H
#define Z2FXLINEMGR_H

#include "JSystem/JAHostIO/JAHFrameNode.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASGadget.h"

class JKRArchive;
class JKRExpHeap;
class JKRHeap;

struct Z2FxLineConfigSingle {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8[8];
};

struct Z2FxLineConfig {
    /* 0x00 */ Z2FxLineConfigSingle unk0;
    /* 0x18 */ Z2FxLineConfigSingle unk18;
};  // Size: 0x30

class Z2FxLineEditNode : public JAHFrameNode {
public:
    class LineStereo : public JAHFrameNode {
    public:
        LineStereo(bool dolby) : JAHFrameNode(NULL) {
            unk70.unk0.unk0 = 1;
            unk70.unk0.unk1 = 0;
            unk70.unk0.unk4 = 0x6000;
            unk70.unk0.unk2 = 1;
            unk70.unk0.unk6 = 0;
            unk70.unk0.unk3 = 0x40;
            unk70.unk0.unk8[0] = 0;
            unk70.unk0.unk8[1] = 0;
            unk70.unk0.unk8[2] = 0;
            unk70.unk0.unk8[3] = 0;
            unk70.unk0.unk8[4] = 0;
            unk70.unk0.unk8[5] = 0;
            unk70.unk0.unk8[6] = 0x3fff;
            unk70.unk0.unk8[7] = 0;

            unk70.unk18.unk0 = 1;
            unk70.unk18.unk1 = 0;
            unk70.unk18.unk4 = 0;
            unk70.unk18.unk2 = 1;
            unk70.unk18.unk6 = 0x6000;
            unk70.unk18.unk3 = 0x40;
            unk70.unk18.unk8[0] = 0;
            unk70.unk18.unk8[1] = 0;
            unk70.unk18.unk8[2] = 0;
            unk70.unk18.unk8[3] = 0;
            unk70.unk18.unk8[4] = 0;
            unk70.unk18.unk8[5] = 0;
            unk70.unk18.unk8[6] = 0x3fff;
            unk70.unk18.unk8[7] = 0;

            if (dolby)
                setNodeName("Dolby Stereo");
            else
                setNodeName("Norma Stereo");
            mIsDolby = dolby;
        }

        void message(JAHControl&);
        void propertyEvent(JAH_P_Event, uintptr_t);
        void lineEditSingle(Z2FxLineConfigSingle*, JAHControl&);

        /* 0x70 */ Z2FxLineConfig unk70;
        /* 0xA0 */ bool mIsDolby;
    };

    Z2FxLineEditNode(JKRExpHeap*);

    void message(JAHControl&);
    void syncSetting(u8, JASDsp::FxlineConfig_*);
    void update(bool);
    void saveLine(LineStereo*);
    void loadLine(LineStereo*);

    /* 0x70 */ JKRExpHeap* mHeap;
    /* 0x74 */ LineStereo* unk74;
    /* 0x78 */ LineStereo* unk78;
};

struct Z2FxLineMgr : public JASGlobalInstance<Z2FxLineMgr> {
    Z2FxLineMgr();
    void initDataArc(JKRArchive* arc, JKRHeap* heap);
    void setLineID(s8 fxID, bool, bool);
    void setLine(Z2FxLineConfig* config, bool, bool);
    void setFxForceOff(bool);
    void setUnderWaterFx(bool isUnderWaterFx);
    void setSceneFx(s32 sceneNo);

    void setHIOEdit(Z2FxLineEditNode* hioEdit) { mHIOEdit = hioEdit; }

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
