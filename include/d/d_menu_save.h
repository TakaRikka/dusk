#ifndef D_MENU_D_MENU_SAVE_H
#define D_MENU_D_MENU_SAVE_H

#include "d/d_drawlist.h"
#include "d/d_save.h"

class CPaneMgr;
class CPaneMgrAlpha;
struct CSTControl;
class J2DAnmColorKey;
class J2DAnmTransformKey;
class JUTFont;
class STControl;
class dMsgString_c;
class dFile_warning_c;
class dFile_info_c;
class dMsgScrnExplain_c;
class dSelect_cursor_c;
class mDoDvdThd_mountArchive_c;

class dDlst_MenuSaveExplain_c : public dDlst_base_c {
public:
    dDlst_MenuSaveExplain_c() { mpScrn = NULL; }

    virtual void draw();
    virtual ~dDlst_MenuSaveExplain_c() {}

    void setScrnExplain(dMsgScrnExplain_c* p_scrn) { mpScrn = p_scrn; }

private:
    /* 0x4 */ dMsgScrnExplain_c* mpScrn;
};

class dDlst_MenuSave_c : public dDlst_base_c {
public:
    dDlst_MenuSave_c() {
        Scr = NULL;
        mMsgString = NULL;
    }

    virtual void draw();
    virtual ~dDlst_MenuSave_c() {}

    /* 0x04 */ J2DScreen* Scr;
    /* 0x08 */ JUTFont* font[2];
    /* 0x10 */ dMsgString_c* mMsgString;
};

class dMs_HIO_c {
public:
    dMs_HIO_c();
    virtual ~dMs_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x5 */ u8 mDisplayWaitFrames;
    /* 0x6 */ u8 mCardWaitFrames;
    /* 0x7 */ u8 mEffectDispFrames;
    /* 0x8 */ u8 mCharSwitchFrames;
    /* 0x9 */ u8 mSelectIcon;
    /* 0xA */ u8 mSelectFrames;
    /* 0xB */ u8 mFadeOutTimer;
    /* 0xC */ bool mTitleMsgCheck;
    /* 0xD */ u8 field_0xd;
    /* 0xE */ bool mErrorMsgCheck;
    /* 0xF */ u8 field_0xf;
};

class dMenu_save_c {
public:
    enum {
        TYPE_DEFAULT = 1,
        TYPE_WHITE_EVENT = 3,
        TYPE_BLACK_EVENT
    };

    enum {
        CURSOR_NO,
        CURSOR_YES,
    };

