#ifndef Z2ENVSEMGR_H
#define Z2ENVSEMGR_H

#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2SeMgr.h"

enum WindType {
    WIND_TYPE_0,
    WIND_TYPE_1,
    WIND_TYPE_2,
    WIND_TYPE_3,
};

struct Z2EnvSeBase {
    Z2EnvSeBase(Vec* posPtr = NULL);
    ~Z2EnvSeBase();
    bool startEnvSe(JAISoundID soundID, f32 fxMix, f32 volume, f32 pan, f32 dolby, f32 pitch, u32 moveSteps);
    bool startEnvSeLevel(JAISoundID soundID, f32 fxMix, f32 volume, f32 pan, f32 dolby, f32 pitch, u32 moveSteps);

    JAISoundHandle* getHandle() { return &mHandle; }
    void setPos(Vec* pos) { mPos = pos; }

    /* 0x0 */ Vec* mPos;
    /* 0x4 */ JAISoundHandle mHandle;
};  // Size: 0x8

struct Z2EnvSeAutoPan : public Z2EnvSeBase {
    Z2EnvSeAutoPan();
    ~Z2EnvSeAutoPan();
    void setPanParam(f32 pan, f32 dolby, bool, bool, f32, f32);
    void calcPan();
    bool startEnvSeAutoPanLevel(JAISoundID soundID, f32 fxMix, f32 volume, f32 pitch);

    /* 0x08 */ f32 mPan;
    /* 0x0C */ f32 mDolby;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ bool field_0x18;
    /* 0x19 */ bool field_0x19;
};  // Size: 0x1C

struct Z2EnvSeDir : public Z2EnvSeBase {
    Z2EnvSeDir(Vec* dir = NULL);
    ~Z2EnvSeDir();
    void setPanDir(Vec* dir);
    void calcPan(f32);
    bool startEnvSeDirLevel(JAISoundID soundID, f32 fxMix, f32 volume);

    /* 0x08 */ f32 mPan;
    /* 0x0C */ f32 mDolby;
    /* 0x10 */ Vec mPanDir;
};  // Size: 0x1C

struct Z2EnvSeMgr : public JASGlobalInstance<Z2EnvSeMgr> {
    Z2EnvSeMgr();
    ~Z2EnvSeMgr();
    void framework();
    void resetScene();
    void resetSceneInner();
    void initSceneEnvSe(s32 sceneID, s8 room, f32);
    void playSceneEnvSe();
    void initStaticEnvSe(u8, u8, u8, u8, Vec*);
    bool startStaticEnvSe(s8 reverb);
    void registWindowPos(Vec* posPtr);
    bool startRainSe(s32, s8);
    void startNearThunderSe(s8 reverb);
    void startFarThunderSe(Vec* posPtr, s8 reverb);
    void setSnowPower(s8 reverb);
    void initStrongWindSe();
    void setWindDirection(Vec* dir);
    bool startStrongWindSe(s8 reverb);
    void initRiverSe(u8, u8, u8, u8);
    void registRiverSePos(Vec* posPtr);
    void setHyrulSewerOpen(bool isSewerOpen);
    bool startRiverSe(s8 reverb);
    void initFallSe(u8, u8, u8, u8);
    void registFallSePos(Vec* posPtr);
    bool startFallSe(s8 reverb);
    void initEtcSe(u8, u8, u8, u8);
    void registEtcSePos(Vec* posPtr);
    bool startEtcSe(s8 reverb);
    void registWolfSmellSePos(Vec* posPtr);
    void startFogWipeTrigger(Vec* pos);
    void setFogWipeWidth(f32 width);
    f32 getFogDensity();
    bool startFogSe();
    void initLv3WaterSe(u8, u8, u8, u8);
    void registLv3WaterSePos(u8, Vec* posPtr);
    void startLv3WaterSe(s8 reverb);

    void setWindType(u8 type) { mWindType = type; }
    void registSmellSePos(Vec* posPtr) { registEtcSePos(posPtr); }
    void startSmellSe(s8 reverb) { startEtcSe(reverb); }
    void initSmellSe(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
        initEtcSe(param_0, param_1, param_2, param_3);
    }

