#ifndef D_A_NPC_THE_H
#define D_A_NPC_THE_H

#include "d/actor/d_a_npc4.h"

struct daNpcThe_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
};

class daNpcThe_Param_c {
public:
    virtual ~daNpcThe_Param_c() {}

    static const daNpcThe_HIOParam m;
};

STATIC_ASSERT(sizeof(daNpcThe_Param_c::m) == 0x6C);

#if DEBUG
class daNpcThe_HIO_c : public mDoHIO_entry_c {
public:
    daNpcThe_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpcThe_HIOParam m;
};

#define NPC_THE_HIO_CLASS daNpcThe_HIO_c
#else
#define NPC_THE_HIO_CLASS daNpcThe_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpcThe_c
 * @brief Telma
 *
 * @details
 *
 */
class daNpcThe_c : public daNpcF_c {
public:
    typedef BOOL (daNpcThe_c::*ActionFn)(void*);
    typedef BOOL (daNpcThe_c::*EventFn)(int);

    enum Type {
        /* 0 */ TYPE_BAR,
        /* 1 */ TYPE_1,
        /* 2 */ TYPE_KAKARIKO,
    };

    enum Animation {
        /* 0x00 */ ANM_NONE,
        /* 0x01 */ ANM_F_TALK_A,
        /* 0x02 */ ANM_F_SMILE,
        /* 0x03 */ ANM_F_CHUCKLE,
        /* 0x04 */ ANM_F_TALK_B,
        /* 0x05 */ ANM_F_TALK_C,
        /* 0x06 */ ANM_F_LAUGH,
        /* 0x07 */ ANM_F_TALK_R,
        /* 0x08 */ ANM_FH_SMILE,
        /* 0x09 */ ANM_FH_CHUCKLE,
        /* 0x0A */ ANM_FH_TALK_B,
        /* 0x0B */ ANM_FH_TALK_C,
        /* 0x0C */ ANM_FH_LAUGH,
        /* 0x0D */ ANM_FH_TALK_R,
        /* 0x0E */ ANM_WAIT_A,
        /* 0x0F */ ANM_WAIT_LOOKING,
        /* 0x10 */ ANM_TO_WAIT_B,
        /* 0x11 */ ANM_WAIT_B,
        /* 0x12 */ ANM_WAIT_LOOKUP,
        /* 0x13 */ ANM_TALK_R,
        /* 0x14 */ ANM_TALKING_R,
        /* 0x15 */ ANM_LOOK_A,
        /* 0x16 */ ANM_LOOK,
        /* 0x17 */ ANM_KUNE_WAIT_A,
        /* 0x18 */ ANM_KUNE_TALK,
        /* 0x19 */ ANM_KUNE_WAIT_B,
        /* 0x1A */ ANM_KUNE_SHISHI,
    };

    enum Expression {
        /* 0x0 */ EXPR_TALK_A,
        /* 0x1 */ EXPR_SMILE,
        /* 0x2 */ EXPR_CHUCKLE,
        /* 0x3 */ EXPR_TALK_B,
        /* 0x4 */ EXPR_TALK_C,
        /* 0x5 */ EXPR_LAUGH,
        /* 0x6 */ EXPR_TALK_R,
        /* 0x7 */ EXPR_H_TALK_R,
        /* 0x8 */ EXPR_H_SMILE,
        /* 0x9 */ EXPR_H_CHUCKLE,
        /* 0xA */ EXPR_H_TALK_B,
        /* 0xB */ EXPR_H_TALK_C,
        /* 0xC */ EXPR_H_LAUGH,
        /* 0xD */ EXPR_NONE,
    };

    enum Motion {
        /* 0x0 */ MOT_WAIT_A,
        /* 0x1 */ MOT_LOOK_A,
        /* 0x2 */ MOT_TO_WAIT_B,
        /* 0x3 */ MOT_WAIT_LOOKUP,
        /* 0x4 */ MOT_KUNE_TALK,
        /* 0x5 */ MOT_KUNE_WAIT_A,
        /* 0x6 */ MOT_KUNE_SHISHI,
        /* 0x7 */ MOT_WAIT_B,
        /* 0x8 */ MOT_KUNE_WAIT_B,
        /* 0x9 */ MOT_TALK_R,
        /* 0xA */ MOT_LOOK,
    };