    enum {
        /* 0x00 */ PROC_SAVE_QUESTION,
        /* 0x01 */ PROC_SAVE_QUESTION2,
        /* 0x02 */ PROC_SAVE_QUESTION21,
        /* 0x03 */ PROC_SAVE_QUESTION3,
        /* 0x04 */ PROC_SAVE_QUESTION4,
        /* 0x05 */ PROC_SAVE_GUIDE,
        /* 0x06 */ PROC_MEMCARD_CHECK,
        /* 0x07 */ PROC_BACK_SAVE_QUESTION,
        /* 0x08 */ PROC_BACK_SAVE_QUESTION2,
        /* 0x09 */ PROC_MEMCARD_ERRMSG_WAIT_KEY,
        /* 0x0A */ PROC_IPL_SELECT_DISP1,
        /* 0x0B */ PROC_IPL_SELECT_DISP2,
        /* 0x0C */ PROC_MEMCARD_ERR_GO_IPL_SEL,
        /* 0x0D */ PROC_IPL_SELECT2_DISP,
        /* 0x0E */ PROC_MEMCARD_ERR_GO_IPL_SEL2,
        /* 0x0F */ PROC_MEMCARD_ERR_GOTO_IPL,
        /* 0x10 */ PROC_MEMCARD_ERRMSG_WAIT_FORMAT_SEL,
        /* 0x11 */ PROC_CARD_FORMAT_YES_SEL_DISP,
        /* 0x12 */ PROC_CARD_FORMAT_NO_SEL_DISP,
        /* 0x13 */ PROC_MEMCARD_ERRMSG_WAIT_FORMAT_SEL2,
        /* 0x14 */ PROC_CARD_FORMAT_YES_SEL2_DISP,
        /* 0x15 */ PROC_MEMCARD_FORMAT,
        /* 0x16 */ PROC_MEMCARD_FORMAT_WAIT,
        /* 0x17 */ PROC_MAKE_GAME_FILE_SEL_DISP,
        /* 0x18 */ PROC_MEMCARD_MAKE_GAME_FILE_SEL,
        /* 0x19 */ PROC_MAKE_GAME_FILE_DISP,
        /* 0x1A */ PROC_MEMCARD_MAKE_GAME_FILE,
        /* 0x1B */ PROC_MEMCARD_MAKE_GAME_FILE_WAIT,
        /* 0x1C */ PROC_MEMCARD_COMMAND_END,
        /* 0x1D */ PROC_MEMCARD_COMMAND_END2,
        /* 0x1E */ PROC_MEMCARD_DATA_LOAD_WAIT,
        /* 0x1F */ PROC_MEMCARD_DATA_SAVE_WAIT,
        /* 0x20 */ PROC_MEMCARD_DATA_SAVE_WAIT2,
        /* 0x21 */ PROC_GAME_CONTINUE_DISP,
        /* 0x22 */ PROC_GAME_CONTINUE,
        /* 0x23 */ PROC_GAME_CONTINUE2,
        /* 0x24 */ PROC_GAME_CONTINUE3,
        /* 0x25 */ PROC_SAVE_END,
        /* 0x26 */ PROC_ENDING_NO_SAVE,
        /* 0x27 */ PROC_ENDING_NO_SAVE2,
        /* 0x28 */ PROC_ENDING_DATA_CHECK,
        /* 0x29 */ PROC_SAVE_WAIT,
        /* 0x2A */ PROC_MESSAGE_CHANGE,
        /* 0x2B */ PROC_RETRY_QUESTION0,
        /* 0x2C */ PROC_RETRY_QUESTION1,
        /* 0x2D */ PROC_RETRY_QUESTION2,
        /* 0x2E */ PROC_OPEN_SAVE_SELECT,
        /* 0x2F */ PROC_OPEN_SAVE_SELECT2,
        /* 0x30 */ PROC_OPEN_SAVE_SELECT3,
        /* 0x31 */ PROC_SAVE_SELECT,
        /* 0x32 */ PROC_SAVE_SELECT_MOVE_ANM,
        /* 0x33 */ PROC_SELECT_DATA_OPEN_MOVE,
        /* 0x34 */ PROC_SAVE_YES_NO_SELECT,
        /* 0x35 */ PROC_YES_NO_CURSOR_MOVE_ANM,
        /* 0x36 */ PROC_SAVE_YES_NO_CANCEL_MOVE,
        /* 0x37 */ PROC_SAVE_MOVE_DISP,
        /* 0x38 */ PROC_SAVE_MOVE_DISP2,
        /* 0x39 */ PROC_MSG_WINDOW_INIT_OPEN,
        /* 0x3A */ PROC_MSG_WINDOW_OPEN,
        /* 0x3B */ PROC_MSG_WINDOW_CLOSE,
        /* 0x3C */ PROC_ERR_YES_NO_CURSOR_MOVE_ANM,
    };

