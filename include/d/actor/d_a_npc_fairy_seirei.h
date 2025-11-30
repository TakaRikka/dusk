#ifndef D_A_NPC_FAIRY_SEIREI_H
#define D_A_NPC_FAIRY_SEIREI_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_FairySeirei_c
 * @brief Fairy Spirit
 *
 * @details
 *
 */
class daNpc_FairySeirei_c : public daNpcT_c {
public:
    typedef int (daNpc_FairySeirei_c::*cutFunc)(int);
    typedef int (daNpc_FairySeirei_c::*actionFunc)(int);

    ~daNpc_FairySeirei_c();
    int create();
    int Delete();
    int Execute();
    int Draw();
    int isDelete();
    void reset();
    void setParam();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setCollision();
    void setAttnPos();
    BOOL drawDbgInfo();
    bool selectAction();
    bool setAction(actionFunc);
    void setPrtcls();
    int wait(int);
    int talk(int);
    void _to_FairyCave();
    daNpc_FairySeirei_c(daNpcT_faceMotionAnmData_c const* param_1,
                                       daNpcT_motionAnmData_c const* param_2,
                                       daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3,
                                       int param_4,
                                       daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5,
                                       int param_6, daNpcT_evtData_c const* param_7, char** param_8)
        : daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {}

    u8 getType() { return fopAcM_GetParam(this) & 0xFF; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    BOOL chkAction(actionFunc i_action) { return (mExecuteFunc == i_action); }

    void ptcl_se() { mSound.startCreatureSoundLevel(Z2SE_FAIRY_STAY, 0, -1); }

    int getSeneNo() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ u8 field_0xE40[4];
    /* 0xE44 */ u8 mType;
    /* 0xE48 */ dCcD_Cyl mCyl;
    /* 0xF84 */ actionFunc mInitFunc;
    /* 0xF90 */ actionFunc mExecuteFunc;
    /* 0xF98 */ u32 mpEmitters[3];
    /* 0xFA8 */ bool mTalking;
    /* 0xFA9 */ u8 field_0xFA4[3];
    /* 0xFAC */ u8 field_0xFAC;
};

STATIC_ASSERT(sizeof(daNpc_FairySeirei_c) == 0xfb0);

class daNpc_FairySeirei_Param_c {
public:
    virtual ~daNpc_FairySeirei_Param_c() {}

    static f32 const m[37];
};

#endif /* D_A_NPC_FAIRY_SEIREI_H */
