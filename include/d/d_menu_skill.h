#ifndef D_MENU_D_MENU_SKILL_H
#define D_MENU_D_MENU_SKILL_H

#include "d/d_drawlist.h"

class CPaneMgr;
struct CSTControl;
class J2DTextBox;
class STControl;
class mDoDvdThd_mountArchive_c;
class dMsgString_c;
class dSelect_cursor_c;

class dMenu_Skill_c : public dDlst_base_c {
public:
    enum {
        /* 0 */ PROC_CLOSE_MOVE,
        /* 1 */ PROC_WAIT_MOVE,
        /* 2 */ PROC_OPEN_MOVE,
        /* 3 */ PROC_MOVE_MOVE,
    };

    dMenu_Skill_c(JKRExpHeap*, STControl*, CSTControl*);
    void _create();
    void _move();
    void _draw();
    bool isSync();
    void skill_init_calc();
    void init();
    int _open();
    int _close();
    void wait_init();
    void wait_move();
    void read_open_init();
    void read_open_move();
    void read_move_init();
    void read_move_move();
    void read_close_init();
    void read_close_move();
    void screenSetMenu();
    void screenSetLetter();
    void screenSetDoIcon();
    void setCursorPos();
    void changeActiveColor();
    void setPageText();
    void setAButtonString(u16);
    void setBButtonString(u16);
    void setNameString(u16);
    u8 getSkillNum();
    void setHIO(bool);

    virtual void draw() { _draw(); }
    virtual ~dMenu_Skill_c();

    u8 getStatus() { return mStatus; }

private:
    /* 0x004 */ JKRExpHeap* mpHeap;
    /* 0x008 */ JKRArchive* mpArchive;
    /* 0x00C */ STControl* mpStick;
    /* 0x010 */ CSTControl* mpCStick;
    /* 0x014 */ mDoDvdThd_mountArchive_c* mpMount;
    /* 0x018 */ J2DScreen* mpMenuScreen;
    /* 0x01C */ J2DScreen* mpLetterScreen;
    /* 0x020 */ J2DScreen* mpIconScreen;
    /* 0x024 */ J2DPicture* mpTagPicture[7][4];
    /* 0x094 */ J2DPicture* field_0x94[7];
    /* 0x0B0 */ J2DTextBox* mpFTagPicture[7][4];
    /* 0x120 */ J2DTextBox* mpAButtonString[5];
    /* 0x134 */ J2DTextBox* mpBButtonString[5];
    /* 0x148 */ J2DTextBox* mpNameString[4];
    /* 0x158 */ J2DPicture* mpBlackTex;
    /* 0x15C */ CPaneMgr* mpParent;
    /* 0x160 */ CPaneMgr* mpLetterParent[7];
    /* 0x17C */ CPaneMgr* mpTextParent;
    /* 0x180 */ CPaneMgr* mpTextPane;
    /* 0x184 */ CPaneMgr* mpExpName;
    /* 0x188 */ CPaneMgr* mpButtonAB[2];
    /* 0x190 */ CPaneMgr* mpButtonText[2];
    /* 0x198 */ dSelect_cursor_c* mpDrawCursor;
    /* 0x19C */ dMsgString_c* mpString;
    /* 0x1A0 */ JUtility::TColor mSelectBlack[5];
    /* 0x1B4 */ JUtility::TColor mUnselectBlack[5];
    /* 0x1C8 */ JUtility::TColor mSelectWhite[5];
    /* 0x1DC */ JUtility::TColor mUnselectWhite[5];
    /* 0x1F0 */ f32 mPosX;
    /* 0x1F4 */ f32 mBarScale[2];
    /* 0x1FC */ u32 mStringID;
    /* 0x200 */ s16 mFrame;
    /* 0x202 */ s16 mProcFrame;
    /* 0x204 */ u8 mStatus;
    /* 0x205 */ u8 mProcess;
    /* 0x206 */ u8 mIndex;
    /* 0x207 */ u8 mSkillNum;
    /* 0x208 */ u8 mRemainder;
    /* 0x209 */ u8 field_0x209; // Initialized but never used
    /* 0x20A */ u8 field_0x20a; // Initialized but never used
    /* 0x20B */ u8 mTotalSkillNum;
};  // Size: 0x20C

#endif /* D_MENU_D_MENU_SKILL_H */