    dMenu_save_c();
    void _create();
    void screenSet();
    void initialize();
    void displayInit();
    bool _open();
    int _close();
    void _delete();
    void _move();
    void saveSelAnm();
    void selFileWakuAnm();
    void bookIconAnm();
    void memCardWatch();
    void saveQuestion();
    void saveQuestion2();
    void saveQuestion21();
    void saveQuestion3();
    void saveQuestion4();
    void saveGuide();
    void memCardCheck();
    void memCardErrMsgWaitKey();
    void backSaveQuestionInitSet();
    void backSaveQuestion();
    void backSaveQuestion2();
    void closeSelect();
    void closeSelect2();
    void iplSelMsgInitSet();
    void iplSelInitSet();
    void IPLSelectDisp1();
    void IPLSelectDisp2();
    void memCardErrGoIPLSel();
    void IPLSelect2Disp();
    void memCardErrGoIPLSel2();
    void memCardErrGotoIPL();
    void memCardErrMsgWaitFormatSel();
    void cardFormatYesSelDisp();
    void cardFormatNoSelDisp();
    void memCardErrMsgWaitFormatSel2();
    void cardFormatYesSel2Disp();
    void memCardFormat();
    void memCardFormatWait();
    void gameFileMakeSelInitSet();
    void makeGameFileSelDisp();
    void memCardMakeGameFileSel();
    void makeGameFileDisp();
    void memCardMakeGameFile();
    void memCardMakeGameFileWait();
    void memCardCommandEnd();
    void memCardCommandEnd2();
    void memCardDataLoadWait();
    void dataWrite();
    void memCardDataSaveWait();
    void memCardDataSaveWait2();
    void gameContinueDisp();
    void gameContinue();
    void gameContinue2();
    void gameContinue3();
    void saveEnd();
    void endingNoSave();
    void endingNoSave2();
    void endingDataCheck();
    void retryQuestion0();
    void retryQuestion1();
    void retryQuestion2();
    void restartInit();
    void saveWait();
    void messageChange();
    int YesNoSelect();
    void msgTxtSet(u16, bool);
    void openSaveSelect();
    void openSaveSelect2();
    void openSaveSelect3();
    void saveSelect();
    void saveSelectStart();
    void selectDataOpenMove();
    void dataSelectAnmSet();
    void saveSelectMoveAnime();
    void saveYesNoSelect();
    void yesnoSelectStart();
    void saveMoveDisp();
    void saveMoveDisp2();
    void yesnoSelectAnmSet(u8);
    void yesNoCursorMoveAnm();
    void yesnoCancelAnmSet();
    void saveYesNoCancelMove();
    void headerTxtSet(u16);
    bool headerTxtChangeAnm();
    void errDispInitSet(int);
    void msgWindowInitOpen();
    void msgWindowOpen();
    void msgWindowClose();
    bool errYesNoSelect(u8, u8);
    void errCurMove(u8, u8);
    void errYesNoCursorMoveAnm();
    void errorTxtSet(u16);
    bool errorTxtChangeAnm();
    void saveSelectOpenInit();
    void selectDataBaseMoveAnmInitSet(int, int);
    bool selectDataBaseMoveAnm();
    void saveSelectOpenAnmSet();
    void selectDataMoveAnmInitSet(int, int);
    bool selectDataMoveAnm();
    void yesnoMenuMoveAnmInitSet(int anmFrame, int frameMax, u8 param_2);
    bool yesnoMenuMoveAnm();
    bool yesnoSelectMoveAnm(u8);
    void yesnoCursorShow();
    void errorMoveAnmInitSet(int, int);
    bool errorMoveAnm();
    void modoruTxtDispAnmInit(u8);
    bool modoruTxtDispAnm();
    void ketteiTxtDispAnmInit(u8);
    bool ketteiTxtDispAnm();
    void selectWakuAlpahAnmInit(u8, u8, u8, u8);
    bool selectWakuAlpahAnm(u8);
    void selFileCursorShow();
    void yesnoWakuAlpahAnmInit(u8, u8, u8, u8);
    bool yesnoWakuAlpahAnm(u8);
    void dataSave();
    void setSaveData();
    void setInitSaveData();
    void _draw();
    void _draw2();

    virtual ~dMenu_save_c() {}

