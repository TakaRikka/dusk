#ifndef MSG_SCRN_D_MSG_SCRN_3SELECT_H
#define MSG_SCRN_D_MSG_SCRN_3SELECT_H

#include "d/d_drawlist.h"

class J2DScreen;
class J2DAnmTransform;
class J2DAnmColorKey;
class J2DAnmTextureSRTKey;
class dSelect_cursor_c;
class CPaneMgr;
class J2DTextBox;

class dMsgScrn3Select_c {
public:
    enum Process_e {
        PROC_OPEN1_e,
        PROC_OPEN2_e,
        PROC_WAIT_e,
        PROC_SELECT_e,
        PROC_CHANGE_e,
        PROC_CLOSE_e,
        PROC_MAX_e,
    };

    dMsgScrn3Select_c();
    virtual ~dMsgScrn3Select_c();
    bool isSelect();
    void setString(char*, char*, char*);
    void setRubyString(char*, char*, char*);
    void translate(f32, f32);
    void draw(f32, f32);
    void selAnimeInit(u8, u8, u8, f32, u8);
    bool selAnimeMove(u8, u8, bool);
    bool selAnimeEnd();
    f32 getTextBoxWidth();
    f32 getFontSize();
    f32 getRubyFontSize();
    f32 getCharSpace();
    f32 getRubyCharSpace();
    f32 getTextBoxGlobalPosX(int);
    f32 getTextBoxGlobalPosY(int);
    void open1Proc();
    void open2Proc();
    void waitProc();
    void selectProc();
    void changeProc();
    void closeProc();
    void selectScale();
    void selectTrans();
    void selectAnimeTransform(int);

    void setOffsetX(f32 i_offsetX) { mOffsetX = i_offsetX; }
    bool isAnimeUpdate(int param_0) { return (field_0x114 & (u8)(1 << param_0)) ? TRUE : FALSE; }
    void onAnimeUpdate(int param_0) { field_0x114 |= (u8)(1 << param_0); }
    void offAnimeUpdate(int param_0) { field_0x114 &= ~(u8)(1 << param_0); }

private:
    /* 0x004 */ J2DScreen* mpScreen;
    /* 0x008 */ J2DAnmTransform* mpAnmBck;
    /* 0x00C */ J2DAnmColorKey* mpAnmBpk;
    /* 0x010 */ J2DAnmTextureSRTKey* mpAnmBtk;
    /* 0x014 */ dSelect_cursor_c* mpSelectCursor;
    /* 0x018 */ CPaneMgr* mpParent;
    /* 0x01C */ CPaneMgr* mpTmSel_c[3];
    /* 0x028 */ CPaneMgr* mpTmrSel_c[3];
    /* 0x034 */ CPaneMgr* mpSel_c[3];
    /* 0x040 */ CPaneMgr* mpSelCld_c[3];
    /* 0x04C */ CPaneMgr* mpSelCldr_c[3];
    /* 0x058 */ CPaneMgr* mpSelCldm_c[3];
    /* 0x064 */ CPaneMgr* mpSelCldw_c[5][3];
    /* 0x0A0 */ CPaneMgr* mpSelPo_c[3];
    /* 0x0AC */ CPaneMgr* mpKahen_c[3];
    /* 0x0B8 */ CPaneMgr* mpCursor_c[3];
    /* 0x0C4 */ JUtility::TColor mSelMsgCol;
    /* 0x0C8 */ JUtility::TColor mSelMsgInactiveCol;
    /* 0x0CC */ Vec mCursorPos[3];
    /* 0x0F0 */ f32 mBckFrame;
    /* 0x0F4 */ f32 mBpkFrame;
    /* 0x0F8 */ f32 mBtkFrame;
    /* 0x0FC */ f32 mOffsetX;
    /* 0x100 */ f32 mBaseWidth;
    /* 0x104 */ f32 mWidth;
    /* 0x108 */ u8 field_0x108;
    /* 0x10A */ s16 mSelWaitTimer;
    /* 0x10C */ s16 field_0x10c;
    /* 0x10E */ u8 mSelNo;
    /* 0x10F */ u8 mLastSelNo;
    /* 0x110 */ u8 field_0x110;
    /* 0x111 */ u8 mSelNum;
    /* 0x112 */ u8 field_0x112;
    /* 0x113 */ u8 mProcess;
    /* 0x114 */ u8 field_0x114;
    /* 0x115 */ u8 field_0x115;
    /* 0x116 */ u8 field_0x116;
    /* 0x117 */ u8 field_0x117;
};  // Size: 0x118

#endif /* MSG_SCRN_D_MSG_SCRN_3SELECT_H */
