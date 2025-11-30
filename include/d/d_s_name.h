#ifndef D_S_D_S_NAME_H
#define D_S_D_S_NAME_H

#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_scene.h"

class dSn_HIO_c {
public:
    dSn_HIO_c();
    virtual ~dSn_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x5 */ u8 mFileSelWaitTime;
};

class dScnName_camera_c : public camera_process_class {
public:
    dScnName_camera_c() { field_0x22f = 84; }
    virtual ~dScnName_camera_c() {}
};

// remove later and use header instead
class dFile_select_c {
public:
    dFile_select_c(JKRArchive*);
    virtual ~dFile_select_c();
    void _create();
    void _move();
    void _draw();

    bool getFadeFlag() { return mFadeFlag; }
    int isDataNew(u8 i) { return mDataNew[i]; }
    int isSelectEnd() { return mSelectEnd; }
    u8 getSelectNum() { return mSelectNum; }
    void setUseType(u8 type) { mUseType = type; }

private:
    u8 field_0x4[0x254];
    /* 0x0258 */ u8 mDataNew[3];
    /* 0x025B */ u8 field_0x25b[0x265 - 0x25b];
    /* 0x0265 */ u8 mSelectNum;
    /* 0x0266 */ u8 field_0x266[0x270 - 0x266];
    /* 0x0270 */ bool mSelectEnd;
    /* 0x0271 */ u8 field_0x271[0x3b0 - 0x271];
    /* 0x03B0 */ u8 mUseType;
    /* 0x03B1 */ u8 field_0x3b1[0x2374 - 0x3b1];
    /* 0x2374 */ bool mFadeFlag;
    /* 0x2375 */ u8 field_0x2375[0x237c - 0x2375];
};

// remove later and use header instead
class dBrightCheck_c {
public:
    dBrightCheck_c(JKRArchive*);
    virtual ~dBrightCheck_c();
    void _move();
    void _draw();

    bool isEnd() { return mEnd; }

    u8 field_0x4[0x15];
    /* 0x19 */ bool mEnd;
};

class dScnName_c : public scene_class {
public:
    dScnName_c() {}

    s32 create();
    void setView();
    s32 execute();
    s32 draw();
    ~dScnName_c();
    void FileSelectOpen();
    void FileSelectMain();
    void FileSelectMainNormal();
    void FileSelectClose();
    void brightCheckOpen();
    void brightCheck();
    void changeGameScene();

    #if VERSION == VERSION_GCN_PAL
    void bmg_data_set();
    void tex_data_set();
    #endif

private:
    #if VERSION == VERSION_GCN_PAL
    u8 field_0x1c4_pal[0x28];
    #endif
    /* 0x1C4 */ request_of_phase_process_class field_0x1c4;
    /* 0x1CC */ JKRExpHeap* mHeap;
    /* 0x1D0 */ JKRExpHeap* field_0x1d0;
    /* 0x1D4 */ dScnName_camera_c mCamera;
    /* 0x414 */ dFile_select_c* dFs_c;
    /* 0x418 */ dBrightCheck_c* mBrightCheck;
    /* 0x41C */ u8 field_0x41c;
    /* 0x41D */ u8 field_0x41d;
    /* 0x41E */ u8 field_0x41e;
    /* 0x41F */ u8 field_0x41f;
    /* 0x420 */ u8 field_0x420;
};

#endif /* D_S_D_S_NAME_H */
