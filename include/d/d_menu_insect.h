#ifndef D_MENU_D_MENU_INSECT_H
#define D_MENU_D_MENU_INSECT_H

#include "d/d_drawlist.h"
#include "d/d_save.h"

class CPaneMgr;
struct CSTControl;
class J2DTextBox;
class STControl;
class dMsgScrn3Select_c;
class dMsgString_c;
class dSelect_cursor_c;
class mDoDvdThd_mountArchive_c;
class JKRArchive;

class dMenu_Insect_c : public dDlst_base_c {
public:
    dMenu_Insect_c(JKRExpHeap*, STControl*, CSTControl*, u8);
    void _create();
    void _move();
    void _draw();
    bool isSync();
    void init();
    int _open();
    int _close();
    void wait_init();
    void wait_move();
    void explain_open_init();
    void explain_open_move();
    void explain_move_init();
    void explain_move_move();
    void select_move_init();
    void select_move_move();
    void explain_close_init();
    void explain_close_move();
    void screenSetBase();
    void screenSetExplain();
    void screenSetDoIcon();
    static u8 getGetInsectNum();
    u8 getInsectItemID(int, int);
    bool isGetInsect(int, int);
    bool isGiveInsect(int, int);
    static bool isGiveInsect(u8);
    static bool isCatchInsect(u8);
    static bool isCatchNotGiveInsect(u8);
    void cursorMove();
    void setCursorPos();
    u8 dpdMove();
    void setAButtonString(u16);
    void setBButtonString(u16);
    void setHIO(bool);

    virtual void draw() { _draw(); }
    virtual ~dMenu_Insect_c();

    u8 getStatus() { return mStatus; }

private:
    /* 0x04 */ JKRExpHeap* mpHeap;
    /* 0x08 */ JKRArchive* mpArchive;
    /* 0x0C */ STControl* mpStick;
    /* 0x10 */ CSTControl* mpCStick;
    /* 0x14 */ mDoDvdThd_mountArchive_c* mpMount;
    /* 0x18 */ dSelect_cursor_c* mpDrawCursor;
    /* 0x1C */ dMsgScrn3Select_c* mpSelect_c;
    /* 0x20 */ J2DScreen* mpScreen;
    /* 0x24 */ J2DScreen* mpExpScreen;
    /* 0x28 */ J2DScreen* mpIconScreen;
    /* 0x2C */ J2DTextBox* mpAButtonString[5];
    /* 0x40 */ J2DTextBox* mpBButtonString[5];
    /* 0x54 */ J2DPicture* field_0x54[2];
    /* 0x5C */ J2DTextBox* field_0x5c;
    /* 0x60 */ J2DPicture* mpBlackTex;
    /* 0x64 */ CPaneMgr* mpParent;
    /* 0x68 */ CPaneMgr* mpExpParent;
    /* 0x6C */ CPaneMgr* mpINSParent[MAX_INSECT_NUM];
    /* 0xCC */ CPaneMgr* mpInfoText;
    /* 0xD0 */ CPaneMgr* mpExpSubWin[2];
    /* 0xD8 */ CPaneMgr* mpButtonAB[2];
    /* 0xE0 */ CPaneMgr* mpButtonText[2];
    /* 0xE8 */ dMsgString_c* mpString;
    /* 0xEC */ ResTIMG* mpExpItemTex;
    /* 0xF0 */ s16 field_0xf0;
    /* 0xF2 */ u8 mStatus;
    /* 0xF3 */ u8 field_0xf3;
    /* 0xF4 */ u8 field_0xf4;
    /* 0xF5 */ u8 field_0xf5;
    /* 0xF6 */ u8 field_0xf6;
    /* 0xF7 */ u8 field_0xf7;
    /* 0xF8 */ u8 field_0xf8;
    /* 0xF9 */ u8 field_0xf9;
    /* 0xFA */ u8 field_0xfa;
    /* 0xFB */ u8 field_0xfb;
    /* 0xFC */ u8 field_0xfc;
};

#endif /* D_MENU_D_MENU_INSECT_H */
