#ifndef D_A_NPC_YAMIT_H
#define D_A_NPC_YAMIT_H

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_tag_yami.h"

struct daNpc_yamiT_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_yamiT_Param_c {
public:
    virtual ~daNpc_yamiT_Param_c() {}

    static const daNpc_yamiT_HIOParam m;
};

#if DEBUG
class daNpc_yamiT_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_yamiT_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_yamiT_HIOParam m;
};

#define NPC_YAMIT_HIO_CLASS daNpc_yamiT_HIO_c
#else
#define NPC_YAMIT_HIO_CLASS daNpc_yamiT_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_yamiT_c
 * @brief Twili (Tall)
 *
 * @details
 *
 */
class daNpc_yamiT_c : public daNpcT_c {
public:
    typedef BOOL (daNpc_yamiT_c::*cutFunc)(int);
    typedef int (daNpc_yamiT_c::*actionFunc)(void*);

    ~daNpc_yamiT_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    static void* _search_Tag(void*, void*);
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    u32 getFlowNodeNo();
    BOOL isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    BOOL evtEndProc();
    void drawGhost();
    int selectAction();
    BOOL chkAction(int (daNpc_yamiT_c::*)(void*));
    int setAction(int (daNpc_yamiT_c::*)(void*));
    int wait(void*);
    int talk(void*);
    BOOL cutStopper(int);
    BOOL _cutStopper_Init(int const&);
    BOOL _cutStopper_Main(int const&);
    daNpc_yamiT_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 6; }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    int getSwitchBitNo() {
        u32 full_prm = fopAcM_GetParam(this);
        return (full_prm >> 16) & 0xFF;
    }
    u8 _chk_TagPrm(fopAc_ac_c* i_tag) { return (fopAcM_GetParam(i_tag) & 0xFF) == 2; }
    BOOL _is_vanish_prm() {
        int reg_r31 = (fopAcM_GetParam(this) >> 28) != 0;
        if (reg_r31 == 15) {
            reg_r31 = 0;
        }

        return reg_r31;
    }

    BOOL is_vanish() { return mVanish; }
    void vanish_on() { mVanish = 1; }
    void vanish_off() { mVanish = 0; }
    void on_CoHit() {
        field_0xe44.OnCoSetBit();
        field_0xe44.OnTgSetBit();
    }
    void off_CoHit() {
        field_0xe44.OffCoSetBit();
        field_0xe44.OffTgSetBit();
    }
    u8 _is_stopper_off() { return fopAcM_isSwitch(this, 0x3D) && fopAcM_isSwitch(this, 0x3E); }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0xE40 */ NPC_YAMIT_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl field_0xe44;
    /* 0xF80 */ u8 mType;
    /* 0xF81 */ s8 mVanish;
    /* 0xF82 */ s8 field_0xf82;
    /* 0xF83 */ s8 field_0xf83;
    /* 0xF84 */ daTagYami_c* mpTagYami;
    /* 0xF84 */ actionFunc mNextAction;
    /* 0xF90 */ actionFunc mAction;
    /* 0xF9C */ daNpcT_Path_c mPath;
    /* 0xFC4 */ u8 field_0xfc4;
};

STATIC_ASSERT(sizeof(daNpc_yamiT_c) == 0xfcc);

#endif /* D_A_NPC_YAMIT_H */
