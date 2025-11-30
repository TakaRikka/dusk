#ifndef D_A_NPC_IMPAL_H
#define D_A_NPC_IMPAL_H

#include "d/actor/d_a_npc4.h"

struct daNpcImpal_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 demo_start_dist;
};

class daNpcImpal_Param_c {
public:
    virtual ~daNpcImpal_Param_c() {}

    static const daNpcImpal_HIOParam m;
};

#if DEBUG
class daNpcImpal_HIO_c : public mDoHIO_entry_c {
public:
    daNpcImpal_HIO_c();

    void genMessage(JORMContext*);

    daNpcImpal_HIOParam m;
};

#define NPC_IMPAL_HIO_CLASS daNpcImpal_HIO_c
#else
#define NPC_IMPAL_HIO_CLASS daNpcImpal_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpcImpal_c
 * @brief Impaz
 *
 * @details
 *
 */
class daNpcImpal_c : public daNpcF_c {
public:
    typedef int (daNpcImpal_c::*cutFunc)(int);
    typedef bool (daNpcImpal_c::*actionFunc)(void*);
    typedef BOOL (daNpcImpal_c::*EventFn)(int);

    enum Animation {
        /* 0x00 */ ANM_0,
        /* 0x01 */ ANM_1,
        /* 0x02 */ ANM_2,
        /* 0x03 */ ANM_3,
        /* 0x04 */ ANM_4,
        /* 0x05 */ ANM_5,
        /* 0x06 */ ANM_6,
        /* 0x07 */ ANM_7,
        /* 0x08 */ ANM_8,
        /* 0x09 */ ANM_9,
        /* 0x10 */ ANM_10,
        /* 0x11 */ ANM_11,
        /* 0x12 */ ANM_12,
    };

    enum Expression {
        /* 0x0 */ EXPR_0,
        /* 0x1 */ EXPR_1,
        /* 0x2 */ EXPR_2,
        /* 0x3 */ EXPR_3,
        /* 0x4 */ EXPR_4,
        /* 0x5 */ EXPR_5,
        /* 0x6 */ EXPR_6,
        /* 0x7 */ EXPR_7,
    };

    enum Motion {
        /* 0x00 */ MOT_0,
        /* 0x01 */ MOT_1,
        /* 0x02 */ MOT_2,
        /* 0x03 */ MOT_3,
        /* 0x04 */ MOT_4,
        /* 0x05 */ MOT_5,
        /* 0x06 */ MOT_6,
    };

    daNpcImpal_c();
    ~daNpcImpal_c();
    int Create();
    BOOL CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    bool ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    inline bool setExpressionAnm(int, bool);
    inline bool setExpressionBtp(int);
    void setMotionAnm(int, f32);
    inline void reset();
    inline bool setAction(daNpcImpal_c::actionFunc);
    bool wait(void*);
    void setMotion(int, f32, BOOL);
    void setExpression(int, f32);
    bool talk(void*);
    bool demo(void*);
    BOOL EvCut_ImpalAppear1(int);
    BOOL EvCut_ImpalAppear2(int);
    BOOL EvCut_CopyRod(int);
    inline void setParam();
    inline BOOL main();
    inline BOOL ctrlBtk();
    inline void setAttnPos();
    inline void lookat();
    inline BOOL drawDbgInfo();

    s16 getMessageNo() { return s16(shape_angle.x); }
    int getSwitchNo() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    inline void playExpression();
    inline void playMotion();
    inline int step(s16, int);
    inline void setExpressionTalkAfter();
    inline BOOL chkFindPlayer();
    inline void setLookMode(int i_lookMode);
    inline void deleteObstacle();

    static EventFn mEvtSeqList[4];

private:
    /* 0xB48 */ Z2Creature mCreatureSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xC80 */ NPC_IMPAL_HIO_CLASS* mpHIO;
    /* 0xC84 */ dCcD_Cyl mCyl;
    /* 0xDC0 */ actionFunc mpActionFn;
    /* 0xDCC */ request_of_phase_process_class mPhase[1];
    /* 0xDD4 */ u32 mItemPartnerId;
    /* 0xDD8 */ u32 field_0xdd8;
    /* 0xDDC */ u32 field_0xddc;
    /* 0xDE0 */ s16 mFlowID;
    /* 0xDE2 */ s16 field_de2;
    /* 0xDE4 */ s16 mLookMode;
    /* 0xDE6 */ u16 mMode;
    /* 0xDE8 */ bool field_0xde8;
    /* 0xDE9 */ bool field_0xde9;
};

STATIC_ASSERT(sizeof(daNpcImpal_c) == 0xdec);

#endif /* D_A_NPC_IMPAL_H */
