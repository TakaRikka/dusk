#ifndef D_METER_D_METER2_H
#define D_METER_D_METER2_H

#include "d/d_drawlist.h"
#include "f_op/f_op_msg.h"

class CPaneMgr;
class J2DAnmColor;
class J2DAnmTextureSRTKey;
class J2DAnmTransformKey;
class JKRExpHeap;
class JKRHeap;

class dMeter2Draw_c;
class dMeterMap_c;
class dMeterString_c;
class dMeterButton_c;
class dMeterHaihai_c;

class dMeterSub_c : public dDlst_base_c {
public:
    virtual void draw() {}
    virtual ~dMeterSub_c() {}
    virtual int _create() { return 0; }
    virtual int _execute(u32) { return 0; }
    virtual int _delete() { return 0; }
    virtual bool isDead() { return false; }
};

class dMeter2_c : public msg_class {
public:
    enum {
        /* 0x2 */ ARROW_MAX = 2,
        /* 0x3 */ PACHINKO_MAX,
    };

    enum {
        /* 0x0 */ X_ITEM,
        /* 0x1 */ X_STATUS,
        /* 0x2 */ Y_ITEM,
        /* 0x3 */ Y_STATUS,
    };

    int _create();
    int _execute();
    int _draw();
    int _delete();
    int emphasisButtonDelete();
    void setLifeZero();
    void checkStatus();
    void moveLife();
    void moveKantera();
    void moveOxygen();
    void moveLightDrop();
    void moveRupee();
    void moveKey();
    void moveButtonA();
    void moveButtonB();
    void moveButtonR();
    void moveButtonZ();
    void moveButton3D();
    void moveButtonC();
    void moveButtonS();
    void moveButtonXY();
    void moveButtonCross();
    void moveTouchSubMenu();
    void moveSubContents();
    void move2DContents();
    void checkSubContents();
    void check2DContents();
    void moveBombNum();
    void moveBottleNum();
    void moveArrowNum();
    void movePachinkoNum();
    void alphaAnimeLife();
    void alphaAnimeKantera();
    void alphaAnimeOxygen();
    void alphaAnimeLightDrop();
    void alphaAnimeRupee();
    void alphaAnimeKey();
    void alphaAnimeButton();
    void alphaAnimeButtonCross();
    bool isShowLightDrop();
    void killSubContents(u8);
    u8 isKeyVisible();
    int isArrowEquip();
    int isPachinkoEquip();