    /* 0x000 */ Z2EnvSeAutoPan field_0x0;
    /* 0x01C */ Z2EnvSeAutoPan field_0x1c;
    /* 0x038 */ u8 field_0x38;
    /* 0x039 */ u8 field_0x39;
    /* 0x03C */ Vec field_0x3c;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ f32 field_0x54;
    /* 0x058 */ f32 field_0x58;
    /* 0x05C */ f32 field_0x5c;
    /* 0x060 */ f32 field_0x60;
    /* 0x064 */ Z2MultiSeMgr field_0x64;
    /* 0x080 */ Z2EnvSeBase field_0x80;
    /* 0x088 */ Z2EnvSeBase field_0x88;
    /* 0x090 */ Z2Calc::FNoise1f field_0x90;
    /* 0x0A0 */ Z2Calc::FNoise1f field_0xa0;
    /* 0x0B0 */ Z2Calc::FNoise1f field_0xb0;
    /* 0x0C0 */ Z2Calc::FNoise1f field_0xc0;
    /* 0x0D0 */ Z2Calc::FNoise1f field_0xd0;
    /* 0x0E0 */ Z2Calc::FNoise1f field_0xe0;
    /* 0x0F0 */ Z2Calc::FNoise1f field_0xf0;
    /* 0x100 */ Z2Calc::FNoise1f field_0x100;
    /* 0x110 */ Z2EnvSeBase field_0x110;
    /* 0x118 */ Z2EnvSeBase field_0x118;
    /* 0x120 */ Z2Calc::FNoise1f field_0x120;
    /* 0x130 */ Z2Calc::FNoise1f field_0x130;
    /* 0x140 */ s8 field_0x140;
    /* 0x141 */ s8 mSnowPower;
    /* 0x144 */ Z2EnvSeDir field_0x144;
    /* 0x160 */ u8 mWindType;
    /* 0x164 */ Z2MultiSeMgr mRiverSeMgr;
    /* 0x180 */ Z2EnvSeBase field_0x180;
    /* 0x188 */ u8 field_0x188;
    /* 0x18C */ Z2Calc::FNoise1f field_0x18c;
    /* 0x19C */ Z2Calc::FNoise1f field_0x19c;
    /* 0x1AC */ f32 field_0x1ac;
    /* 0x1B0 */ f32 field_0x1b0;
    /* 0x1B4 */ f32 field_0x1b4;
    /* 0x1B8 */ s8 field_0x1b8;
    /* 0x1BC */ Z2MultiSeMgr mFallSeMgr;
    /* 0x1D8 */ Z2EnvSeBase field_0x1d8;
    /* 0x1E0 */ u8 field_0x1e0;
    /* 0x1E4 */ f32 field_0x1e4;
    /* 0x1E8 */ f32 field_0x1e8;
    /* 0x1EC */ f32 field_0x1ec;
    /* 0x1F0 */ Z2MultiSeMgr mEtcSeMgr;
    /* 0x20C */ Z2EnvSeBase field_0x20c;
    /* 0x214 */ u8 field_0x214;
    /* 0x218 */ f32 field_0x218;
    /* 0x21C */ f32 field_0x21c;
    /* 0x220 */ f32 field_0x220;
    /* 0x224 */ Z2MultiSeMgr field_0x224;
    /* 0x240 */ Z2EnvSeBase field_0x240;
    /* 0x248 */ u8 field_0x248;
    /* 0x24C */ f32 field_0x24c;
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ Z2MultiSeMgr field_0x258;
    /* 0x274 */ Z2EnvSeBase field_0x274;
    /* 0x27C */ u8 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ Z2MultiSeMgr field_0x28c;
    /* 0x2A8 */ Z2EnvSeBase field_0x2a8;
    /* 0x2B0 */ u8 field_0x2b0;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ f32 field_0x2b8;
    /* 0x2BC */ f32 field_0x2bc;
    /* 0x2C0 */ Z2MultiSeMgr mWolfSmellSeMgr;
    /* 0x2DC */ Z2EnvSeBase field_0x2dc;
    /* 0x2E4 */ u8 field_0x2e4;
    /* 0x2E8 */ Z2Calc::FNoise1f field_0x2e8;
    /* 0x2F8 */ Z2Calc::FNoise1f field_0x2f8;
    /* 0x308 */ f32 mFogDensity;
};  // Size: 0x30C

STATIC_ASSERT(sizeof(Z2EnvSeMgr) == 0x30C);

extern Z2EnvSeMgr g_mEnvSeMgr;

inline Z2EnvSeMgr* Z2GetEnvSeMgr() {
    return JASGlobalInstance<Z2EnvSeMgr>::getInstance();
}

#endif /* Z2ENVSEMGR_H */