    u8 getSaveStatus() { return mSaveStatus; }
    u8 getEndStatus() { return mEndStatus; }
    void setUseType(u8 type) { mUseType = type; }

private:
    /* 0x0004 */ JKRArchive* mpArchive;
    /* 0x0008 */ mDoDvdThd_mountArchive_c* mpMount;
    /* 0x000C */ STControl* stick;
    /* 0x0010 */ dDlst_MenuSaveExplain_c mMenuSaveExplain;
    /* 0x0018 */ dDlst_MenuSave_c mSaveSel;
    /* 0x002C */ dSelect_cursor_c* mSelIcon;
    /* 0x0030 */ dFile_warning_c* mWarning;
    /* 0x0034 */ dFile_info_c* mFileInfo[3];
    /* 0x0040 */ J2DAnmTransformKey* field_0x40;
    /* 0x0044 */ J2DAnmTransformKey* field_0x44;
    /* 0x0048 */ J2DAnmTransformKey* field_0x48;
    /* 0x004C */ J2DAnmTransformKey* field_0x4c;
    /* 0x0050 */ s32 field_0x50;
    /* 0x0054 */ u8 mSelectedFile;
    /* 0x0055 */ u8 mLastSelFile;
    /* 0x0058 */ CPaneMgr* mpSelectMoveBase;
    /* 0x005C */ int mDataBaseMoveAnmFrame;
    /* 0x0060 */ int mDataBaseMoveFrameMax;
    /* 0x0064 */ u8 field_0x64;
    /* 0x0065 */ u8 field_0x65;
    /* 0x0068 */ CPaneMgr* mpSelData[3];
    /* 0x0074 */ int field_0x74[3];
    /* 0x0080 */ int field_0x80;
    /* 0x0084 */ CPaneMgr* mpNoYes[2];
    /* 0x008C */ int field_0x8c[2];
    /* 0x0094 */ int mYesNoMoveAnmFrame;
    /* 0x0098 */ int mYesNoMoveAnmMax;
    /* 0x009C */ u8 field_0x9c;
    /* 0x009D */ u8 field_0x9d;
    /* 0x009E */ u8 field_0x9e;
    /* 0x00A0 */ J2DPane* field_0xa0;
    /* 0x00A4 */ u8 field_0xa4[0x10];
    /* 0x00B4 */ J2DPane* field_0xb4;
    /* 0x00B8 */ int field_0xb8;
    /* 0x00BC */ int field_0xbc;
    /* 0x00C0 */ CPaneMgrAlpha* mpErrTxtPane[2];
    /* 0x00C8 */ char* mpErrTxt[2];
    /* 0x00D0 */ u8 mErrTxtType;
    /* 0x00D1 */ u8 mErrTxtAnmComplete;
    /* 0x00D2 */ u8 field_0xd2;
    /* 0x00D3 */ u8 field_0xd3;
    /* 0x00D4 */ CPaneMgr* mpSelWakuMoyo[3];
    /* 0x00E0 */ CPaneMgr* mpSelWakuGold[3];
    /* 0x00EC */ CPaneMgr* mpSelWakuGold2[3];
    /* 0x00F8 */ CPaneMgr* mpBookWaku[3];
    /* 0x0104 */ u8 field_0x104[3];
    /* 0x0107 */ u8 field_0x107[3];
    /* 0x010A */ u8 field_0x10a[3];
    /* 0x010D */ u8 field_0x10d[11];
    /* 0x0118 */ CPaneMgr* mNoYesBase[2];
    /* 0x0120 */ CPaneMgr* mNoYesGold[2];
    /* 0x0128 */ CPaneMgr* mNoYesGold2[2];
    /* 0x0130 */ u8 mNoYesWakuStartAlpha[2];
    /* 0x0132 */ u8 mNoYesWakuEndAlpha[2];
    /* 0x0134 */ u8 mNoYesWakuAnmTimer[2];
    /* 0x0138 */ CPaneMgr* mpNoYesTxt[2];
    /* 0x0140 */ J2DAnmColor* mpFileWakuAnm;
    /* 0x0144 */ int mFileWakuAnmFrame;
    /* 0x0148 */ J2DAnmTextureSRTKey* mpFileWakuRotAnm;
    /* 0x014C */ int mFileWakuRotAnmFrame;
    /* 0x0150 */ J2DAnmColor* field_0x150;
    /* 0x0154 */ int field_0x154;
    /* 0x0158 */ J2DAnmTextureSRTKey* field_0x158;
    /* 0x015C */ int field_0x15c;
    /* 0x0160 */ J2DAnmTevRegKey* field_0x160;
    /* 0x0164 */ int field_0x164;
    /* 0x0168 */ CPaneMgrAlpha* mpHeaderTxtPane[2];
    /* 0x0170 */ char* mpHeaderTxt[2];
    /* 0x0178 */ u8 mHeaderTxtType;  // 0: Select Menu  1: YesNo Menu
    /* 0x0179 */ u8 mHeaderAnmComplete;
    /* 0x017A */ u8 field_0x17a;
    /* 0x017B */ u8 field_0x17b;
    /* 0x017C */ CPaneMgrAlpha* mpDataBase[3];
    /* 0x0188 */ CPaneMgrAlpha* mpNoDataBase[3];
    /* 0x0194 */ CPaneMgrAlpha* mpBBtnIcon;
    /* 0x0198 */ CPaneMgrAlpha* mpABtnIcon;
    /* 0x019C */ u8 field_0x19c;
    /* 0x019D */ u8 field_0x19d;
    /* 0x01A0 */ CPaneMgrAlpha* mpBackTxt;
    /* 0x01A4 */ CPaneMgrAlpha* mpConfirmTxt;
    /* 0x01A8 */ u8 field_0x1a8;
    /* 0x01A9 */ u8 field_0x1a9;
    /* 0x01AA */ u8 field_0x1aa[3];
    /* 0x01AD */ u8 field_0x1ad[3];
    /* 0x01B0 */ u8 mDisplayMenu;
    /* 0x01B1 */ u8 field_0x1b1;
    /* 0x01B2 */ u8 mMenuProc;
    /* 0x01B3 */ u8 field_0x1b3;
    /* 0x01B4 */ u8 field_0x1b4;
    /* 0x01B5 */ u8 mErrProc;
    /* 0x01B6 */ u8 mYesNoCursor;
    /* 0x01B7 */ u8 mYesNoPrevCursor;
    /* 0x01B8 */ u8 field_0x1b8;
    /* 0x01B9 */ u8 field_0x1b9;
    /* 0x01BA */ u8 mSaveStatus;
    /* 0x01BB */ u8 mEndStatus;
    /* 0x01BC */ u8 mUseType;
    /* 0x01BD */ u8 mWaitTimer;
    /* 0x01BE */ u8 field_0x1be;
    /* 0x01BF */ u8 field_0x1bf;
    /* 0x01C0 */ u8 field_0x1c0;
    /* 0x01CC */ void (dMenu_save_c::*mpErrFunc)();
    /* 0x01D0 */ u8 mSaveBuffer[QUEST_LOG_SIZE * 3];
    /* 0x218C */ dMsgScrnExplain_c* mpScrnExplain;
    /* 0x2190 */ u8 field_0x2190;
    /* 0x2192 */ u16 field_0x2192;
    /* 0x2194 */ u8 field_0x2194;
    /* 0x2195 */ u8 field_0x2195;
    /* 0x2198 */ int mCmdState;
    /* 0x219C */ bool field_0x219c;
    /* 0x219D */ u8 field_0x219d;
    /* 0x219E */ u8 field_0x219e;
    /* 0x219F */ u8 field_0x219f;
    /* 0x21A0 */ u8 field_0x21a0;
    /* 0x21A1 */ u8 field_0x21a1;
    /* 0x21A2 */ u8 field_0x21a2;
    /* 0x21A3 */ u8 field_0x21a3;
};

#endif /* D_MENU_D_MENU_SAVE_H */
