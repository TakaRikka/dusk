#ifndef D_MENU_D_MENU_RING_H
#define D_MENU_D_MENU_RING_H

#include "d/d_com_inf_game.h"

class CPaneMgr;
class CPaneMgrAlpha;
struct CSTControl;
class J2DTextBox;
class STControl;
class dKantera_icon_c;
class dMsgString_c;
class dMenu_ItemExplain_c;
class dSelect_cursor_c;

class dMenu_Ring_c : public dDlst_base_c {
public:
    enum dMenu_Ring_Status {
        /* 0 */ STATUS_WAIT,
        /* 1 */ STATUS_MOVE,
        /* 2 */ STATUS_EXPLAIN,
        /* 3 */ STATUS_EXPLAIN_FORCE,
    };

    dMenu_Ring_c(JKRExpHeap*, STControl*, CSTControl*, u8);
    void _create();
    void _delete();
    void _move();
    void _draw();
    void setKanteraPos(f32, f32);
    bool isOpen();
    bool isMoveEnd();
    bool isClose();
    u8 getStickInfo(STControl*);
    s16 calcStickAngle(STControl*, u8);
    void setRotate();
    void setItemScale(int, f32);
    void setButtonScale(int, f32);
    void setItem();
    void setJumpItem(bool);
    void setScale();
    void setNameString(u32);
    void setActiveCursor();
    void setMixItem();
    void drawItem();
    void drawItem2();
    void stick_wait_init();
    void stick_wait_proc();
    void stick_move_init();
    void stick_move_proc();
    void stick_explain_init();
    void stick_explain_force_init();
    void stick_explain_proc();
    void stick_explain_force_proc();
    void setSelectItem(int, u8);
    void drawSelectItem();
    void setSelectItemForce(int);
    u8 getCursorPos(u8);
    u8 getItemNum(u8);
    u8 getItemMaxNum(u8);
    bool checkExplainForce();
    bool checkCombineBomb(int);
    void setCombineBomb(int);
    void drawNumber(int, int, f32, f32);
    u8 getItem(int, u8);
    void setDoStatus(u8);
    bool isMixItemOn();
    bool isMixItemOff();
    void setMixMessage();
    void textScaleHIO();
    void textCentering();
    f32 clacEllipseFunction(f32, f32, f32);
    f32 calcDistance(f32, f32, f32, f32);
    void clacEllipsePlotAverage(int, f32, f32);
    bool dpdMove();
    u8 openExplain(u8);

    virtual void draw() { _draw(); }
    virtual ~dMenu_Ring_c();

