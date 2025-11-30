#ifndef D_FILE_D_FILE_SEL_INFO_H
#define D_FILE_D_FILE_SEL_INFO_H

#include "d/d_drawlist.h"

class dDlst_FileInfo_c : public dDlst_base_c {
public:
    dDlst_FileInfo_c() {}
    virtual void draw();
    virtual ~dDlst_FileInfo_c() {}

    /* 0x04 */ J2DScreen* Scr;
    /* 0x08 */ JUTFont* mFont;
    /* 0x0C */ J2DPane* mBasePane;
    /* 0x10 */ J2DPane* field_0x10;
};

class CPaneMgrAlpha;
class dSv_save_c;

class dFile_info_c {
public:
    dFile_info_c(JKRArchive*, u8);
    void screenSet();
    int setSaveData(dSv_save_c*, int, u8);
    void setHeartCnt(dSv_save_c*);
    void setSaveDate(dSv_save_c*);
    void setPlayTime(dSv_save_c*);
    void modeWait();
    void modeMove();
    void _draw();

    virtual ~dFile_info_c();

    void setBasePane(J2DPane* pane) { mFileInfo.mBasePane = pane; }
    CPaneMgrAlpha* getDatBase() { return mDatBase; }
    CPaneMgrAlpha* getNoDatBase() { return mNoDatBase; }
    void draw() { _draw(); }

private:
    /* 0x04 */ JKRArchive* mArchive;
    /* 0x08 */ dDlst_FileInfo_c mFileInfo;
    /* 0x1C */ u8 field_0x1c[4];
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x24 */ CPaneMgrAlpha* mDatBase;
    /* 0x28 */ CPaneMgrAlpha* mNoDatBase;
    /* 0x2C */ char* mPlayerName;
    /* 0x30 */ char* mSaveDate;
    /* 0x34 */ char* mPlayTime;
    /* 0x38 */ char* mSaveStatus;
};

typedef void (dFile_info_c::*warningFunc)(void);

#endif /* D_FILE_D_FILE_SEL_INFO_H */
