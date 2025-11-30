#ifndef D_A_NPC_KAKASHI_H
#define D_A_NPC_KAKASHI_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Kakashi_c
 * @brief Scarecrow
 *
 * @details
 *
 */
class daNpc_Kakashi_c : public daNpcT_c {
public:
    typedef int (daNpc_Kakashi_c::*ActionFunc)(void*);
    
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    int getType();
    int isDelete();
    void reset();
    void srchActors();
    int selectAction();
    BOOL chkAction(ActionFunc);
    int setAction(ActionFunc);
    fopAc_ac_c* hitChk();
    void setStaggerParam(fopAc_ac_c*);
    void setPrtcls(int);
    int judgeSwdTutorial();
    int cutSwdTutorial(int);
    int cutGetWoodSwd(int);
    int cutMarosWhisper(int);
    int wait(void*);
    int swdTutorial(void*);
    int talk(void*);

    daNpc_Kakashi_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                                   daNpcT_motionAnmData_c const* i_motionAnmData,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                                   int i_faceMotionStepNum,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                                   daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                    : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                               i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                               i_arcNames)
                    {
                        OS_REPORT("|%06d:%x|daNpc_Kakashi_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }

    virtual ~daNpc_Kakashi_c();
    virtual int ctrlJoint(J3DJoint*, J3DModel*);
    virtual void setParam();
    virtual BOOL checkChangeEvt();
    virtual BOOL evtTalk();
    virtual BOOL evtEndProc();
    virtual BOOL evtCutProc();
    virtual void action();
    virtual void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual int drawDbgInfo();

    int getFlowNodeNo() {
        u16 prm = home.angle.x;
        return prm == 0xFFFF ? -1 : prm;
    }

    u8 getTalkPattern() {
        u8 var_r31 = (fopAcM_GetParam(this) & 0xF0000000) >> 0x1C;
        if (var_r31 == 15) {
            var_r31 = 0;
        }
        return var_r31;
    }

    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u8 getBitSW2() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }

    static char* mCutNameList[4];
    static int (daNpc_Kakashi_c::*mCutList[])(int);

private:
    /* 0x0E40 */ u8 field_0xE40[0xE44 - 0xE40];
    /* 0x0E44 */ Z2SoundObjSimple mSound;
    /* 0x0E64 */ dCcD_Cyl mCcCyl;
    /* 0x0FA0 */ dCcD_Sph mCcSph[3];
    /* 0x1348 */ u8 mType;
    /* 0x134C */ ActionFunc mSelectAction;
    /* 0x1358 */ ActionFunc mAction;
    /* 0x1364 */ csXyz field_0x1364[3];
    /* 0x1378 */ f32 field_0x1378[3];
    /* 0x1384 */ s16 field_0x1384[3];
    /* 0x138A */ u8 field_0x138A[0x138C - 0x138A];
    /* 0x138C */ s16 field_0x138c;
    /* 0x138E */ u8 field_0x138e;
    /* 0x138F */ u8 field_0x138f;
    /* 0x1390 */ u8 field_0x1390;
    /* 0x1391 */ u8 field_0x1391;
    /* 0x1392 */ u8 field_0x1392;
    /* 0x1393 */ u8 field_0x1393;
    /* 0x1394 */ u8 field_0x1394;
};

STATIC_ASSERT(sizeof(daNpc_Kakashi_c) == 0x1398);

struct daNpc_Kakashi_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
};

class daNpc_Kakashi_Param_c {
public:
    virtual ~daNpc_Kakashi_Param_c() {}

    static const daNpc_Kakashi_HIOParam m;
};


#endif /* D_A_NPC_KAKASHI_H */
