#ifndef D_FILE_D_FILE_SELECT_H
#define D_FILE_D_FILE_SELECT_H

#include "d/d_com_inf_game.h"
#include "d/d_file_sel_warning.h"
#include "d/d_msg_string.h"
#include "d/d_name.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"

class dFile_info_c;

class dDlst_FileSel_c : public dDlst_base_c {
public:
    void draw();
    virtual ~dDlst_FileSel_c() {
        delete Scr;
        delete mpMessageString;
    }

    dDlst_FileSel_c() {
        mpMessageFont[0] = mDoExt_getMesgFont();
        mpMessageFont[1] = mDoExt_getSubFont();
        mpMessageString = new dMsgString_c();
    }

    /* 0x04 */ J2DScreen* Scr;
    /* 0x08 */ JUTFont* mpMessageFont[2];
    /* 0x0C */ dMsgString_c* mpMessageString;
};

class dFile_select3D_c {
public:
    dFile_select3D_c();
    virtual ~dFile_select3D_c();
    void _create(u8, u8);
    void _delete();
    void freeHeap();
    void _move();
    void draw();
    void setJ3D(char const*, char const*, char const*);
    void set_mtx();
    void animePlay();
    void animeEntry();
    void createMaskModel();
    void createMirrorModel();
    void toItem3Dpos(f32, f32, f32, cXyz*);
    void calcViewMtx(Mtx);

    void drawOff() { mpModel = 0; }
    void setBasePane(CPaneMgr* paneMgr) { mPaneMgr = paneMgr; }
    void setBase2Pane(J2DPane* pane) { mPane = pane; }

    /* 0x0004 */ JKRSolidHeap* mpHeap;
    /* 0x0008 */ J3DModel* mpModel;
    /* 0x000C */ mDoExt_bckAnm* mBckAnm;
    /* 0x0010 */ mDoExt_brkAnm* mBrkAnm;
    /* 0x0014 */ dKy_tevstr_c mTevstr;
    /* 0x039C */ CPaneMgr* mPaneMgr;
    /* 0x03A0 */ J2DPane* mPane;
    /* 0x03A4 */ cXyz field_0x03a4;
    /* 0x03B0 */ csXyz field_0x03b0;
    /* 0x03B6 */ u8 padding[2];
    /* 0x03B8 */ cXyz field_0x03b8;
    /* 0x03C4 */ float field_0x03c4;
    /* 0x03C8 */ float field_0x03c8;
    /* 0x03CC */ u8 field_0x03CC[0x03CE - 0x03CC];
    /* 0x03CE */ u8 mMirrorIdx;
    /* 0x03CF */ u8 mMaskIdx;
};

class dDlst_FileSelYn_c : public dDlst_base_c {
public:
    void draw();
    virtual ~dDlst_FileSelYn_c() { delete ScrYn; }

    /* 0x04 */ J2DScreen* ScrYn;
    /* 0x08 */ u8 field_0x08[4];
};

class dDlst_FileSelDt_c : public dDlst_base_c {
public:
    void draw();
    virtual ~dDlst_FileSelDt_c() { delete ScrDt; }

    /* 0x04 */ J2DScreen* ScrDt;
    /* 0x08 */ J2DPane* mpPane;
    /* 0x0C */ J2DPane* mpPane2;
};

class dDlst_FileSelCp_c : public dDlst_base_c {
public:
    void draw();
    virtual ~dDlst_FileSelCp_c() { delete Scr; }

    /* 0x04 */ J2DScreen* Scr;
    /* 0x08 */ bool field_0x08;
    /* 0x0C */ J2DPane* mpPane1;
    /* 0x10 */ J2DPane* mpPane2;
};

class dDlst_FileSel3m_c : public dDlst_base_c {
public:
    void draw();
    virtual ~dDlst_FileSel3m_c() { delete Scr3m; }

    /* 0x04 */ J2DScreen* Scr3m;
};

class dFs_HIO_c {
public:
    dFs_HIO_c();
    virtual ~dFs_HIO_c() {}