    daNpcThe_c();
    ~daNpcThe_c();
    /*  inline  */ inline BOOL CreateHeap();
    BOOL ctrlJoint(J3DJoint*, J3DModel*);
    static BOOL createHeapCallBack(fopAc_ac_c*);
    static BOOL ctrlJointCallBack(J3DJoint*, int);
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setMotionAnm(int, f32);
    BOOL wait(void*);
    void setMotion(int, f32, int);
    void setExpression(int, f32);
    BOOL waitTW(void*);
    BOOL talk(void*);
    BOOL test(void*);
    BOOL EvCut_TwResistance(int);
    BOOL EvCut_Introduction(int);
    cPhs__Step create();
    void reset();
    /*  inline  */ inline int Draw();
    inline void setParam();
    inline BOOL main();
    inline void playMotion();
    inline void playExpression();
    /*  inline  */ inline void setExpressionTalkAfter();
    inline BOOL doEvent();
    inline BOOL ctrlBtk();
    inline void setAttnPos();
    inline void lookat();
    inline BOOL drawDbgInfo();
    void adjustShapeAngle() {}

    u8 getTypeFromParam() {
        switch (fopAcM_GetParam(this) & 0xff) {
        case 0:
            return TYPE_BAR;
        case 1:
            return TYPE_1;
        case 2:
            return TYPE_KAKARIKO;
        default:
            return TYPE_BAR;
        }
    }

    bool isDelete() {
        if (mType == TYPE_BAR || mType == TYPE_1 || mType == TYPE_KAKARIKO) {
            return false;
        } else {
            return true;
        }
    }

    void setLookMode(int i_lookMode) {
        if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
            mLookMode = i_lookMode;
        }
    }

    bool isSneaking() {
        if (!strcmp(dComIfGp_getStartStageName(), "R_SP116") &&
            dComIfGp_getStartStageRoomNo() == 5 && dComIfG_play_c::getLayerNo(0) == 2)
        {
            return true;
        }
        return false;
    }

    bool chkAction(ActionFn action) { return mpActionFn == action; }

    BOOL setAction(ActionFn action) {
        if (mpActionFn != NULL) {
            mMode = 3;
            (this->*mpActionFn)(NULL);
        }
        if (action != NULL) {
            mpActionFn = action;
            mMode = 0;
            (this->*mpActionFn)(NULL);
        }
        return true;
    }

    void doNormalAction(int) {
        mOrderEvtNo = 0;
        if (mpNextActionFn != NULL) {
            if (mpNextActionFn == mpActionFn) {
                (this->*mpActionFn)(NULL);
            } else {
                ActionFn action = mpNextActionFn;
                setAction(action);
            }
        }
    }

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[3];
    /* 0xC90 */ NPC_THE_HIO_CLASS* mpHIO;
    /* 0xC94 */ dCcD_Cyl mColCyl;
    /* 0xDD0 */ ActionFn mpNextActionFn;
    /* 0xDDC */ ActionFn mpActionFn;
    /* 0xDE8 */ request_of_phase_process_class mPhase[2];
    /* 0xDF8 */ u8 field_0xdf8[8];
    /* 0xE00 */ u32 mItemID;
    /* 0xE04 */ u32 field_0xe04;
    /* 0xE08 */ u32 field_0xe08;
    /* 0xE0C */ u32 field_0xe0c;
    /* 0xE10 */ u32 field_0xe10;
    /* 0xE14 */ int mFlowID;
    /* 0xE18 */ s16 mLookMode;
    /* 0xE1A */ u16 mMode;
    /* 0xE1C */ bool field_0xe1c;
    /* 0xE1D */ bool field_0xe1d;
    /* 0xE1E */ u8 mType;

    static char* mEvtCutNameList[4];
    static EventFn mEvtCutList[4];
};

STATIC_ASSERT(sizeof(daNpcThe_c) == 0xE20);

#endif /* D_A_NPC_THE_H */
