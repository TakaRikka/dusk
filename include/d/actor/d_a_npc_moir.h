#ifndef D_A_NPC_MOIR_H
#define D_A_NPC_MOIR_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcMoiR_c
 * @brief Rusl (Resistance)
 *
 * @details
 *
 */

struct daNpcMoiR_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 demo_start_dist;                 // デモ開始距離 - Demo Start Distance
    /* 0x70 */ s16 horizontal_eye_movement_range;   // 目玉横可動範囲 - Horizontal Eye Movement Range  
    /* 0x72 */ u8 demo_start_area;                  // デモ開始エリア - Demo Start Area
};

class daNpcMoiR_Param_c {
public:
    virtual ~daNpcMoiR_Param_c() {}

    static daNpcMoiR_HIOParam const m;
};

#if DEBUG
class daNpcMoiR_HIO_c : public mDoHIO_entry_c {
public:
    daNpcMoiR_HIO_c();
    void genMessage(JORMContext*);

    daNpcMoiR_HIOParam m;
};

#define NPC_MOIR_HIO_CLASS daNpcMoiR_HIO_c
#else
#define NPC_MOIR_HIO_CLASS daNpcMoiR_Param_c
#endif

class daNpcMoiR_c : public daNpcF_c {
public:
    typedef bool (daNpcMoiR_c::*ActionFn)(void*);
    typedef BOOL (daNpcMoiR_c::*Action2Fn)(void*);
    typedef BOOL (daNpcMoiR_c::*EventFn)(int);

    enum Animation {
        /* 0x00 */ ANM_NONE,
        /* 0x01 */ ANM_F_TALK_A,
        /* 0x02 */ ANM_F_HOLD_S,
        /* 0x03 */ ANM_3,
        /* 0x04 */ ANM_F_SURPRISED,
        /* 0x05 */ ANM_F_HOLD_S_LOOP,
        /* 0x06 */ ANM_F_TAKAJO_HI,
        /* 0x07 */ ANM_FH_HOLD_S,
        /* 0x08 */ ANM_FH_SERIOUS,
        /* 0x09 */ ANM_FH_HOLD_S_2,
        /* 0x0A */ ANM_FH_TAKAJO_HI,
        /* 0x0B */ ANM_WAIT_A,
        /* 0x0C */ ANM_WAIT_B,
        /* 0x0D */ ANM_WAIT_C,
        /* 0x0E */ ANM_TALK_A,
        /* 0x0F */ ANM_STEP,
        /* 0x10 */ ANM_LOOK_AROUND,
        /* 0x11 */ ANM_FUE,
        /* 0x12 */ ANM_TAKAJO_NI,
        /* 0x13 */ ANM_TAKAJO_WAIT_NI,
        /* 0x14 */ ANM_SURPRISED,
        /* 0x15 */ ANM_SQUAREUP,
        /* 0x16 */ ANM_SQUAREUP_STEP,
        /* 0x17 */ ANM_DRIVEAWAY,
        /* 0x18 */ ANM_TAKEOFF,
        /* 0x19 */ ANM_TAKING_OFF,
        /* 0x1A */ ANM_TAKING_OFF_TALK,
        /* 0x1B */ ANM_TAKE_MET,
        /* 0x1C */ ANM_SIT,
        /* 0x1D */ ANM_SIT_TALK,
        /* 0x1E */ ANM_TAKEOFF_SIT,
        /* 0x1F */ ANM_TAKING_OFF_SIT,
        /* 0x20 */ ANM_TAKING_OFF_SIT_TALK,
        /* 0x21 */ ANM_TAKE_MET_SIT,
        /* 0x22 */ ANM_TAKAJO,
        /* 0x23 */ ANM_TAKAJO_WAIT,
        /* 0x24 */ ANM_TAKAJO_HI,
        /* 0x25 */ ANM_TAKAJO_HI_WAIT,
        /* 0x26 */ ANM_RUN,
    };

    enum Expression {
        /* 0x00 */ EXPR_F_TALK_A,
        /* 0x03 */ EXPR_F_HOLD_S_LOOP = 0x3,
        /* 0x0A */ EXPR_ANM_3 = 0xA,
        /* 0x0B */ EXPR_FH_SERIOUS,
        /* 0x0C */ EXPR_F_TAKAJO_HI,
        /* 0x0D */ EXPR_FH_TAKAJO_HI,
        /* 0x0E */ EXPR_FH_HOLD_S,
        /* 0x0F */ EXPR_F_SURPRISED,
        /* 0x10 */ EXPR_F_HOLD_S,
        /* 0x11 */ EXPR_FH_HOLD_S_LOOP,
        /* 0x12 */ EXPR_NONE,
    };