    /* 0x0000 */ // void* vtable;
    /* 0x0004 */ s8 field_0x0004;
    /* 0x0005 */ u8 field_0x0005;
    /* 0x0006 */ u8 field_0x0006;
    /* 0x0007 */ u8 field_0x0007;
    /* 0x0008 */ u8 field_0x0008;
    /* 0x0009 */ u8 field_0x0009;
    /* 0x000A */ u8 field_0x000a;
    /* 0x000B */ u8 field_0x000b;
    /* 0x000C */ u8 field_0x000c;
    /* 0x000D */ u8 field_0x000d;
    /* 0x000E */ u8 field_0x000e[2];
    /* 0x0010 */ float field_0x0010[3];
#if VERSION == VERSION_GCN_PAL
    u8 temp_padding[0x1D];
#endif
    /* 0x001C */ u8 field_0x001c;
    /* 0x001D */ u8 field_0x001d;
    /* 0x001E */ u8 field_0x001e;
    /* 0x001F */ u8 field_0x001f;
    /* 0x0020 */ u8 field_0x0020;
    /* 0x0021 */ u8 field_0x0021;
    /* 0x0022 */ u8 field_0x0022;
    /* 0x0023 */ u8 field_0x0023;
};

// Trying to define this using dSv_save_c causes alignment issues
typedef char SaveData[0xa94];

class dFile_select_c {
public:
    typedef void (dFile_select_c::*DispFunc)();

    dFile_select_c(JKRArchive*);
    virtual ~dFile_select_c();
    void _create();
    void _move();
    void selFileWakuAnm();
    void bookIconAnm();
    void selCopyFileWakuAnm();
    void copyBookIconAnm();
    void dataDelEffAnm();
    void dataCopyEffAnm();
    void selectDataBaseMoveAnmInitSet(int, int);
    bool selectDataBaseMoveAnm();
    void dataSelectInAnmSet();
    void dataSelectIn();
    void dataSelectInit();
    void dataSelect();
    void dataSelectStart();
    void selectDataMoveAnmInitSet(int, int);
    bool selectDataMoveAnm();
    void dataSelectAnmSet();
    void dataSelectMoveAnime();
    void makeRecInfo(u8);
    void selectDataOpenMove();
    void selectDataNameMove();
    void selectDataOpenEraseMove();
    void menuSelect();
    void menuSelectStart();
    void menuSelectCansel();
    void menuMoveAnmInitSet(int, int);
    bool menuMoveAnm();
    void menuSelectAnmSet();
    void menuSelectMoveAnm();
    void ToNameMove();
    void ToNameMove2();
    void nameInputWait();
    void nameInput();
    void nameToDataSelectMove();
    void nameInputFade();
    void nameInput2Move();
    void nameInput2();
    void backNameInputMove0();
    void backNameInputMove();
    void ToCopyPaneMove();
    void ToErasePaneMove();
    void backSelectMove();
    void copySelMoveAnmInitSet(int, int);
    void setSaveDataForCopySel();
    void copyDataToSelect();
    void copyDataToSelectStart();
    void copyDataToSelectCansel();
    void copyDataToSelectMoveAnmSet();
    void copyDataToSelectMoveAnm();
    void copySelectWakuAlpahAnmInit(u8, u8, u8, u8);
    bool copySelectWakuAlpahAnm(u8);
    u8 getCptoNum(u8);
    void copyToSelBack();
    void copyToSelPaneMove();
    bool yesnoMenuMoveAnmInitSet(int, int);
    bool yesnoMenuMoveAnm();
    bool yesnoSelectMoveAnm();
    void yesnoCursorShow();
    void YesNoSelect();
    void yesNoSelectStart();
    void yesnoSelectAnmSet();
    void yesnoCancelAnmSet();
    void YesNoCancelMove();
    void yesNoCursorMoveAnm();
    void CmdExecPaneMove0();
    void CommandExec();
    void DataEraseWait();
    void DataEraseWait2();
    void ErasePaneMoveOk();
    void ErasePaneMoveOk2();
    void eraseEndBackSelectWait();
    void eraseEndBackSelect();
    void DataCopyWait();
    void DataCopyWait2();
    void copyPaneMoveOk();
    void copyPaneMoveOk2();
    void ErrorMsgPaneMove();
    void backDatSelPaneMove();
    void backDatSelWait();
    void backDatSelWait2();
    void nextModeWait();
    void screenSet();
    void screenSetCopySel();
    void screenSetYesNo();
    void screenSet3Menu();
    void screenSetDetail();
    void setWakuAnm();
    void displayInit();
    void setSaveData();
    void headerTxtSet(u16, u8, u8);
    bool headerTxtChangeAnm();
    void modoruTxtChange(u8);
    void modoruTxtDispAnmInit(u8);
    bool modoruTxtDispAnm();
    void ketteiTxtDispAnmInit(u8);
    bool ketteiTxtDispAnm();
    void selectWakuAlpahAnmInit(u8, u8, u8, u8);
    bool selectWakuAlpahAnm(u8);
    void selFileCursorShow();
    void menuWakuAlpahAnmInit(u8, u8, u8, u8);
    bool menuWakuAlpahAnm(u8);
    void menuCursorShow();
    void yesnoWakuAlpahAnmInit(u8, u8, u8, u8);
    bool yesnoWakuAlpahAnm(u8);
    void _draw();
    void errorMoveAnmInitSet(int, int);
    bool errorMoveAnm();
    void errDispInitSet(int, int);
    void MemCardCheckMain();
    void MemCardStatCheck();
    void MemCardLoadWait();
    void MemCardErrMsgWaitKey();
    void noFileSpaceDispInit();
    void MemCardNoFileSpaceDisp();
    void iplSelDispInit();
    void MemCardGotoIPLSelectDisp();
    void MemCardGotoIPLSelect();
    void MemCardGotoIPL();
    void noSaveSelDispInit();
    void MemCardNoSaveSelDisp();
    void MemCardErrMsgWaitNoSaveSel();
    void formatYesSelDispInitSet();
    void formatNoSelDispInitSet();
    void MemCardFormatYesSelDisp();
    void MemCardFormatNoSelDisp();
    void MemCardErrMsgWaitFormatSel();
    void formatYesSel2DispInitSet();
    void MemCardErrMsgWaitFormatSel2();
    void MemCardFormatYesSel2Disp();
    void MemCardFormat();
    void MemCardFormatWait();
    void MemCardFormatCheck();
    void MemCardMakeGameFileSel();
    void MemCardMakeGameFileSelDisp();
    void MemCardMakeGameFile();
    void MemCardMakeGameFileWait();
    void MemCardMakeGameFileCheck();
    void MemCardMsgWindowInitOpen();
    void MemCardMsgWindowOpen();
    void MemCardMsgWindowClose();
    bool errYesNoSelect();
    void errCurMove(u8);
    void MemCardErrYesNoCursorMoveAnm();
    void errorTxtSet(u16);
    bool errorTxtChangeAnm();
    bool fileRecScaleAnm();
    void fileRecScaleAnmInitSet2(f32, f32);
    bool fileRecScaleAnm2();
    bool fileInfoScaleAnm();
    void nameMoveAnmInitSet(int, int);
    bool nameMoveAnm();
    void MemCardSaveDataClear();
    void setInitSaveData();
    void dataSave();