    void drawFlag0() { mDrawFlag = 0; }
    void setStatus(u8 i_status) { mStatus = i_status; }

private:
    /* 0x004 */ JKRExpHeap* mpHeap;
    /* 0x008 */ STControl* mpStick;
    /* 0x00C */ CSTControl* mpCStick;
    /* 0x010 */ dSelect_cursor_c* mpDrawCursor;
    /* 0x014 */ dMsgString_c* mpString;
    /* 0x018 */ CPaneMgrAlpha* mpMessageParent;
    /* 0x01C */ CPaneMgrAlpha* mpCenterParent;
    /* 0x020 */ CPaneMgrAlpha* mpSpotParent;
    /* 0x024 */ CPaneMgr* mpTextParent[10];
    /* 0x04C */ CPaneMgr* mpNameParent;
    /* 0x050 */ CPaneMgr* mpCircle;
    /* 0x054 */ dKantera_icon_c* mpKanteraMeter;
    /* 0x058 */ J2DScreen* mpScreen;
    /* 0x05C */ J2DScreen* mpSpotScreen;
    /* 0x060 */ u8 field_0x060[0x064 - 0x060];
    /* 0x064 */ J2DScreen* mpCenterScreen;
    /* 0x068 */ J2DPicture* mpSelectItemTex[4][3];
    /* 0x098 */ J2DPicture* mpItemTex[MAX_ITEM_SLOTS][3];
    /* 0x1B8 */ J2DPicture* mpBlackTex;
    /* 0x1BC */ J2DPicture* mpItemNumTex[3];      // Displays digits on ammo-based items. Each index
                                                  // represents one drawn digit
    /* 0x1C8 */ J2DTextBox* mpComboOffString[5];  // Displays "Combo Off" combined bow is equipped
                                                  // and cursor is on combinable item
    /* 0x1DC */ J2DTextBox* mpBowArrowComboString[5];  // Displays "Bow & Arrow Combo" if bow is
                                                       // equipped and cursor is on combinable item
    /* 0x1F0 */ ResTIMG* mpSelectItemTexBuf[4][3][2];
    /* 0x250 */ ResTIMG* mpItemBuf[MAX_ITEM_SLOTS][3];
    /* 0x370 */ dMenu_ItemExplain_c* mpItemExplain;
    /* 0x374 */ void*
        mpResData[3];  // Contains screen data from .blo files which control e.g. the
                       // appearance of specific sprites and where they are on the screen
    /* 0x380 */ cXyz mDirectSelectCursorPos;  // Positions the cursor with it's X and Z coordinates
                                              // when Direct Select is active
    /* 0x38C */ f32 mItemSlotPosX[MAX_ITEM_SLOTS];
    /* 0x3EC */ f32 mItemSlotPosY[MAX_ITEM_SLOTS];
    /* 0x44C */ f32 mItemSlotParam1[MAX_ITEM_SLOTS];
    /* 0x4AC */ f32 mItemSlotParam2[MAX_ITEM_SLOTS];
    /* 0x50C */ f32 mRingRadiusH;
    /* 0x510 */ f32 mRingRadiusV;
    /* 0x514 */ f32 mAlphaRate;  // Responsible for multiple things but especially for the
                                 // background becoming darker in a specific velocity when opening
    /* 0x518 */ f32 field_0x518[4];
    /* 0x528 */ f32 field_0x528[4];
    /* 0x538 */ f32 field_0x538[4];
    /* 0x548 */ f32 field_0x548[4];
    /* 0x558 */ f32 field_0x558[4];
    /* 0x568 */ f32 mCenterPosX;  // Controls the position in the x-axis of the whole item wheel
                                  // and moves it when opening or closing.
    /* 0x56C */ f32 mCenterPosY;  // Controls the position in the y-axis of the whole item wheel
                                  // and moves it when opening or closing.
    /* 0x570 */ f32 mPikariFlashingSpeed;  // Controls the flashing speed of the combine icon (Bow &
                                           // Arrow Combo)
    /* 0x574 */ f32 field_0x574[3];
    /* 0x580 */ f32 field_0x580[3];
    /* 0x58C */ f32 mRingItemNamePosX;
    /* 0x590 */ f32 mRingItemNamePosY;
    /* 0x594 */ f32 mRingItemNameScale;
    /* 0x598 */ f32 mRingGuidePosX[10];
    /* 0x5C0 */ f32 mRingGuidePosY[10];
    /* 0x5E8 */ f32 mRingGuideScale[10];
    /* 0x610 */ f32 mRingCursorScale;
    /* 0x614 */ f32 mRingPosX;
    /* 0x618 */ f32 mRingPosY;
    /* 0x61C */ f32 mRingScaleH;
    /* 0x620 */ f32 mRingScaleV;
    /* 0x624 */ f32 mRingAlpha;   // the alpha depends on if the player is a wolf or a human
    /* 0x628 */ u32 field_0x628;  // unused, not even initialized
    /* 0x62C */ u32 mPlayerIsWolf;
    /* 0x630 */ u32 mNameStringID;
    /* 0x634 */ s32 field_0x634;
    /* 0x638 */ s16 mOpenCloseFrames;
    /* 0x63A */ s16 field_0x63a;  // just gets set to zero in some functions. Unused
    /* 0x63C */ s16 field_0x63c;  // unused
    /* 0x63E */ s16 field_0x63e[MAX_ITEM_SLOTS];
    /* 0x66E */ s16 field_0x66e;
    /* 0x670 */ s16 field_0x670;
    /* 0x672 */ s16 mWaitFrames;
    /* 0x674 */ s16 field_0x674[4];
    /* 0x67C */ u16 field_0x67c;
    /* 0x67E */ s16 field_0x67e;
    /* 0x680 */ s16 mCursorSpeed;
    /* 0x682 */ s16 field_0x682;
    /* 0x684 */ s16 field_0x684;
    /* 0x686 */ s16 field_0x686[4];
    /* 0x68E */ s16 field_0x68e;
    /* 0x690 */ u8 mItemSlots[MAX_ITEM_SLOTS];
    /* 0x6A8 */ u8 mCurrentSlot;
    /* 0x6A9 */ u8 field_0x6a9;  // unused
    /* 0x6AA */ u8 mXButtonSlot;
    /* 0x6AB */ u8 mYButtonSlot;
    /* 0x6AC */ u8 field_0x6ac;
    /* 0x6AD */ u8 field_0x6ad;
    /* 0x6AE */ u8 mItemsTotal;  // Contains the amount of items which are actually obtained and in
                                 // the item wheel
    /* 0x6AF */ u8 mTotalItemTexToAlloc;  // Similiar to mItemsTotal but used to allocate and setup
                                          // the item textures based on obtained items
    /* 0x6B0 */ u8 mStatus;
    /* 0x6B1 */ u8 mOldStatus;
    /* 0x6B2 */ u8 field_0x6b2;  // used to set status but always zero
    /* 0x6B3 */ u8 field_0x6b3;  // ends up being useless since checkCombineComb and setCombineBomb
                                 // are empty functions at least in GCN
    /* 0x6B4 */ u8 field_0x6b4[4];
    /* 0x6B8 */ u8 field_0x6b8[4];
    /* 0x6BC */ u8 mRingOrigin;  // The value decides the origin the item wheel comes from before
                                 // it's in the center of the screen
    /* 0x6BD */ u8 mDrawFlag;
    /* 0x6BE */ u8 field_0x6be[4];
    /* 0x6C2 */ u8 field_0x6c2;
    /* 0x6C3 */ u8 field_0x6c3;  // unused
    /* 0x6C4 */ u8 field_0x6c4;  // only gets set to current slot when item explain gets opened
    /* 0x6C5 */ u8 field_0x6c5;  // unused
    /* 0x6C6 */ u8 field_0x6c6;  // unused
    /* 0x6C7 */ u8 field_0x6c7[4];
    /* 0x6CB */ u8 field_0x6cb;
    /* 0x6CC */ u8 mDoStatus;
    /* 0x6CD */ u8 field_0x6cd;
    /* 0x6CE */ bool mDirectSelectActive;
    /* 0x6CF */ u8 field_0x6cf;
    /* 0x6D0 */ u8 field_0x6d0;
    /* 0x6D1 */ u8 field_0x6d1;
    /* 0x6D2 */ u8 field_0x6d2;
    /* 0x6D3 */ u8 field_0x6d3;
};

#endif /* D_MENU_D_MENU_RING_H */