    void setNowLifeGauge(s16 life) { mNowLifeGauge = life; }
    void onArrowSoundBit(int bit) { mArrowSound |= (1 << bit); }
    void offArrowSoundBit(int bit) { mArrowSound &= ~(1 << bit); }
    bool isArrowSoundBit(int bit) { return mArrowSound & (1 << bit) ? true : false; }
    void onRupeeSoundBit(int bit) { mRupeeSound |= (1 << bit); }
    void offRupeeSoundBit(int bit) { mRupeeSound &= ~(1 << bit); }
    bool isRupeeSoundBit(int bit) { return mRupeeSound & (1 << bit); }
    dMeter2Draw_c* getMeterDrawPtr() { return mpMeterDraw; }
    s16 getNowLifeGauge() { return mNowLifeGauge; }
    u8 getSubContents() { return mSubContentType; }
    u16 getSubContentsStringType() { return mSubContentsStringType; }
    bool isShowFlag(int i_no) { return field_0x1e6 & (1 << i_no); }
    void onShowFlag(int i_no) { field_0x1e6 |= (1 << i_no); }

private:
    /* 0x0FC */ int field_0xfc;
    /* 0x100 */ JKRExpHeap* mpHeap;
    /* 0x104 */ JKRExpHeap* mpSubHeap;
    /* 0x108 */ JKRHeap* field_0x108;
    /* 0x10C */ dMeter2Draw_c* mpMeterDraw;
    /* 0x110 */ dMeterSub_c* mpSubContents;
    /* 0x114 */ dMeterString_c* mpSubSubContents;
    /* 0x118 */ dMeterButton_c* mpEmpButton;
    /* 0x11C */ dMeterHaihai_c* field_0x11c;  // type is a guess for now
    /* 0x120 */ dMeterMap_c* mpMap;
    /* 0x124 */ u32 mStatus;
    /* 0x128 */ u32 field_0x128;
    /* 0x12C */ int field_0x12c;
    /* 0x130 */ f32 field_0x130;
    /* 0x134 */ f32 mButtonATalkPosX[2];
    /* 0x13C */ f32 mButtonATalkPosY[2];
    /* 0x144 */ f32 field_0x144;
    /* 0x148 */ f32 field_0x148[2];
    /* 0x150 */ f32 field_0x150[2];
    /* 0x158 */ f32 field_0x158;
    /* 0x15C */ f32 field_0x15c;
    /* 0x160 */ f32 field_0x160[4];
    /* 0x170 */ u8 field_0x170[4];
    /* 0x174 */ f32 field_0x174[4];
    /* 0x184 */ u8 field_0x184[4];
    /* 0x188 */ f32 field_0x188;
    /* 0x18C */ f32 field_0x18c;
    /* 0x190 */ s16 field_0x190;
    /* 0x192 */ s16 mNowLifeGauge;
    /* 0x194 */ s16 mMaxLife;
    /* 0x196 */ s16 mNowMagic;
    /* 0x198 */ s16 mMaxMagic;
    /* 0x19A */ s16 field_0x19a;
    /* 0x19C */ int mNowOil;
    /* 0x1A0 */ int mMaxOil;
    /* 0x1A4 */ int mNowOxygen;
    /* 0x1A8 */ int mMaxOxygen;
    /* 0x1AC */ int field_0x1ac;
    /* 0x1B0 */ s16 mRupeeNum;
    /* 0x1B2 */ s16 mKeyNum;
    /* 0x1B4 */ s16 field_0x1b4;
    /* 0x1B6 */ u16 mSubContentsStringType;
    /* 0x1B8 */ u16 field_0x1b8[5];
    /* 0x1C2 */ u8 mLightDropNum;
    /* 0x1C3 */ u8 mNeedLightDropNum;
    /* 0x1C4 */ u8 mDoStatus;
    /* 0x1C5 */ u8 mAStatus;
    /* 0x1C6 */ u8 field_0x1c6;
    /* 0x1C7 */ u8 mCollectSmell;
    /* 0x1C8 */ u8 mRStatus;
    /* 0x1C9 */ u8 mZStatus;
    /* 0x1CA */ u8 m3DStatus;
    /* 0x1CB */ u8 mCStickStatus;
    /* 0x1CC */ u8 mSButtonStatus;
    /* 0x1CD */ u8 mNunStatus;
    /* 0x1CE */ u8 mRemoConStatus;
    /* 0x1CF */ u8 mNunZStatus;
    /* 0x1D0 */ u8 mNunCStatus;
    /* 0x1D1 */ u8 mBottleStatus;
    /* 0x1D2 */ u8 mItemStatus[4];
    /* 0x1D6 */ u8 field_0x1d6[2];
    /* 0x1D8 */ u8 field_0x1d8[2];
    /* 0x1DA */ u8 mArrowNum;
    /* 0x1DB */ u8 mPachinkoNum;
    /* 0x1DC */ u8 mDoSetFlag;
    /* 0x1DD */ u8 mASetFlag;
    /* 0x1DE */ u8 mRSetFlag;
    /* 0x1DF */ u8 mXSetFlag;
    /* 0x1E0 */ u8 mYSetFlag;
    /* 0x1E1 */ u8 field_0x1e1;
    /* 0x1E2 */ u8 mEquipSword;
    /* 0x1E3 */ u8 field_0x1e3;
    /* 0x1E4 */ u8 field_0x1e4;
    /* 0x1E5 */ u8 mSubContentType;
    /* 0x1E6 */ u8 field_0x1e6;
    /* 0x1E7 */ u8 field_0x1e7;
    /* 0x1E8 */ u8 field_0x1e8;
    /* 0x1E9 */ u8 field_0x1e9;
    /* 0x1EA */ u8 mRupeeSound;
    /* 0x1EB */ u8 mArrowSound;
    /* 0x1EC */ u8 field_0x1ec;
    /* 0x1ED */ u8 field_0x1ed;
    /* 0x1EE */ u8 mLifeCountType;
    /* 0x1EF */ u8 mBottleNum[4];
    /* 0x1F3 */ u8 mBombNum[3];
    /* 0x1F6 */ u8 mBombMax[3];
    /* 0x1F9 */ u8 mItemMaxNum[4];
    /* 0x1FD */ u8 field_0x1fd;
    /* 0x1FE */ u8 field_0x1fe;
    /* 0x1FF */ u8 field_0x1ff;
    /* 0x200 */ u8 field_0x200;
    /* 0x201 */ u8 field_0x201;
    /* 0x204 */ f32 mLifeGaugePosX;
    /* 0x208 */ f32 mLifeGaugePosY;
    /* 0x20C */ f32 mLifeGaugeScale;
    /* 0x210 */ f32 mHeartScale;
    /* 0x214 */ f32 mLargeHeartScale;
    /* 0x218 */ u8 field_0x218[0x22C - 0x218];
    /* 0x22C */ f32 mLanternMeterScale;
    /* 0x230 */ f32 mLanternMeterPosX;
    /* 0x234 */ f32 mLanternMeterPosY;
    /* 0x238 */ f32 mOxygenMeterScale;
    /* 0x23C */ f32 mOxygenMeterPosX;
    /* 0x240 */ f32 mOxygenMeterPosY;
    /* 0x244 */ u8 field_0x244[0x246 - 0x244];
    /* 0x246 */ s16 field_0x246;
    /* 0x248 */ s16 field_0x248;
    /* 0x24A */ u8 field_0x24a[0x268 - 0x24a];
    /* 0x268 */ f32 mRupeeKeyScale;
    /* 0x26C */ f32 mRupeeKeyPosX;
    /* 0x270 */ f32 mRupeeKeyPosY;
    /* 0x274 */ f32 mRupeeScale;
    /* 0x278 */ f32 mRupeePosX;
    /* 0x27C */ f32 mRupeePosY;
    /* 0x280 */ f32 mRupeeFramePosY;
    /* 0x284 */ f32 mRupeeFrameScale;
    /* 0x288 */ f32 mRupeeFramePosX;
    /* 0x28C */ f32 mRupeeCountScale;
    /* 0x290 */ f32 mRupeeCountPosX;
    /* 0x294 */ f32 mRupeeCountPosY;
    /* 0x298 */ u8 mWalletSize;
    /* 0x29C */ f32 mKeyScale;
    /* 0x2A0 */ f32 mKeyPosX;
    /* 0x2A4 */ f32 mKeyPosY;
    /* 0x2A8 */ f32 mKeyNumScale;
    /* 0x2AC */ f32 mKeyNumPosX;
    /* 0x2B0 */ f32 mKeyNumPosY;
    /* 0x2B4 */ f32 mButtonAScale;
    /* 0x2B8 */ f32 mButtonAPosX;
    /* 0x2BC */ f32 mButtonAPosY;
    /* 0x2C0 */ f32 mButtonATextScale;
    /* 0x2C4 */ f32 mButtonATextPosX;
    /* 0x2C8 */ f32 mButtonATextPosY;
    /* 0x2CC */ f32 field_0x2cc;
    /* 0x2D0 */ f32 mButtonBPosX;
    /* 0x2D4 */ f32 mButtonBPosY;
    /* 0x2D8 */ u8 field_0x2d8[0x2e4 - 0x2d8];
    /* 0x2E4 */ f32 mButtonATalkScale[2];
    /* 0x2EC */ f32 field_0x2ec[2];
    /* 0x2F4 */ f32 mItemBBaseScale[2];
    /* 0x2FC */ f32 mItemBBasePosX[2];
    /* 0x304 */ f32 mItemBBasePosY[2];
    /* 0x30C */ f32 mButtonBFontScale;
    /* 0x310 */ f32 mButtonBFontPosX;
    /* 0x314 */ f32 mButtonBFontPosY;
    /* 0x318 */ u8 field_0x318[0x324 - 0x318];
    /* 0x324 */ f32 field_0x324;
    /* 0x328 */ f32 field_0x328;
    /* 0x32C */ f32 field_0x32c;
    /* 0x330 */ f32 field_0x330;
    /* 0x334 */ f32 field_0x334;
    /* 0x338 */ f32 field_0x338;
    /* 0x33C */ f32 field_0x33c;
    /* 0x340 */ f32 field_0x340;
    /* 0x344 */ f32 field_0x344;
    /* 0x348 */ f32 field_0x348;
    /* 0x34C */ f32 field_0x34c;
    /* 0x350 */ f32 field_0x350;
    /* 0x354 */ u8 field_0x354[0x360 - 0x354];
    /* 0x360 */ f32 field_0x360;
    /* 0x364 */ f32 field_0x364;
    /* 0x368 */ f32 field_0x368;
    /* 0x36C */ f32 field_0x36c;
    /* 0x370 */ f32 field_0x370;
    /* 0x374 */ f32 field_0x374;
    /* 0x378 */ u8 field_0x378[0x384 - 0x378];
    /* 0x384 */ f32 field_0x384;
    /* 0x388 */ f32 field_0x388;
    /* 0x38C */ f32 field_0x38c;
    /* 0x390 */ f32 field_0x390[3];
    /* 0x39C */ f32 field_0x39c[2];
    /* 0x3A4 */ f32 field_0x3a4[2];
    /* 0x3AC */ f32 field_0x3ac[2];
    /* 0x3B4 */ f32 field_0x3b4[2];
    /* 0x3BC */ f32 field_0x3bc[2];
    /* 0x3C4 */ f32 field_0x3c4[2];
    /* 0x3CC */ u8 field_0x3cc[0x3e4 - 0x3CC];
    /* 0x3E4 */ f32 field_0x3e4;
    /* 0x3E8 */ f32 field_0x3e8;
    /* 0x3EC */ f32 field_0x3ec;
    /* 0x3F0 */ f32 field_0x3f0;
    /* 0x3F4 */ f32 field_0x3f4;
    /* 0x3F8 */ f32 field_0x3f8;
    /* 0x3FC */ u8 field_0x3fc[0x420 - 0x3fc];
    /* 0x420 */ f32 mVesselPosX;
    /* 0x424 */ f32 mVesselPosY;
    /* 0x428 */ f32 mVesselScale;
    /* 0x42C */ f32 mVesselAlpha;
    /* 0x430 */ f32 field_0x430;
    /* 0x434 */ u8 field_0x434[0x448 - 0x434];
    /* 0x448 */ f32 mButtonCrossOFFPosX;
    /* 0x44C */ f32 mButtonCrossOFFPosY;
    /* 0x450 */ f32 mButtonCrossScale;
    /* 0x454 */ f32 mButtonCrossONPosX;
    /* 0x458 */ f32 mButtonCrossONPosY;
    /* 0x45C */ f32 mButtonCrossTextScale;
    /* 0x460 */ u8 field_0x460[0x4bc - 0x460];
    /* 0x4BC */ u8 field_0x4bc;
    /* 0x4BC */ u8 field_0x4bd;
};

#endif /* D_METER_D_METER2_H */
