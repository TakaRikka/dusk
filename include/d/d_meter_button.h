#ifndef D_METER_D_METER_BUTTON_H
#define D_METER_D_METER_BUTTON_H

#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_drawlist.h"

class CPaneMgr;
class dMsgString_c;
class COutFont_c;

class dMeterButton_c : public dDlst_base_c {
public:
    enum {
        BUTTON_A_e,
        BUTTON_B_e,
        BUTTON_R_e,
        BUTTON_Z_e,
        BUTTON_3D_e,
        BUTTON_C_e,
        BUTTON_S_e,
        BUTTON_X_e,
        BUTTON_Y_e,
        BUTTON_NUN_e,
        BUTTON_REMO_e,
        BUTTON_REMO2_e,
        BUTTON_AR_e,
        BUTTON_3DB_e,
        BUTTON_NURE_e,
        BUTTON_REEL_e,
        BUTTON_REEL2_e,
        BUTTON_AB_e,
        BUTTON_TATE_e,
        BUTTON_NUNZ_e,
        BUTTON_NUNC_e,
        BUTTON_BIN_e,
        BUTTON_NONE_e,
    };

    enum {
        DIR_LEFT_e = (1 << 0),
        DIR_DOWN_e = (1 << 1),
        DIR_RIGHT_e = (1 << 2),
        DIR_UP_e = (1 << 3),
    };

    dMeterButton_c();
    int _create();
    int _execute(u32, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
                                 bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
                                 bool);
    int _delete();
    bool alphaAnimeButtonA(u32, bool);
    bool alphaAnimeButtonB(u32, bool);
    bool alphaAnimeButtonR(u32, bool);
    bool alphaAnimeButtonZ(u32, bool);
    bool alphaAnimeButton3D(u32, bool);
    bool alphaAnimeButtonC(u32, bool);
    bool alphaAnimeButtonS(u32, bool);
    bool alphaAnimeButtonX(u32, bool);
    bool alphaAnimeButtonY(u32, bool);
    bool alphaAnimeButtonNun(u32, bool);
    bool alphaAnimeButtonRemo(u32, bool);
    bool alphaAnimeButtonRemo2(u32, bool);
    bool alphaAnimeButtonAR(u32, bool);
    bool alphaAnimeButton3DB(u32, bool);
    bool alphaAnimeButtonNURE(u32, bool);
    bool alphaAnimeButtonReel(u32, bool);
    bool alphaAnimeButtonReel2(u32, bool);
    bool alphaAnimeButtonAB(u32, bool);
    bool alphaAnimeButtonTate(u32, bool);
    bool alphaAnimeButtonNunZ(u32, bool);
    bool alphaAnimeButtonNunC(u32, bool);
    bool alphaAnimeButtonBin(u32, bool);
    void screenInitButton();
    void screenInitText();
    void updateButton();
    void updateText(u32);
    void setAlphaButtonAAnimeMin();
    bool isFastSet(int);
    void setAlphaButtonAAnimeMax();
    void setAlphaButtonBAnimeMin();
    void setAlphaButtonBAnimeMax();
    void setAlphaButtonRAnimeMin();
    void setAlphaButtonRAnimeMax();
    void setAlphaButtonZAnimeMin();
    void setAlphaButtonZAnimeMax();
    void setAlphaButton3DAnimeMin();
    void setAlphaButton3DAnimeMax();
    void setAlphaButton3DVAnimeMin();
    void setAlphaButton3DVAnimeMax();
    void setAlphaButtonCAnimeMin();
    void setAlphaButtonCAnimeMax();
    void setAlphaButtonSAnimeMin();
    void setAlphaButtonSAnimeMax();
    void setAlphaButtonXAnimeMin();
    void setAlphaButtonXAnimeMax();
    void setAlphaButtonYAnimeMin();
    void setAlphaButtonYAnimeMax();
    void setAlphaButtonNunAnimeMin();
    void setAlphaButtonNunAnimeMax();
    void setAlphaButtonRemoAnimeMin();
    void setAlphaButtonRemoAnimeMax();
    void setAlphaButtonRemo2AnimeMin();
    void setAlphaButtonRemo2AnimeMax();
    void setAlphaButtonARAnimeMin();
    void setAlphaButtonARAnimeMax();
    void setAlphaButton3DBAnimeMin();
    void setAlphaButton3DBAnimeMax();
    void setAlphaButtonNUREAnimeMin();
    void setAlphaButtonNUREAnimeMax();
    void setAlphaButtonReelAnimeMin();
    void setAlphaButtonReelAnimeMax();
    void setAlphaButtonReel2AnimeMin();
    void setAlphaButtonReel2AnimeMax();
    void setAlphaButtonABAnimeMin();
    void setAlphaButtonABAnimeMax();
    void setAlphaButtonTateAnimeMin();
    void setAlphaButtonTateAnimeMax();
    void setAlphaButtonNunZAnimeMin();
    void setAlphaButtonNunZAnimeMax();
    void setAlphaButtonNunCAnimeMin();
    void setAlphaButtonNunCAnimeMax();
    void setAlphaButtonBinAnimeMin();
    void setAlphaButtonBinAnimeMax();
    void setAlphaButtonOAnimeMin(int);
    void setAlphaButtonOAnimeMax(int);
    void setAlphaIconAnimeMin();
    void setAlphaIconAnimeMax();
    bool isClose();
    void setString(char*, u8, u8, u8);
    void hideAll();
    f32 getCenterPosCalc(u8, char*, int);
    void trans_button(int, f32);
    void hide_button(u8);
    void pikariCheck();
    void paneTrans(CPaneMgr*, f32, f32, u8);

