#ifndef D_DEBUG_CAMERA_H
#define D_DEBUG_CAMERA_H

#include "d/d_camera.h"
#include "JSystem/JHostIO/JORFile.h"

enum dDbgCam_KeyAction {
    /* | */ dDbgCam_KeyAction_FIXEDFRM,
    /* ] */ dDbgCam_KeyAction_UNITRANS,
    /* > */ dDbgCam_KeyAction_BSPLINE,
    /* - */ dDbgCam_KeyAction_NOACTION,
};

class dDbgCamera_c;

class dDbgCamKey_c {
public:
    dDbgCamKey_c() {}
    ~dDbgCamKey_c();

    /* 0x00 */ cXyz mCenter;
    /* 0x0C */ cXyz mEye;
    /* 0x18 */ f32 mFovy;
    /* 0x1C */ f32 mBank;
    struct {
        /* 0x20 */ fpc_ProcID id;
        /* 0x24 */ char name[dStage_NAME_LENGTH];
    } mActorInfo;
    /* 0x2C */ int mAction;
    /* 0x30 */ int mTimer;
};

class dDbgCamSetup_c : public JORReflexible {
public:
    dDbgCamSetup_c();
    ~dDbgCamSetup_c() {}

    void Init(dDbgCamera_c*);
    void Finish();
    f32 Translate(f32);
    s16 Rotate(f32);
    f32 FlatMove(f32);
    f32 Turbo(f32);

    void genMessage(JORMContext*);
    void listenPropertyEvent(const JORPropertyEvent*);

    f32 FovyTick() { return mFovyTick; }
    f32 RotAccel() { return mRotAccel; }
    f32 TransAccel() { return mTransAccel; }
    int CenterMarker() { return mCenterMarker; }
    f32 CenterBall() { return mCenterBall; }
    f32 CenterCloss() { return mCenterCross; }

    bool CheckFlag(u16 i_flag) { return (i_flag & mFlag) ? true : false; }

    /* 0x04 */ s32 mActorSrchType;
    /* 0x08 */ f32 mActorSrchRange;
    /* 0x0C */ fpc_ProcID mActorID;
    /* 0x10 */ dDbgCamera_c* mpDbgCam;
    /* 0x14 */ f32 mMoveSpeed;
    /* 0x18 */ f32 mMoveTurbo;
    /* 0x1C */ f32 mMoveParallelShift;
    /* 0x20 */ f32 mRotateSpeed;
    /* 0x24 */ f32 mFovyTick;
    /* 0x28 */ f32 mTransAccel;
    /* 0x2C */ f32 mRotAccel;
    /* 0x30 */ f32 mMoveMax;
    /* 0x34 */ f32 mRotateMax;
    /* 0x38 */ f32 mUnkMax;
    /* 0x3C */ f32 mCenterCross;
    /* 0x40 */ f32 mCenterBall;
    /* 0x44 */ s32 mCenterMarker;
    /* 0x48 */ u16 mFlag;
    /* 0x4C */ cXyz mGazePoint;
    /* 0x58 */ cXyz mPerspective;
    /* 0x64 */ f32 mAngle;
    /* 0x68 */ cXyz field_0x68;
    /* 0x74 */ cXyz field_0x74;
    /* 0x80 */ s8 m_childNo;
    /* 0x84 */ s32 mPosX;
    /* 0x88 */ s32 mPosY;
};

class dDbgCamera_c {
public:
    dDbgCamera_c() {}
    ~dDbgCamera_c() {}

    void InitlChk();
    int SetlChk(dBgS_LinChk);
    
    int moveTool();
    int cmdTool();
    int makeZevData();
    void printZevData();
    void WriteZevData();
    void monitor();
    bool showCenterMarker();
    bool showCameraPos();
    bool showIdlCameraPos();
    void Init(dCamera_c*);
    void Finish();
    void Reset(dCamera_c*);
    int Run();
    int DrawShape();
    int Report(int, int, JUtility::TColor, const char*, ...);
    void OpenFile();
    void PrintFile(char*, ...);
    void CloseFile();

    char* ActionName(s32 i_action) {
        static char* name[] = {
            "FIXEDFRM",
            "UNITRANS",
            "BSPLINE",
            "NOACTION",
        };

        return name[i_action];
    }

    cXyz Center() { return mRunKey.mCenter; }
    void Fovy(f32 fovy) { mRunKey.mFovy = fovy; };
    void Eye(const cXyz& eye) { mRunKey.mEye = eye; };
    void Center(const cXyz& center) { mRunKey.mCenter = center; };
    f32 Fovy() { return mRunKey.mFovy; }
    s16 Bank() { return cSAngle(mRunKey.mBank).Val(); }
    cXyz Up() { return mUp; }
    cXyz Eye() { return mRunKey.mEye; }
    bool Playing() { return mIsPlaying; }

    dDbgCamKey_c PlaneKey() {
        dDbgCamKey_c rt;
        rt.mCenter = mpCamera->Center();
        rt.mEye = mpCamera->Eye();
        rt.mFovy = mpCamera->Fovy();
        rt.mBank =  mpCamera->Bank().Degree();
        rt.mActorInfo.id = fpcM_ERROR_PROCESS_ID_e;
        strcpy(rt.mActorInfo.name, "WORLD");
        rt.mAction = dDbgCam_KeyAction_NOACTION;
        rt.mTimer = 20;
        return rt;
    }

    char ActionSymbol(s32 i_action) { return "|]>-..."[i_action]; }

    /* 0x000 */ dCamera_c* mpCamera;
    /* 0x004 */ dDbgCamKey_c mRunKey;
    /* 0x038 */ cXyz mUp;
    /* 0x044 */ cSGlobe field_0x44;
    /* 0x04C */ int mPadNo;
    /* 0x050 */ int field_0x50;
    /* 0x054 */ u32 field_0x54;
    /* 0x058 */ int field_0x58;
    /* 0x05C */ int mCmdMode;
    /* 0x060 */ char mActionKey[32];
    /* 0x080 */ u8 field_0x80;
    /* 0x084 */ dDbgCamKey_c mKeys[32];
    class {
    public:
        /* 0x000 */ int field_0x0;
        /* 0x004 */ int field_0x4;
        /* 0x008 */ u8 field_0x8[0x010 - 0x008];
        /* 0x010 */ cXyz field_0x10[32];
        /* 0x190 */ cXyz field_0x190[32];
    } /* 0x704 */ field_0x704;
    /* 0xA14 */ u8 field_0xA14[0xB14 - 0xA14];
    /* 0xB14 */ int field_0xb14;
    /* 0xB18 */ bool mIsPlaying;
    /* 0xB1C */ dBgS_LinChk mLchk[2];
    /* 0xBFC */ int mLchkNum;
    /* 0xC00 */ int field_0xc00;
    /* 0xC04 */ int field_0xc04;
    /* 0xC08 */ int field_0xc08;
    /* 0xC0C */ int field_0xc0c;
    /* 0xC10 */ f32 field_0xc10;
    /* 0xC14 */ f32 field_0xc14;
    /* 0xC18 */ int field_0xc18;
    /* 0xC1C */ int field_0xc1c;
    /* 0xC20 */ dDbgCamSetup_c mCamSetup;
    /* 0xCAC */ JORFile mFile;
};

#endif
