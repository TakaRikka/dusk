#ifndef D_MENU_D_MENU_COLLECT_H
#define D_MENU_D_MENU_COLLECT_H

#include "d/d_drawlist.h"
#include "d/d_kankyo.h"

class CPaneMgr;
struct CSTControl;
class STControl;
class dMsgString_c;
class dMenu_save_c;
class dMenu_Option_c;
class dMenu_Letter_c;
class dMenu_Fishing_c;
class dMenu_Skill_c;
class dMenu_Insect_c;
class dSelect_cursor_c;

class dMenu_Collect2D_c;
class dMenu_Collect2DTop_c : public dDlst_base_c {
public:
    dMenu_Collect2DTop_c(dMenu_Collect2D_c* param_0) { mpCollect2D = param_0; }
    virtual void draw();
    virtual ~dMenu_Collect2DTop_c() {}

    /* 0x4 */ dMenu_Collect2D_c* mpCollect2D;
};

class dMenu_Collect2D_c : public dDlst_base_c {
public:
    dMenu_Collect2D_c(JKRExpHeap*, STControl*, CSTControl*);
    void _create();
    void _delete();
    void initialize();
    bool isFishIconVisible();
    bool isSkillIconVisible();
    bool isInsectIconVisible();
    void screenSet();
    void animationSet();
    void btkAnimeLoop0(J2DAnmTextureSRTKey*);
    void setBackAlpha();
    void cursorMove();
    void cursorPosSet();
    void changeSword();
    void changeShield();
    void changeClothe();
    void setArrowMaxNum(u8);
    void setWalletMaxNum(u16);
    void setSmellType();
    void setHeartPiece();
    void setPohMaxNum(u8);
    void setEquipItemFrameColorSword(int);
    void setEquipItemFrameColorShield(int);
    void setEquipItemFrameColorClothes(int);
    void setHIO(bool);
    u64 getItemTag(int, int, bool);
    void wait_init();
    void wait_proc();
    void save_open_init();
    void save_open_proc();
    void save_move_init();
    void save_move_proc();
    void save_close_init();
    void save_close_proc();
    void option_open_init();
    void option_open_proc();
    void option_move_init();
    void option_move_proc();
    void option_close_init();
    void option_close_proc();
    void letter_open_init();
    void letter_open_proc();
    void letter_move_init();
    void letter_move_proc();
    void letter_close_init();
    void letter_close_proc();
    void fishing_open_init();
    void fishing_open_proc();
    void fishing_move_init();
    void fishing_move_proc();
    void fishing_close_init();
    void fishing_close_proc();
    void skill_open_init();
    void skill_open_proc();
    void skill_move_init();
    void skill_move_proc();
    void skill_close_init();
    void skill_close_proc();
    void insect_open_init();
    void insect_open_proc();
    void insect_move_init();
    void insect_move_proc();
    void insect_close_init();
    void insect_close_proc();
    void _move();
    void _draw();
    void drawTop();
    bool isKeyCheck();
    bool isOutCheck();
    void setAButtonString(u16);
    void setBButtonString(u16);
    void setItemNameString(u8, u8);
    void setItemNameStringNull();

    virtual void draw() { _draw(); }
    virtual ~dMenu_Collect2D_c();