    virtual void draw();
    virtual ~dMeterButton_c();

    bool isSetButton(u8 i_no) { return mButtonTimers[i_no] <= 0; }
    void onButtonShowBit(int i_bit) { field_0x4b0 |= (u16)(1 << i_bit); }
    void offButtonShowBit(int i_bit) { field_0x4b0 &= ~(u16)(1 << i_bit); }
    bool isButtonShowBit(int i_bit) { return field_0x4b0 & (u16)(1 << i_bit); }

    static const int BUTTON_NUM = 22;

    /* 0x004 */ dMsgString_c* mpString_c;
    /* 0x008 */ J2DScreen* mpButtonScreen;
    /* 0x00C */ J2DScreen* field_0x00c;
    /* 0x010 */ J2DScreen* mpScreen;
    /* 0x014 */ J2DScreen* mpTextScreen;
    /* 0x018 */ CPaneMgr* mpParent;
    /* 0x01C */ CPaneMgr* mpButtonA;
    /* 0x020 */ CPaneMgr* mpButtonR;
    /* 0x024 */ CPaneMgr* mpButtonB;
    /* 0x028 */ CPaneMgr* mpButtonZ;
    /* 0x02C */ CPaneMgr* mpButton3D;
    /* 0x030 */ CPaneMgr* mpButtonC;
    /* 0x034 */ CPaneMgr* mpButtonS;
    /* 0x038 */ CPaneMgr* mpButtonX;
    /* 0x03C */ CPaneMgr* mpButtonY;
    /* 0x040 */ CPaneMgr* mpButtonNun;
    /* 0x044 */ CPaneMgr* mpButtonRemo;
    /* 0x048 */ CPaneMgr* mpButtonRemo2;
    /* 0x04C */ CPaneMgr* mpButtonAR;
    /* 0x050 */ CPaneMgr* mpButton3DB;
    /* 0x054 */ CPaneMgr* mpButtonNURE;
    /* 0x058 */ CPaneMgr* mpButtonReel;
    /* 0x05C */ CPaneMgr* mpButtonReel2;
    /* 0x060 */ CPaneMgr* mpButtonAB;
    /* 0x064 */ CPaneMgr* mpButtonTate;
    /* 0x068 */ CPaneMgr* mpButtonNunZ;
    /* 0x06C */ CPaneMgr* mpButtonNunC;
    /* 0x070 */ CPaneMgr* mpButtonBin;
    /* 0x074 */ u8 field_0x074[0x078 - 0x074];
    /* 0x078 */ CPaneMgr* mpText[2];
    /* 0x080 */ CPaneMgr* field_0x080;
    /* 0x084 */ J2DTextBox* mpTextBox[10];
    /* 0x0AC */ u8 field_0x0AC[0x0D4 - 0x0AC];
    /* 0x0D4 */ COutFont_c* mpOutFont;
    /* 0x0D8 */ CPaneMgr* mpFkAll_c;
    /* 0x0DC */ CPaneMgr* mpFkRoot_c;
    /* 0x0E0 */ CPaneMgr* mpTmRoot_c;
    /* 0x0E4 */ CPaneMgr* mpTm_c[2];
    /* 0x0EC */ CPaneMgr* field_0x0ec[2];
    /* 0x0F4 */ CPaneMgr* mpItem_c;
    /* 0x0F8 */ CPaneMgr* mpMidona;
    /* 0x0FC */ CPaneMgr* field_0x0fc[4];
    /* 0x10C */ JKRHeap* mpHeap;
    /* 0x110 */ void* mpFishingTex;
    /* 0x114 */ char mButtonText[2][15];
    /* 0x132 */ u8 field_0x132[0x134 - 0x132];
    /* 0x134 */ f32 field_0x134;
    /* 0x138 */ f32 field_0x138;
    /* 0x13C */ f32 field_0x13c;
    /* 0x140 */ f32 field_0x140;
    /* 0x144 */ f32 field_0x144;
    /* 0x148 */ f32 field_0x148;
    /* 0x14C */ f32 field_0x14c;
    /* 0x150 */ f32 field_0x150;
    /* 0x154 */ f32 field_0x154;
    /* 0x158 */ f32 field_0x158;
    /* 0x15C */ f32 field_0x15c;
    /* 0x160 */ f32 field_0x160;
    /* 0x164 */ f32 field_0x164;
    /* 0x168 */ f32 field_0x168;
    /* 0x16C */ f32 field_0x16c;
    /* 0x170 */ f32 field_0x170;
    /* 0x174 */ f32 field_0x174;
    /* 0x178 */ f32 field_0x178;
    /* 0x17C */ f32 field_0x17c;
    /* 0x180 */ f32 field_0x180;
    /* 0x184 */ f32 field_0x184;
    /* 0x188 */ f32 field_0x188;
    /* 0x18C */ f32 field_0x18c[BUTTON_NUM];
    /* 0x1E4 */ f32 field_0x1e4[2];
    /* 0x1EC */ f32 mButtonWidth[BUTTON_NUM];
    /* 0x244 */ f32 field_0x244[BUTTON_NUM];
    /* 0x29C */ f32 field_0x29c[2];
    /* 0x2A4 */ f32 field_0x2a4;
    /* 0x2A8 */ f32 field_0x2a8;
    /* 0x2AC */ u8 field_0x2AC[0x2E0 - 0x2AC];
    /* 0x2E0 */ f32 field_0x2e0[2];
    /* 0x2E8 */ f32 field_0x2e8[2];
    /* 0x2F0 */ f32 mParentCenterX;
    /* 0x2F4 */ f32 field_0x2f4[2];
    /* 0x2FC */ f32 field_0x2fc[2];
    /* 0x304 */ f32 field_0x304[BUTTON_NUM];
    /* 0x35C */ f32 field_0x35c;
    /* 0x360 */ f32 field_0x360;
    /* 0x364 */ Vec mVtxCenterA;
    /* 0x370 */ Vec mVtxCenterB;
    /* 0x37C */ Vec mVtxCenterR;
    /* 0x388 */ Vec mVtxCenterZ;
    /* 0x394 */ Vec mVtxCenter3D;
    /* 0x3A0 */ Vec mVtxCenterC;
    /* 0x3AC */ Vec mVtxCenterS;
    /* 0x3B8 */ Vec mVtxCenterX;
    /* 0x3C4 */ Vec mVtxCenterY;
    /* 0x3D0 */ u8 field_0x3D0[0x400 - 0x3D0];
    /* 0x400 */ Vec mVtxCenter3DB;
    /* 0x40C */ u8 field_0x40C[0x460 - 0x40C];
    /* 0x460 */ Vec mVtxCenterBin;
    /* 0x46C */ Vec mVtxCenterText0;
    /* 0x478 */ Vec mVtxCenterText1;
    /* 0x484 */ s16 mButtonTimers[BUTTON_NUM];
    /* 0x4B0 */ u16 field_0x4b0;
    /* 0x4B2 */ u16 mMsgID;
    /* 0x4B4 */ s16 field_0x4b4;
    /* 0x4B6 */ s16 field_0x4b6;
    /* 0x4B8 */ s16 field_0x4b8[2];
    /* 0x4BC */ u8 field_0x4bc[2];
    /* 0x4BE */ u8 field_0x4be[2];
    /* 0x4C0 */ bool mPlayedButtonSound[BUTTON_NUM];
    /* 0x4D6 */ u8 mMeterSubContent;
    /* 0x4D7 */ u8 field_0x4d7[2];
    /* 0x4D9 */ u8 field_0x4d9;
    /* 0x4DA */ u8 field_0x4da[4];
    /* 0x4E0 */ f32 mParentPosX;
    /* 0x4E4 */ f32 mParentPosY;
    /* 0x4E8 */ f32 mParentScale;
    /* 0x4EC */ f32 mButtonAPosX;
    /* 0x4F0 */ f32 mButtonAPosY;
    /* 0x4F4 */ f32 mButtonAScale;
    /* 0x4F8 */ f32 mButtonBPosX;
    /* 0x4FC */ f32 mButtonBPosY;
    /* 0x500 */ f32 mButtonBScale;
    /* 0x504 */ f32 mButtonRPosX;
    /* 0x508 */ f32 mButtonRPosY;
    /* 0x50C */ f32 mButtonRScale;
    /* 0x510 */ f32 mButtonZPosX;
    /* 0x514 */ f32 mButtonZPosY;
    /* 0x518 */ f32 mButtonZScale;
    /* 0x51C */ f32 mButton3DPosX;
    /* 0x520 */ f32 mButton3DPosY;
    /* 0x524 */ f32 mButton3DScale;
    /* 0x528 */ f32 mButtonCPosX;
    /* 0x52C */ f32 mButtonCPosY;
    /* 0x530 */ f32 mButtonCScale;
    /* 0x534 */ f32 mButtonSPosX;
    /* 0x538 */ f32 mButtonSPosY;
    /* 0x53C */ f32 mButtonSScale;
    /* 0x540 */ f32 mButtonXPosX;
    /* 0x544 */ f32 mButtonXPosY;
    /* 0x548 */ f32 mButtonXScale;
    /* 0x54C */ f32 mButtonYPosX;
    /* 0x550 */ f32 mButtonYPosY;
    /* 0x554 */ f32 mButtonYScale;
    /* 0x558 */ f32 field_0x558;
    /* 0x55C */ f32 field_0x55c;
    /* 0x560 */ f32 field_0x560;
    /* 0x564 */ f32 field_0x564;
    /* 0x568 */ f32 field_0x568;
    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ f32 field_0x570;
    /* 0x574 */ f32 field_0x574;
    /* 0x578 */ f32 field_0x578;
    /* 0x57C */ f32 field_0x57c;
    /* 0x580 */ f32 field_0x580;
    /* 0x584 */ f32 field_0x584;
    /* 0x588 */ f32 mButton3DBPosX;
    /* 0x58C */ f32 mButton3DBPosY;
    /* 0x590 */ f32 mButton3DBScale;
    /* 0x594 */ f32 field_0x594;
    /* 0x598 */ f32 field_0x598;
    /* 0x59C */ f32 field_0x59c;
    /* 0x5A0 */ f32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ f32 field_0x5ac;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ f32 field_0x5b8;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ f32 field_0x5c0;
    /* 0x5C4 */ f32 field_0x5c4;
    /* 0x5C8 */ f32 field_0x5c8;
    /* 0x5CC */ f32 field_0x5cc;
    /* 0x5D0 */ f32 field_0x5d0;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ f32 field_0x5dc;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ f32 mButtonBinPosX;
    /* 0x5EC */ f32 mButtonBinPosY;
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ f32 mTextPosX[2];
    /* 0x608 */ f32 mTextPosY[2];
    /* 0x610 */ f32 mTextScale[2];
    /* 0x618 */ f32 mItemPosX;
    /* 0x61C */ f32 mItemPosY;
    /* 0x620 */ f32 mItemScale;
    /* 0x624 */ f32 mMidonaPosX;
    /* 0x628 */ f32 mMidonaPosY;
    /* 0x62C */ f32 mMidonaScale;
};

#endif /* D_METER_D_METER_BUTTON_H */
