#ifndef D_A_NPC_TKC_H
#define D_A_NPC_TKC_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcTkc_c
 * @brief Ooccoo Jr.
 *
 * @details
 *
*/

struct daNpcTkc_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 warp_initial_pos;
    /* 0x70 */ f32 down_move_spd;
    /* 0x74 */ f32 up_move_spd;
    /* 0x78 */ f32 ellipse_width;
    /* 0x7C */ f32 div;
    /* 0x80 */ f32 max;
    /* 0x84 */ f32 min;
    /* 0x88 */ f32 conversation_dist;
    /* 0x8C */ f32 target_height;
    /* 0x90 */ f32 flight_range;
    /* 0x94 */ f32 step;
    /* 0x98 */ f32 amplitude;
    /* 0x9C */ s16 interval;
    /* 0x9E */ s16 rotation_interval;
    /* 0xA0 */ s16 x_angle;
    /* 0xA2 */ s16 z_angle;
};

class daNpcTkc_Param_c {
public:
    virtual ~daNpcTkc_Param_c() {}

    static daNpcTkc_HIOParam const m;
};

#if DEBUG
class daNpcTkc_HIO_c : public mDoHIO_entry_c {
public:
    daNpcTkc_HIO_c();
    void genMessage(JORMContext*);

    /* 0x8 */ daNpcTkc_HIOParam m;
};
#define NPC_TKC_HIO_CLASS daNpcTkc_HIO_c
#else
#define NPC_TKC_HIO_CLASS daNpcTkc_Param_c
#endif

class daNpcTkc_c : public daNpcF_c {
public:
    typedef void (daNpcTkc_c::*actionFunc)();
    typedef int (daNpcTkc_c::*evtFunc)(int);

    daNpcTkc_c();
    ~daNpcTkc_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setActionWait();
    void setActionFollow();
    void setMtx();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void reset();
    void setMotionAnm(int, f32);
    BOOL setAction(actionFunc);
    void wait();
    void calcFly();
    BOOL searchPlayer();
    void follow();
    void appear();
    void normalTalk();
    void setMotion(int, f32, int);
    void setExpression(int, f32);
    void warpTalk();
    void demo();
    int EvCut_TksSecretChild(int);
    int EvCut_TksWarpExit(int);
    int EvCut_TksWarpBack(int);
    void setParam();
    BOOL main();
    BOOL ctrlBtk();
    void setAttnPos();
    int drawDbgInfo();

    u8 getType() { return fopAcM_GetParam(this) & 0xFF; }
    u16 getMessageNo() { return fopAcM_GetParam(this) >> 16; }
    void setStart() { mStatus = 1; }
    void callOdorokiSound() { mSound.startCreatureVoiceLevel(Z2SE_TKC_V_ODOROKI_LOOP, -1); }
    void setLookMode(int i_lookMode) { if (i_lookMode >= 0 && i_lookMode < 4 && i_lookMode != mLookMode) mLookMode = i_lookMode; }
    BOOL checkFindPlayer();
    void setExpressionTalkAfter();
    void playExpression();
    void playMotion();
    BOOL chkAction(actionFunc action) { return action == mAction; }
    void lookat();

    static evtFunc mEvtSeqList[4];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xBE0 */ NPC_TKC_HIO_CLASS* mpHIO;
    /* 0xBE4 */ dCcD_Sph mSph;

    /* 0xD20 */ actionFunc mAction;
    /* 0xD2C */ request_of_phase_process_class mPhase;
    /* 0xD34 */ fpc_ProcID mUnkID;
    /* 0xD38 */ cXyz mSpeedTarget;
    /* 0xD44 */ cXyz field_0xd44;
    /* 0xD50 */ cXyz mPrevSpeed;
    /* 0xD5C */ int mTimer;
    /* 0xD60 */ int field_0xd60;
    /* 0xD64 */ f32 mCurrentPosYOffset;
    /* 0xD68 */ f32 field_0xd68;
    /* 0xD6C */ f32 field_0xd6c;
    /* 0xD70 */ s16 field_0xd70;
    /* 0xD72 */ s16 mMsgNo;
    /* 0xD74 */ s16 mLookMode;
    /* 0xD76 */ u16 mMode;
    /* 0xD78 */ s16 field_0xd78;
    /* 0xD7A */ u8 mStatus;
    /* 0xD7B */ u8 mColor;
};

STATIC_ASSERT(sizeof(daNpcTkc_c) == 0xd7c);

#endif /* D_A_NPC_TKC_H */