    u8 getCursorX() { return mCursorX; }
    u8 getCursorY() { return mCursorY; }
    CPaneMgr* getpMask() { return mpMaskPm; }
    CPaneMgr* getpLink() { return mpLinkPm; }
    J2DScreen* getIconScreen() { return mpScreenIcon; }
    dMenu_Insect_c* getInsectScreen() { return mpInsectScrn; }
    dMenu_Skill_c* getSkillScreen() { return mpSkillScrn; }
    dMenu_Fishing_c* getFishingScreen() { return mpFishingScrn; }
    dMenu_Letter_c* getLetterScreen() { return mpLetterScrn; }
    dMenu_Option_c* getOptionScreen() { return mpOptionScrn; }
    dMenu_save_c* getSaveScreen() { return mpSaveScrn; }
    J2DPicture* getBlackTex() { return mpBlackTex; }
    u8 getSubWindowOpenCheck() { return mSubWindowOpenCheck; }

private:
    /* 0x004 */ JKRExpHeap* mpHeap;
    /* 0x008 */ JKRExpHeap* mpSubHeap;
    /* 0x00C */ void* field_0xc;
    /* 0x010 */ STControl* mpStick;
    /* 0x014 */ CSTControl* mpCStick;
    /* 0x018 */ J2DScreen* mpScreen;
    /* 0x01C */ J2DScreen* mpScreenIcon;
    /* 0x020 */ dSelect_cursor_c* mpDrawCursor;
    /* 0x024 */ dMsgString_c* mpString;
    /* 0x028 */ u8 field_0x28[4];
    /* 0x02C */ J2DAnmTextureSRTKey* mpAnmKey;
    /* 0x030 */ u8 field_0x30[4];
    /* 0x034 */ f32 field_0x34;
    /* 0x038 */ f32 mFrame;
    /* 0x03C */ f32 field_0x3c;
    /* 0x040 */ u32 mIsWolf;
    /* 0x044 */ f32 field_0x44[2];
    /* 0x04C */ f32 field_0x4c[2];
    /* 0x054 */ f32 mBlueSmokePosX;
    /* 0x058 */ f32 mBlueSmokePosY;
    /* 0x05C */ f32 mBlueSmokeScale;
    /* 0x060 */ f32 mBlueSmokeAlpha;
    /* 0x064 */ f32 mHeartVesselPosX;
    /* 0x068 */ f32 mHeartVesselPosY;
    /* 0x06C */ f32 mHeartVesselScale;
    /* 0x070 */ f32 mHeartPiecePosX;
    /* 0x074 */ f32 mHeartPiecePosY;
    /* 0x078 */ f32 mHeartPieceScale;
    /* 0x07C */ cXyz mLinkGlobalCenterPos;
    /* 0x088 */ dMenu_Collect2DTop_c* mpDraw2DTop;
    /* 0x08C */ J2DPicture* mpBlackTex;
    /* 0x090 */ dMenu_save_c* mpSaveScrn;
    /* 0x094 */ dMenu_Option_c* mpOptionScrn;
    /* 0x098 */ dMenu_Letter_c* mpLetterScrn;
    /* 0x09C */ dMenu_Fishing_c* mpFishingScrn;
    /* 0x0A0 */ dMenu_Skill_c* mpSkillScrn;
    /* 0x0A4 */ dMenu_Insect_c* mpInsectScrn;
    /* 0x0A8 */ CPaneMgr* mpLinkPm;
    /* 0x0AC */ CPaneMgr* mpMaskPm;
    /* 0x0B0 */ CPaneMgr* mpSelPm[7][6];
    /* 0x158 */ u8 field_0x158[0x8];
    /* 0x160 */ CPaneMgr* mpModelBg;
    /* 0x164 */ CPaneMgr* mpHeartParent;
    /* 0x168 */ CPaneMgr* mpHeartPiece;
    /* 0x16C */ CPaneMgr* mpButtonAB[2];
    /* 0x174 */ CPaneMgr* mpButtonText[2];
    /* 0x17C */ u16 mCurrentAString;
    /* 0x17E */ u16 mCurrentBString;
    /* 0x180 */ u16 field_0x180;
    /* 0x182 */ u16 mItemNameString;
    /* 0x184 */ u16 field_0x184[7][6];
    /* 0x1D8 */ u16 field_0x1d8[7][6];
    /* 0x22C */ u8 mProcess;
    /* 0x22D */ u8 field_0x22d[7][6];
    /* 0x257 */ u8 mCursorX;
    /* 0x258 */ u8 mCursorY;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ u8 field_0x25a;
    /* 0x25B */ u8 mEquippedSword;
    /* 0x25C */ u8 mEquippedShield;
    /* 0x25D */ u8 mEquippedClothes;
    /* 0x25E */ u8 mSubWindowOpenCheck;
    /* 0x25F */ u8 field_0x25f;
    /* 0x260 */ u8 field_0x260;
    /* 0x261 */ u8 field_0x261;
};

class dMenu_Collect3D_c {
public:
    dMenu_Collect3D_c(JKRExpHeap*, dMenu_Collect2D_c*, CSTControl*);
    void _create();
    void _delete();
    void _move(u8, u8);
    void draw();
    void setJ3D(const char*, const char*, const char*);
    void set_mtx();
    void animePlay();
    void animeEntry();
    void createMaskModel();
    void createMirrorModel();
    static u8 getCrystalNum();
    static u8 getMirrorNum();
    static u8 getMaskMdlVisible();
    static void setupItem3D(Mtx);
    static void toItem3Dpos(f32, f32, f32, cXyz*);
    static void calcViewMtx(Mtx);

    virtual ~dMenu_Collect3D_c();

    static f32 mViewOffsetY;

private:
    /* 0x004 */ JKRExpHeap* mpHeap;
    /* 0x008 */ JKRSolidHeap* mpSolidHeap;
    /* 0x00C */ dMenu_Collect2D_c* mpCollect2D;
    /* 0x010 */ STControl* mpStick;
    /* 0x014 */ CSTControl* mpCStick;
    /* 0x018 */ J3DModel* mpModel;
    /* 0x01C */ mDoExt_bckAnm* mpBckAnm;
    /* 0x020 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x024 */ u32 mIsWolf;
    /* 0x028 */ dKy_tevstr_c mTevStr;
    /* 0x3B0 */ cXyz mMaskMirrorPos;
    /* 0x3BC */ csXyz mMaskMirrorAngle;
    /* 0x3C4 */ f32 mMaskMirrorOffsetX;
    /* 0x3C8 */ f32 mMaskMirrorOffsetY;
    /* 0x3CC */ f32 mMaskMirrorScale;
    /* 0x3D0 */ f32 mMaskMirrorAnmFrameBrk;
    /* 0x3D4 */ f32 mMaskMirrorAnmFrameBck;
    /* 0x3D8 */ s16 mLinkAngle;
};

class dMenu_Collect_c {
public:
    dMenu_Collect_c(JKRExpHeap*, STControl*, CSTControl*);
    void _create();
    void _delete();
    void _move();
    void draw();

    virtual ~dMenu_Collect_c();

    u8 getSubWindowOpenCheck() { return mpCollect2D->getSubWindowOpenCheck(); }
    bool isKeyCheck() { return mpCollect2D->isKeyCheck(); }
    bool isOutCheck() { return mpCollect2D->isOutCheck(); }

private:
    /* 0x4 */ dMenu_Collect2D_c* mpCollect2D;
    /* 0x8 */ dMenu_Collect3D_c* mpCollect3D;
};

#endif /* D_MENU_D_MENU_COLLECT_H */
