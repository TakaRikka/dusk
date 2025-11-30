#ifndef D_A_NPC2
#define D_A_NPC2

#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "d/d_path.h"

class daBaseNpc_acMngr_c {
public:
    daBaseNpc_acMngr_c();
    void entry(fopAc_ac_c*);
    fopAc_ac_c* getActor();
    virtual ~daBaseNpc_acMngr_c() {}

private:
    /* 0x04 */ fpc_ProcID mActorId;
};

class daBaseNpc_matAnm_c : public J3DMaterialAnm {
public:
    virtual ~daBaseNpc_matAnm_c() {}
    virtual void calc(J3DMaterial*) const;

    /* 0x0F4 */ mutable f32 field_0xF4;
    /* 0x0F8 */ mutable f32 field_0xF8;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;
    /* 0x104 */ u8 mEyeMoveFlg;
    /* 0x105 */ u8 mMorfFrm;
};

class daBaseNpc_path_c {
public:
    daBaseNpc_path_c();
    BOOL isPath();
    int setPathInfo(u8, s8, s8);
    void reverseDir();
    s32 chkPnt(cXyz);
    BOOL setNextPnt();
    u16 getIdx();
    void setIdx(u16);
    Vec getPntPos(u16);
    void dbgDraw(f32);
    virtual ~daBaseNpc_path_c() {}

    static const int MAXNUMCONTROLPNT_e = 64;

public:
    /* 0x004 */ dPath* mPathInfo;
    /* 0x008 */ f32 field_0x8;
    /* 0x00C */ dPnt mCurvePnts[160];
    /* 0xA0C */ u8 field_0xa0c[0xA0E - 0xA0C];
    /* 0xA0E */ u16 mCurvePntNum;
    /* 0xA10 */ u16 mIdx;
    /* 0xA12 */ s8 mDirection;
    /* 0xA13 */ u8 field_0xa13;
    /* 0xA14 */ s16 field_0xa14;
    /* 0xA16 */ u8 field_0xa16[2];
    /* 0xA18 */ int field_0xa18;
    /* 0xA1C */ u8 field_0xa1c[8];
};

class daBaseNpc_lookat_c {
public:
    void limitter(s16, s16*, s16, s16);
    void calc(fopAc_ac_c*, f32 (*)[4], s16);
    void setMaxJntLmt(csXyz, int);
    void setMinJntLmt(csXyz, int);
    void setJntPos(cXyz, int);
    int setAttnPos(cXyz*);
    csXyz getRot(int);
    virtual ~daBaseNpc_lookat_c() {}

    /* 0x04 */ csXyz mMaxJntLmt[4];
    /* 0x1C */ csXyz mMinJntLmt[4];
    /* 0x34 */ cXyz mJntPos[4];
    /* 0x64 */ csXyz field_0x64[4];
    /* 0x7C */ csXyz mRot[4];
    /* 0x94 */ u8 field_0x94[4];
    /* 0x98 */ cXyz* mpAttnPos;
};

class daBaseNpc_c : public fopAc_ac_c {
public:
    daBaseNpc_c();
    J3DAnmTransform* getTrnsfrmKeyAnmP(char*, int);
    int setMcaMorfAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    int setBckAnm(J3DAnmTransform*, f32, int, int, int, bool);
    J3DAnmTransform* getTexPtrnAnmP(char*, int);
    int setBtpAnm(J3DAnmTexPattern*, J3DModelData*, f32, int);
    void orderEvent(int, char*);
    void setEnvTevColor();
    void setRoomNo();
    int checkEndAnm(f32);
    int checkEndAnm(J3DFrameCtrl*);

    virtual ~daBaseNpc_c();
    virtual int execute();
    virtual int draw(f32);
    virtual const char* getResName();
    virtual void attnSttsOn(int, int);
    virtual void setParam();
    virtual void mainProc();
    virtual BOOL btkCtrl();
    virtual void allAnmPlay();
    virtual void adjustShapeAngle();
    virtual void setMtx(int);
    virtual void setCollisions();
    virtual void setAttnPos();
    virtual void drawOtherMdls();
    virtual bool dbgDraw();

    static dCcD_SrcGObjInf const mCcDObj;
    static dCcD_SrcCyl mCcDCyl;
    static dCcD_SrcSph mCcDSph;

public:
    /* 0x56C */ dBgS_ObjAcch mAcch;
    /* 0x744 */ char field_0x744;
    /* 0x745 */ u8 field_0x745[0x74c - 0x745];
    /* 0x74C */ request_of_phase_process_class mPhase;
    /* 0x754 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x758 */ Z2Creature mSound;
    /* 0x7E8 */ u32 mShadowId;
    /* 0x7EC */ mDoExt_bckAnm mBck;
    /* 0x808 */ mDoExt_btpAnm mBtp;
    /* 0x820 */ mDoExt_btkAnm mBtk;
    /* 0x838 */ s16 field_0x838;
    /* 0x83C */ u16* field_0x83c;
    /* 0x840 */ u16 field_0x840;
    /* 0x842 */ u16 field_0x842;
    /* 0x844 */ u8 field_0x844;
    /* 0x845 */ u8 field_0x845[0x848 - 0x845];
    /* 0x848 */ s32 field_0x848;
    /* 0x84C */ dMsgFlow_c mMsgFlow;
    /* 0x898 */ u8 field_0x898[0x89A - 0x898];
    /* 0x89A */ s16 mEvtIdx;
    /* 0x89C */ dCcD_Stts mCcStts;
    /* 0x8D8 */ dBgS_AcchCir mAcchCir;
    /* 0x918 */ s16 field_0x918;
    /* 0x91A */ csXyz field_0x91a;
    /* 0x920 */ daBaseNpc_lookat_c mLookat;
    /* 0x9BC */ csXyz field_0x9bc;
    /* 0x9C2 */ csXyz field_0x9c2;
    /* 0x9C8 */ csXyz field_0x9c8;
    /* 0x9CE */ csXyz field_0x9ce;
    /* 0x9D4 */ cBgS_GndChk field_0x9d4;
    /* 0xA10 */ f32 mGroundY;
};

STATIC_ASSERT(sizeof(daBaseNpc_c) == 0xA14);

class daBaseNpc_moveBgActor_c : public daBaseNpc_c {
public:
    daBaseNpc_moveBgActor_c();
    int MoveBGCreateHeap();
    int MoveBGCreate(char const*, int, MoveBGActor_SetFunc, u32);
    int MoveBGDelete();
    int MoveBGExecute();
    int MoveBGDraw() {
        int ret = Draw();
        return ret;
    }

    virtual ~daBaseNpc_moveBgActor_c() {}
    virtual int CreateHeap() { return 1; }
    virtual int Create() { return 1; }
    virtual int Execute(Mtx**) { return 1; }
    virtual int Draw() { return 1; }
    virtual int Delete() { return 1; }
    virtual int IsDelete() { return 1; }
    virtual int ToFore() { return 1; }
    virtual int ToBack() { return 1; }

    static const char* m_name;
    static int m_dzb_id;
    static MoveBGActor_SetFunc m_set_func;

public:
    /* 0xA14 */ dBgW* mpBgw;
    /* 0xA18 */ Mtx mBgMtx;
};

#endif /* D_A_NPC2 */