    enum Expression_BTP {
        /* 0x0 */ EXPR_BTP_MOIR,
        /* 0x1 */ EXPR_BTP_FH_SERIOUS
    };

    enum Motion {
        /* 0x00 */ MOT_WAIT_A,
        /* 0x01 */ MOT_TALK_A,
        /* 0x02 */ MOT_WAIT_B,
        /* 0x07 */ MOT_WAIT_C = 0x7,
        /* 0x14 */ MOT_LOOK_AROUND_LOOP = 0x14,
        /* 0x15 */ MOT_TAKAJO_WAIT_NI,
        /* 0x16 */ MOT_SIT,
        /* 0x17 */ MOT_SIT_TALK,
        /* 0x18 */ MOT_SQUAREUP,
        /* 0x19 */ MOT_DRIVEAWAY,
        /* 0x1A */ MOT_TAKEOFF,
        /* 0x1B */ MOT_TAKING_OFF,
        /* 0x1C */ MOT_TAKING_OFF_TALK,
        /* 0x1D */ MOT_TAKE_MET,
        /* 0x1E */ MOT_TAKEOFF_SIT,
        /* 0x1F */ MOT_TAKING_OFF_SIT,
        /* 0x20 */ MOT_TAKING_OFF_SIT_TALK,
        /* 0x21 */ MOT_TAKE_MET_SIT,
        /* 0x22 */ MOT_RUN,
        /* 0x23 */ MOT_TAKAJO_HI_WAIT,
        /* 0x24 */ MOT_TAKAJO_HI,
        /* 0x25 */ MOT_SURPRISED,
        /* 0x26 */ MOT_SQUAREUP_STEP,
        /* 0x27 */ MOT_TAKAJO_NI,
        /* 0x28 */ MOT_FUE,
        /* 0x29 */ MOT_LOOK_AROUND,
        /* 0x2A */ MOT_STEP,
    };

    enum Mode {
        /* 0x0 */ MODE_SIT,
        /* 0x1 */ MODE_STAND,
        /* 0x2 */ MODE_2,
    };

    daNpcMoiR_c();
    ~daNpcMoiR_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    inline bool setExpressionAnm(int, bool);
    inline bool setExpressionBtp(int);
    void setMotionAnm(int, f32);
    void reset();
    inline void setWaitAction();
    bool wait_type0(void*);
    void setMotion(int, f32, int);
    void setExpression(int, f32);
    bool wait_type1(void*);
    bool wait_type2(void*);
    bool talk(void*);
    BOOL multiTalk(void*);
    bool fight(void*);
    bool demo(void*);
    bool leave(void*);
    inline BOOL EvCut_Introduction(int);
    inline BOOL EvCut_Appear(int);
    inline BOOL EvCut_Appear2(int);
    inline void setParam();
    inline BOOL main();
    inline void checkHeadGear();
    inline void playMotion();
    inline BOOL ctrlBtk();
    inline void setAttnPos();
    inline void lookat();
    inline void drawOtherMdls();
    inline BOOL drawDbgInfo();

    MtxP getHandRMtx() { return mAnm_p->getModel()->getAnmMtx(17); }

    s16 getMessageNo() { return (fopAcM_GetParam(this) >> 8) & 0xFFFF; }

    inline int setAction(ActionFn action) {
        field_0xe08 = 3;

        if (mAction) {
            (this->*mAction)(NULL);
        }

        field_0xe08 = 0;
        mAction = action;

        if (mAction) {
            (this->*mAction)(NULL);
        }

        return 1;
    }

    inline void playExpression();
    inline BOOL step(s16, int);
    inline void setExpressionTalkAfter();
    inline BOOL chkFindPlayer();
    inline bool chkAction(ActionFn action) {
        return action == mAction;
    }
    inline void setLookMode(int i_lookMode);
    inline void searchActors();

    static EventFn mEvtSeqList[4];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* mpHeadgearModel;
    /* 0xBDC */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBE0 */ daNpcF_Lookat_c mLookat;
    /* 0xC7C */ daNpcF_ActorMngr_c mActorMngr[4];
    /* 0xC9C */ NPC_MOIR_HIO_CLASS* mpHIO;
    /* 0xCA0 */ dCcD_Cyl field_0xca0;
    /* 0xDDC */ ActionFn mAction;
    /* 0xDE8 */ request_of_phase_process_class mPhase[2];
    /* 0xDF8 */ fpc_ProcID field_0xdf8;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ s16 mMsgNo;
    /* 0xE06 */ s16 mLookMode;
    /* 0xE08 */ u16 field_0xe08;
    /* 0xE0A */ bool mWearHelmet;
    /* 0xE0B */ u8 mMode;
};

STATIC_ASSERT(sizeof(daNpcMoiR_c) == 0xe0c);

#endif /* D_A_NPC_MOIR_H */
