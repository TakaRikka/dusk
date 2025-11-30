#ifndef D_MENU_D_MENU_ITEM_EXPLAIN_H
#define D_MENU_D_MENU_ITEM_EXPLAIN_H

#include "d/d_drawlist.h"

class CPaneMgr;
class J2DTextBox;
class STControl;
class dKantera_icon_c;
class dMsgString_c;
class dMenu_ItemExplain_c;
class dSelect_cursor_c;
class dMsgScrnArrow_c;
class dMsgScrn3Select_c;
class J2DOrthoGraph;

class dMenu_ItemExplain_c {
public:
    dMenu_ItemExplain_c(JKRExpHeap*, JKRArchive*, STControl*, bool);
    virtual ~dMenu_ItemExplain_c();
    void move();
    void draw(J2DOrthoGraph*);
    void drawKantera();
    void wait_init();
    void wait_proc();
    void open_init();
    void open_proc();
    void move_init();
    void move_proc();
    void move_select_init();
    void move_select_proc();
    void move_next_init();
    void move_next_proc();
    void close_init();
    void close_proc();
    u8 openExplain(u8, u8, u8, bool);
    u8 openExplainDmap(u8, u8, u8, bool, u8);
    u8 openExplainTx(u32, u32);
    f32 getAlphaRatio();
    void setNumber();
    bool getWarpMarkFlag();
    void setScale();

    u8 getStatus() { return mStatus; }
    void offForceButton() { mForceButton = 0; }
    u8 getForceButton() { return mForceButton; }
    u8 checkEndButton() { return mEndButton; }

    void trans(f32 x, f32 y) {
        field_0xd4 = x;
        field_0xd8 = y;
    }

private:
    /* 0x04 */ JKRExpHeap* mpHeap;
    /* 0x08 */ JKRArchive* mpArchive;
    /* 0x0C */ dMsgString_c* mpInfoString;
    /* 0x10 */ CPaneMgr* mpInfoText;
    /* 0x14 */ J2DPane* field_0x14;  // actual data type not known
    /* 0x18 */ CPaneMgr* mpNameText[4];
    /* 0x28 */ J2DPane* field_0x28[4];  // actual data type not known
    /* 0x38 */ CPaneMgr* mpInfoIcon;
    /* 0x3C */ CPaneMgr* mpParent[2];
    /* 0x44 */ J2DPane* field_0x44[2];  // actual data type not known
    /* 0x4C */ CPaneMgr* mpLabel;
    /* 0x50 */ J2DPane* field_0x50;  // actual data type not known
    /* 0x54 */ u8 field_0x54[0x5C - 0x54];
    /* 0x5C */ J2DPane* field_0x5c;     // actual data type not known
    /* 0x60 */ J2DPane* field_0x60[3];  // actual data type not known
    /* 0x6C */ dKantera_icon_c* mpKanteraMeter;
    /* 0x70 */ J2DScreen* mpInfoScreen;
    /* 0x74 */ J2DPane* field_0x74;  // actual data type not known
    /* 0x78 */ int field_0x78;
    /* 0x7C */ J2DPicture* mpExpItemPane[3];
    /* 0x88 */ ResTIMG* mpExpItemTex[4];
    /* 0x98 */ dMsgScrnArrow_c* mpArrow;
    /* 0x9C */ dMsgScrn3Select_c* mpSelect_c;
    /* 0xA0 */ int field_0xa0[2];
    /* 0xA8 */ J2DPicture* mpItemNumTex[3];
    /* 0xB4 */ J2DPicture* mpBackTex;
    /* 0xB8 */ STControl* mpStick;
    /* 0xBC */ f32 mAlphaRatio;
    /* 0xC0 */ f32 mDescAlpha;
    /* 0xC4 */ u32 field_0xc4;
    /* 0xC8 */ u32 field_0xc8;
    /* 0xCC */ u32 field_0xcc;
    /* 0xD0 */ u32 field_0xd0;
    /* 0xD4 */ f32 field_0xd4;
    /* 0xD8 */ f32 field_0xd8;
    /* 0xDC */ u16 field_0xdc;
    /* 0xDE */ u8 field_0xde;
    /* 0xDF */ u8 field_0xdf;
    /* 0xE0 */ u8 mStatus;
    /* 0xE1 */ u8 field_0xe1;
    /* 0xE2 */ u8 field_0xe2;
    /* 0xE3 */ u8 mEndButton;
    /* 0xE4 */ u8 mForceButton;
    /* 0xE5 */ u8 field_0xe5;
    /* 0xE6 */ u8 field_0xe6;
    /* 0xE7 */ u8 field_0xe7;
};

#endif /* D_MENU_D_MENU_ITEM_EXPLAIN_H */
