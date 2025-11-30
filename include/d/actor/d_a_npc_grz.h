#ifndef D_A_NPC_GRZ_H
#define D_A_NPC_GRZ_H

#include "d/actor/d_a_npc4.h"
#include "d/d_particle_copoly.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Grz_c
 * @brief Darbus
 *
 * @details
 *
*/

struct daNpc_Grz_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ s16 color_time;              // カラータイム - Color Time    
    /* 0x6E */ s16 color_r;                 // カラーR - Color R         
    /* 0x70 */ s16 color_g;                 // カラーG - Color G         
    /* 0x72 */ s16 color_b;                 // カラーB - Color B         
    /* 0x74 */ f32 walk_speed;              // 歩き速度 - Walk Speed        
    /* 0x78 */ f32 rotation_movement_speed; // 回転移動速度 - Rotational Movement Speed    
    /* 0x7C */ f32 demo_start_distance;     // デモ開始距離 - Demo Start Distance    
};

class daNpc_Grz_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_Grz_HIOParam param;
};

class daNpc_Grz_c : public daNpcF_c {
public:
    typedef int (daNpc_Grz_c::*actionFunc)(void*);
    typedef int (daNpc_Grz_c::*cutFunc)(int);

    class daNpc_GrZ_prtclMngr_c {
    public:
        /* 0x00 */ u8 field_0x0;
        /* 0x04 */ cXyz mPos;
        /* 0x10 */ csXyz mAngle;
        /* 0x18 */ cXyz mScale;
        /* 0x24 */ dPaPo_c mPaPo;
    };

    daNpc_Grz_c();
    ~daNpc_Grz_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setParam();
    BOOL main();
    BOOL ctrlBtk();
    void setAttnPos();
    void setCollisionPunch();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setExpression(int, f32);
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    int drawDbgInfo();
    void drawOtherMdls();
    int getTypeFromParam();
    BOOL isDelete();
    void reset();
    void resetCol();
    void playExpression();
    void playMotion();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    bool setSkipZev(int, int);
    void setLookMode(int);
    void lookat();
    void setExpressionTalkAfter();
    int wait(void*);
    int waitSmash(void*);
    int lieDown(void*);
    int waitWeak(void*);
    int talk(void*);
    int test(void*);
    int setPrtcl();
    int doWaitCut(int);
    int doRebirthCut(int);
    int doTalkStandCut(int);
    int doGoOutCut(int);
    int doStoneSmashCut(int);
    int doStoneSmashSkipCut(int);
    void adjustShapeAngle() {}

    u8 getPathNoFromParam() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }

    static char* mEvtCutNameList[7];
    static cutFunc mEvtCutList[7];

private:
    /* 0x0B48 */ Z2Creature mSound;
    /* 0x0BD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0x0BDC */ daNpcF_Lookat_c mLookat;
    /* 0x0C78 */ daNpcF_Path_c mPath;
    /* 0x12A8 */ daNpcF_ActorMngr_c mActorMngrs[4];
    /* 0x12C8 */ daNpc_Grz_HIO_c* mHIO;
    /* 0x12CC */ dCcD_Cyl mCyl1;
    /* 0x1408 */ dCcD_Cyl mCyl2;
    /* 0x1544 */ dCcD_Sph mSphs[4];
    /* 0x1A24 */ actionFunc mNextAction;
    /* 0x1A30 */ actionFunc mAction;
    /* 0x1A3C */ request_of_phase_process_class mPhases[5];
    /* 0x1A64 */ int field_0x1a64;
    /* 0x1A68 */ int field_0x1a68;
    /* 0x1A6C */ int field_0x1a6c;
    /* 0x1A70 */ int field_0x1a70;
    /* 0x1A74 */ int mMsgNo;
    /* 0x1A78 */ s16 mLookMode;
    /* 0x1A7A */ u16 mMode;
    /* 0x1A7C */ u8 mType;
    /* 0x1A80 */ int mAnm;
    /* 0x1A84 */ int field_0x1a84;
    /* 0x1A88 */ int field_0x1a88;
    /* 0x1A8C */ int mTimer;
    /* 0x1A90 */ J3DGXColorS10 mColor;
    /* 0x1A98 */ u8 mSwNo;
    /* 0x1A9C */ int field_0x1a9c;
    /* 0x1AA0 */ u32 field_0x1aa0[3];
    /* 0x1AAC */ daNpc_GrZ_prtclMngr_c mPrtclMngr[1];
};

STATIC_ASSERT(sizeof(daNpc_Grz_c) == 0x1b08);

class daNpc_Grz_Param_c {
public:
    virtual ~daNpc_Grz_Param_c() {}

    static daNpc_Grz_HIOParam const m;
};

#endif /* D_A_NPC_GRZ_H */