    bool getFadeFlag() { return mFadeFlag; }
    int isDataNew(u8 i) { return mDataNew[i]; }
    int isSelectEnd() { return mSelectEnd; }
    u8 getSelectNum() { return mSelectNum; }
    void setUseType(u8 type) { mUseType = type; }

    /* 0x0004 */ u8 field_0x04[4];
    /* 0x0008 */ JKRArchive* mpArchive;
    /* 0x000C */ dFile_select3D_c* mpFileSelect3d;
    /* 0x0010 */ dDlst_FileSel_c fileSel;
    /* 0x0024 */ dDlst_FileSelDt_c mSelDt;
    /* 0x0034 */ dDlst_FileSelCp_c mCpSel;
    /* 0x0048 */ dDlst_FileSelYn_c mYnSel;
    /* 0x0054 */ dDlst_FileSel3m_c m3mSel;
    /* 0x005C */ u8 field_0x005c[4];
    /* 0x0060 */ dSelect_cursor_c* mSelIcon;
    /* 0x0064 */ dSelect_cursor_c* mSelIcon2;
    /* 0x0068 */ dName_c* mpName;
    /* 0x006C */ dFile_warning_c* mpFileWarning;
    /* 0x0070 */ dFile_info_c* field_0x0070[3];
    /* 0x007C */ dFile_info_c* mpFileInfo[2];
    /* 0x0084 */ J2DAnmTransform* field_0x0084;
    /* 0x0088 */ J2DAnmTransform* field_0x0088;
    /* 0x008C */ J2DAnmTransform* field_0x008c;
    /* 0x0090 */ J2DAnmTransform* field_0x0090;
    /* 0x0094 */ J2DAnmTransform* field_0x0094;
    /* 0x0098 */ J2DAnmTransform* field_0x0098;
    /* 0x009C */ J2DAnmTransform* field_0x009c;
    /* 0x00A0 */ s32 field_0x00a0;
    /* 0x00A4 */ CPaneMgr* field_0x00a4;
    /* 0x00A8 */ u8 field_0x00a8[0xb0-0xa8];
    /* 0x00B0 */ s32 field_0x00b0;
    /* 0x00B4 */ s32 field_0x00b4;
    /* 0x00B8 */ u8 field_0x00b8;
    /* 0x00B9 */ u8 field_0x00b9;
    /* 0x00BA */ u8 field_0x00ba;
    /* 0x00BB */ u8 field_0x00bb;
    /* 0x00BC */ CPaneMgr* field_0x00bc[3];
    /* 0x00C8 */ f32 field_0x00c8[3];
    /* 0x00D4 */ f32 field_0x00d4[3];
    /* 0x00E0 */ s32 field_0x00e0[3];
    /* 0x00EC */ s32 field_0x00ec;
    /* 0x00F0 */ CPaneMgr* field_0x00f0[2];
    /* 0x00F8 */ s32 field_0x00f8[2];
    /* 0x0100 */ s32 field_0x0100;
    /* 0x0104 */ s32 field_0x0104;
    /* 0x0108 */ bool field_0x0108;
    /* 0x0109 */ bool field_0x0109;
    /* 0x010A */ u8 field_0x10a[2];
    /* 0x010C */ J2DPane* mpPane;
    /* 0x0110 */ int field_0x0110;
    /* 0x0114 */ int field_0x0114;
    /* 0x0118 */ J2DPane* field_0x0118;
    /* 0x011C */ J2DPane* field_0x011c;
    /* 0x0120 */ int field_0x0120;
    /* 0x0124 */ int field_0x0124;
    /* 0x0128 */ bool field_0x0128;
    /* 0x0129 */ u8 field_0x0129[0x012C - 0x0129];
    /* 0x012C */ J2DPane* field_0x012c;
    /* 0x0130 */ int field_0x0130;
    /* 0x0134 */ int field_0x0134;
    /* 0x0138 */ CPaneMgrAlpha* field_0x0138[2];
    /* 0x0140 */ char* field_0x0140[2];
    /* 0x0148 */ u8 field_0x0148;
    /* 0x0149 */ u8 field_0x0149;
    /* 0x014A */ bool field_0x014a;
    /* 0x014B */ bool field_0x014b;
    /* 0x014C */ CPaneMgrAlpha* field_0x014c[3];
    /* 0x0158 */ CPaneMgrAlpha* field_0x0158[3];
    /* 0x0164 */ CPaneMgr* field_0x0164[3];
    /* 0x0170 */ CPaneMgr* field_0x0170[3];
    /* 0x017C */ CPaneMgr* field_0x017c[3];
    /* 0x0188 */ CPaneMgr* field_0x0188[3];
    /* 0x0194 */ u8 field_0x0194[3];
    /* 0x0197 */ u8 field_0x0197[3];
    /* 0x019A */ u8 field_0x019a[3];
    /* 0x019D */ u8 field_0x019d[3];
    /* 0x01A0 */ CPaneMgrAlpha* field_0x01a0[2];
    /* 0x01A8 */ CPaneMgrAlpha* field_0x01a8[2];
    /* 0x01B0 */ CPaneMgrAlpha* field_0x01b0[2];
    /* 0x01B8 */ u8 field_0x01b8[2];
    /* 0x01BA */ u8 field_0x01ba[2];
    /* 0x01BC */ u8 field_0x01bc[2];
    /* 0x01BE */ u8 field_0x01be[2];
    /* 0x01C0 */ CPaneMgr* field_0x01c0[2];
    /* 0x01C8 */ J2DAnmColor* field_0x01c8;
    /* 0x01CC */ s32 field_0x01cc;
    /* 0x01D0 */ J2DAnmTextureSRTKey* field_0x01d0;
    /* 0x01D4 */ s32 field_0x01d4;
    /* 0x01D8 */ J2DAnmColor* field_0x01d8;
    /* 0x01DC */ s32 field_0x01dc;
    /* 0x01E0 */ J2DAnmTextureSRTKey* field_0x01e0;
    /* 0x01E4 */ s32 field_0x01e4;
    /* 0x01E8 */ J2DAnmTevRegKey* field_0x01e8;
    /* 0x01EC */ s32 field_0x01ec;
    /* 0x01F0 */ J2DAnmTextureSRTKey* field_0x01f0;
    /* 0x01F4 */ s32 field_0x01f4;
    /* 0x01F8 */ J2DAnmTextureSRTKey* field_0x01f8;
    /* 0x01FC */ s32 field_0x01fc;
    /* 0x0200 */ J2DAnmTevRegKey* field_0x0200;
    /* 0x0204 */ s32 field_0x0204;
    /* 0x0208 */ u8 field_0x0208;
    /* 0x0209 */ u8 field_0x0209;
    /* 0x020A */ u8 field_0x020a;
    /* 0x020B */ u8 field_0x020b;
    /* 0x020C */ CPaneMgrAlpha* field_0x020c[2];
    /* 0x0214 */ char* field_0x0214[2];
    /* 0x021C */ u8 field_0x021c;
    /* 0x021D */ u8 field_0x021d;
    /* 0x021E */ u8 field_0x021e;
    /* 0x021F */ u8 field_0x021f;
    /* 0x0220 */ CPaneMgrAlpha* field_0x0220[3];
    /* 0x022C */ CPaneMgrAlpha* field_0x022c[3];
    /* 0x0238 */ CPaneMgrAlpha* field_0x0238;
    /* 0x023C */ CPaneMgrAlpha* field_0x023c;
    /* 0x0240 */ CPaneMgrAlpha* field_0x0240;
    /* 0x0244 */ CPaneMgrAlpha* field_0x0244;
    /* 0x024A */ bool field_0x0248;
    /* 0x024A */ bool field_0x0249;
    /* 0x024A */ u8 field_0x024a;
    /* 0x024B */ u8 field_0x024b;
    /* 0x024C */ u8 field_0x024c;
    /* 0x024B */ u8 field_0x024d[3];
    /* 0x0250 */ char* field_0x0250;
    /* 0x0254 */ STControl* mStick;
    /* 0x0258 */ u8 mDataNew[3];
    /* 0x025B */ u8 field_0x025b[3];
    /* 0x025C */ u8 field_0x025e[0x0264 - 0x025e];
    /* 0x0264 */ u8 mLastSelectNum; // previously selected quest log
    /* 0x0265 */ u8 mSelectNum; // currently selected quest log
    /* 0x0266 */ u8 mLastSelectMenuNum; // previously select menu number (for copy / start / delete)
    /* 0x0267 */ u8 mSelectMenuNum; // currently selected menu number  (for copy / start / delete)
    /* 0x0268 */ u8 field_0x0268;
    /* 0x0269 */ u8 field_0x0269;
    /* 0x026A */ u8 field_0x026a;
    /* 0x026B */ u8 field_0x026b;
    /* 0x026C */ u8 field_0x026c;
    /* 0x026D */ u8 field_0x026d;
    /* 0x026E */ u8 field_0x026e;
    /* 0x026F */ u8 field_0x026f;
    /* 0x0270 */ bool mSelectEnd;
    /* 0x0271 */ u8 field_0x0271;
    /* 0x0272 */ u8 field_0x0272;
    /* 0x0273 */ u8 field_0x0273;
    /* 0x0274 */ u8 field_0x0274;
    /* 0x0272 */ u8 field_0x0275[0x0280 - 0x0275];
    /* 0x0281 */ bool field_0x0280;
    /* 0x0281 */ bool field_0x0281;
    /* 0x0282 */ bool field_0x0282;
    /* 0x0283 */ bool field_0x0283;
    /* 0x0284 */ DispFunc field_0x0284;
    /* 0x0290 */ DispFunc field_0x0290;
    /* 0x029C */ J2DAnmTransform* field_0x029c;
    /* 0x02A0 */ J2DAnmTransform* field_0x02a0;
    /* 0x02A4 */ CPaneMgr* field_0x02a4[3];
    /* 0x02B0 */ u8 field_0x02b0[0x2b4-0x2b0];
    /* 0x02B4 */ int field_0x02b4[3];
    /* 0x02C0 */ CPaneMgr* field_0x02c0[2];
    /* 0x02C8 */ CPaneMgr* field_0x02c8[2];
    /* 0x02D0 */ CPaneMgr* field_0x02d0[2];
    /* 0x02D8 */ CPaneMgr* field_0x02d8[2];
    /* 0x02E0 */ u8 field_0x02e0[2];
    /* 0x02E2 */ u8 field_0x02e2[2];
    /* 0x02E4 */ u8 field_0x02e4[2];
    /* 0x02E4 */ u8 field_0x02e6[0x2e8-0x2e6];
    /* 0x02E8 */ J2DAnmColor* field_0x02e8;
    /* 0x02EC */ s32 field_0x02ec;
    /* 0x02F0 */ J2DAnmTextureSRTKey* field_0x02f0;
    /* 0x02F4 */ s32 field_0x02f4;
    /* 0x02F8 */ J2DAnmColor* field_0x02f8;
    /* 0x02FC */ s32 field_0x02fc;
    /* 0x0300 */ J2DAnmTextureSRTKey* field_0x0300;
    /* 0x0304 */ s32 field_0x0304;
    /* 0x0308 */ J2DAnmTevRegKey* field_0x0308;
    /* 0x030C */ s32 field_0x030c;
    /* 0x0310 */ J2DAnmTransform* field_0x0310;
    /* 0x0314 */ J2DAnmTransform* field_0x0314;
    /* 0x0318 */ J2DAnmColor* field_0x0318;
    /* 0x031C */ s32 field_0x031c;
    /* 0x0320 */ J2DAnmTextureSRTKey* field_0x0320;
    /* 0x0324 */ s32 field_0x0324;
    /* 0x0328 */ J2DAnmTransform* field_0x0328;
    /* 0x032C */ J2DAnmTransform* field_0x032c;
    /* 0x0330 */ J2DAnmColor* field_0x0330;
    /* 0x0334 */ s32 field_0x0334;
    /* 0x0338 */ J2DAnmTextureSRTKey* field_0x0338;
    /* 0x033C */ s32 field_0x033c;
    /* 0x0340 */ CPaneMgr* field_0x0340[3];
    /* 0x034C */ s32 field_0x034c[3];
    /* 0x0358 */ int field_0x0358;
    /* 0x035C */ int field_0x035c;
    /* 0x0360 */ bool field_0x0360;
    /* 0x0361 */ u8 field_0x0361[3];
    /* 0x0364 */ CPaneMgr* mpPaneAlpha1[3];
    /* 0x0370 */ CPaneMgr* mpPaneAlpha2[3];
    /* 0x037C */ CPaneMgr* mpPaneAlpha3[3];
    /* 0x0388 */ u8 field_0x0388[3];
    /* 0x038B */ u8 field_0x038b[3];
    /* 0x038E */ u8 field_0x038e[3];
    /* 0x0390 */ u8 field_0x0391[3];
    /* 0x0394 */ CPaneMgr* mpPaneMgr2[3];
    /* 0x03A0 */ J2DAnmTextureSRTKey* field_0x03a0;
    /* 0x03A4 */ s32 field_0x03a4;
    /* 0x03A8 */ CPaneMgr* field_0x03a8;
    /* 0x03AC */ s16 field_0x03ac;
    /* 0x03AE */ u8 field_0x03AE[0x03B0 - 0x03AE];
    /* 0x03B0 */ u8 mUseType;
    /* 0x03B1 */ u8 field_0x03b1;
    /* 0x03B2 */ u16 field_0x03b2;
    /* 0x03B4 */ s32 field_0x03b4;
    /* 0x03B8 */ SaveData mSave[3];
    /* 0x0D10 */ //u8 field_0x0D10[0x2374 - 0xD10];
    /* 0x2374 */ bool mFadeFlag;
    /* 0x2375 */ bool mHasDrawn;
    /* 0x2376 */ u8 field_0x2376[0x2378 - 0x2376];
    /* 0x2378 */ J2DPicture* field_0x2378;
};

STATIC_ASSERT(sizeof(dFile_select_c) == 0x237C);

#endif /* D_FILE_D_FILE_SELECT_H */
