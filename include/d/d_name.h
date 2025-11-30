#ifndef D_D_NAME_H
#define D_D_NAME_H

#include "d/d_select_cursor.h"

class CPaneMgr;
class CPaneMgrAlpha;
class J2DAnmColorKey;
class J2DAnmTextureSRTKey;
class J2DTextBox;
class JUTFont;
class STControl;

class dNm_HIO_c {
public:
    dNm_HIO_c();
    virtual ~dNm_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mMenuScale;
    /* 0x0C */ f32 mSelCharScale;
    /* 0x10 */ u8 field_0x10;
};

class dDlst_NameIN_c : public dDlst_base_c {
public:
    dDlst_NameIN_c() {}

    virtual void draw();
    virtual ~dDlst_NameIN_c() {}

    /* 0x04 */ J2DScreen* NameInScr;
    /* 0x08 */ JUTFont* font;
    /* 0x0C */ J2DPane* field_0xc;
    /* 0x10 */ J2DPane* field_0x10;
};

class ChrInfo_c {
public:
    /* 0x0 */ u8 mColumn;
    /* 0x1 */ u8 mRow;
    /* 0x2 */ u8 mMojiSet;
    /* 0x3 */ u8 field_0x3;
    /* 0x4 */ int mCharacter;
};  // Size: 0x8

class dName_c {
public:
    enum {
        PROC_MOJI_SELECT,
        PROC_MOJI_SEL_ANM,
        PROC_MOJI_SEL_ANM2,
        PROC_MOJI_SEL_ANM3,
        PROC_MENU_SELECT,
        PROC_MENU_SEL_ANM,
        PROC_MENU_SEL_ANM2,
        PROC_MENU_SEL_ANM3,
        PROC_WAIT
    };

    enum {
        MOJI_HIRA, // hiragana characters
        MOJI_KATA, // katakana characters
        MOJI_EIGO, // english characters
    };

    enum {
        MENU_HIRA, // hiragana menu
        MENU_KATA, // katakana menu
        MENU_EIGO, // english menu
        MENU_END,
    };

    dName_c(J2DPane*);
    void _create();
    void init();
    void initial();
    void showIcon();
    void _move();
    int nameCheck();
    void playNameSet(int);
    void cursorAnm();
    void Wait();
    void MojiSelect();
    void MojiSelectAnmInit();
    void MojiSelectAnm();
    void MojiSelectAnm2();
    void MojiSelectAnm3();
    int mojiChange(u8);
    void selectMojiSet();
    #if REGION_JPN
    int checkDakuon(int, u8);
    int setDakuon(int, u8);
    #endif
    int getMoji();
    void setMoji(int);
    void setNameText();
    void nameCursorMove();
    void selectCursorMove();
    void menuCursorPosSet();
    void MenuSelect();
    void MenuSelectAnmInit();
    void MenuSelectAnm();
    void MenuSelectAnm2();
    void MenuSelectAnm3();
    void menuAbtnSelect();
    void backSpace();
    void mojiListChange();
    void menuCursorMove();
    void menuCursorMove2();
    void selectCursorPosSet(int);
    void _draw();
    void screenSet();
    void displayInit();
    void NameStrSet();
    s32 getMenuPosIdx(u8);

    virtual ~dName_c();

    u8 getCurPos() { return mCurPos; }
    u8 isInputEnd() { return mIsInputEnd; }
    char* getInputStrPtr() { return mInputStr; }
    void hideIcon() { mSelIcon->setAlphaRate(0.0f); }
    void setNextNameStr(char* i_name) { strcpy(mNextNameStr,i_name); }
    void draw() { _draw(); }

private:
    /* 0x004 */ STControl* stick;
    /* 0x008 */ JKRArchive* archive;
    /* 0x00C */ dDlst_NameIN_c nameIn;
    /* 0x020 */ dSelect_cursor_c* mSelIcon;
    /* 0x024 */ J2DAnmColorKey* mCursorColorKey;
    /* 0x028 */ int mCurColAnmF;
    /* 0x02C */ J2DAnmTextureSRTKey* mCursorTexKey;
    /* 0x030 */ int mCurTexAnmF;
    /* 0x034 */ CPaneMgrAlpha* mNameCursor[8];
    /* 0x054 */ char* mNameText[8];
    /* 0x074 */ CPaneMgr* mMojiIcon[65];
    /* 0x178 */ char* mMojiText[65];
    /* 0x27C */ J2DPane* mMojiPane;
    /* 0x280 */ J2DPane* mMenuPane;
    /* 0x284 */ CPaneMgr* mMenuIcon[4];
    /* 0x294 */ J2DTextBox* mMenuText[4];
    /* 0x2A4 */ u8 mCursorDelay;
    /* 0x2A5 */ u8 mCharColumn;
    /* 0x2A6 */ u8 mPrevColumn;
    /* 0x2A7 */ u8 mCharRow;
    /* 0x2A8 */ u8 mPrevRow;
    /* 0x2A9 */ u8 mMojiSet;
    /* 0x2AA */ u8 mPrevMojiSet;
    /* 0x2AB */ u8 mSelProc;
    /* 0x2AC */ u8 field_0x2ac;
    /* 0x2AD */ u8 field_0x2ad;
    /* 0x2AE */ u8 field_0x2ae;
    /* 0x2AF */ u8 mSelMenu;
    /* 0x2B0 */ u8 mPrevSelMenu;
    /* 0x2B1 */ u8 mCurPos;
    /* 0x2B2 */ u8 mLastCurPos;
    /* 0x2B3 */ u8 field_0x2b3;
    /* 0x2B4 */ u8 mIsInputEnd;
    /* 0x2B5 */ char mInputStr[23];
    /* 0x2CC */ ChrInfo_c mChrInfo[8];
    /* 0x30C */ u8 field_0x30c[4][4];  // ?
    /* 0x31C */ char mNextNameStr[24];
};  // Size: 0x334

#endif /* D_D_NAME_H */
