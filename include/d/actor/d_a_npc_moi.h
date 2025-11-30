#ifndef D_A_NPC_MOI_H
#define D_A_NPC_MOI_H

#include "d/actor/d_a_npc.h"

struct daNpc_Moi_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ s16 field_0x98;
    /* 0x9A */ s16 field_0x9a;
    /* 0x9C */ s16 field_0x9c;
    /* 0x9E */ s16 field_0x9e;
    /* 0xA0 */ s16 field_0xa0;
    /* 0xA2 */ s16 field_0xa2;
    /* 0xA4 */ f32 field_0xa4;
    /* 0xA8 */ f32 field_0xa8;
    /* 0xAC */ f32 field_0xac;
    /* 0xB0 */ f32 field_0xb0;
    /* 0xB4 */ f32 field_0xb4;
    /* 0xB8 */ f32 field_0xb8;
};

class daNpc_Moi_Param_c {
public:
    virtual ~daNpc_Moi_Param_c() {};

    static const daNpc_Moi_HIOParam m;
};

#if DEBUG
class daNpc_Moi_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Moi_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Moi_HIOParam m;
};

#define NPC_MOI_HIO_CLASS daNpc_Moi_HIO_c
#else
#define NPC_MOI_HIO_CLASS daNpc_Moi_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Moi_c
 * @brief Rusl
 *
 * @details
 *
 */
class daNpc_Moi_c : public daNpcT_c {
public:
    typedef int (daNpc_Moi_c::*cutFunc)(int);
    typedef int (daNpc_Moi_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
        TYPE_6,
    };

    ~daNpc_Moi_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    BOOL isDelete();
    void reset();
    BOOL chkMoiN();
    void afterJntAnm(int);
    void setParam();
    BOOL checkChangeEvt();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawOtherMdl();
    bool afterSetMotionAnm(int, int, f32, int);
    void changeAnm(int*, int*);
    void changeBck(int*, int*);
    void changeBtp(int*, int*);
    void changeBtk(int*, int*);
    int selectAction();
    BOOL chkAction(int (daNpc_Moi_c::*)(void*));
    int setAction(int (daNpc_Moi_c::*)(void*));
    BOOL chkPlayerGetWoodShield();
    void setSSlash(int);
    BOOL chkPullOutSw();
    void pullOutSw();
    BOOL chkSetInSw();
    void setInSw();
    void setSwAT();
    void torch();
    int injuryCheck();
    int injuryTurn(cXyz);
    int injuryWalk();
    int poise();
    int cutDeliveredSw(int);
    int cutAppearanceMoi(int);
    int cutConversationWithUri(int);
    int cutFindWolf(int);
    int wait(void*);
    int walk(void*);
    int walkOnEggshell(void*);
    int rest(void*);
    int talk(void*);
    daNpc_Moi_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}
    u16 getEyeballMaterialNo() {
        if (chkMoiN()) {
            return 4;
        } else {
            return 2;
        }
    }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int param_0) { return param_0 == 4; }
    BOOL checkRemoveJoint(int param_0) { return param_0 == 8; }
    s32 getFootLJointNo() { return 22; }
    s32 getFootRJointNo() { return 25; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }
    bool chkSFight() { return field_0x166b == 1; }
    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }

    static char* mCutNameList[5];
    static cutFunc mCutList[5];

private:
    /* 0x0E40 */ NPC_MOI_HIO_CLASS* mpHIO;
    /* 0x0E44 */ J3DModel* mpModel[6];
    /* 0x0E5C */ dCcD_Cyl mCyl[4];
    /* 0x134C */ dCcD_Cyl mCyl2;
    /* 0x1488 */ dCcD_Sph mSph;
    /* 0x15C0 */ fpc_ProcID field_0x15c0;
    /* 0x15C4 */ u8 mType;
    /* 0x15C8 */ daNpcT_ActorMngr_c mActorMngr[4];
    /* 0x15E8 */ daNpcT_Path_c mPath;
    /* 0x1610 */ actionFunc field_0x1610;
    /* 0x161C */ actionFunc field_0x161c;
    /* 0x1628 */ cXyz field_0x1628;
    /* 0x1634 */ csXyz field_0x1634[3];
    /* 0x1648 */ int field_0x1648;
    /* 0x164C */ int field_0x164c;
    /* 0x1650 */ int field_0x1650;
    /* 0x1654 */ int field_0x1654;
    /* 0x1658 */ int field_0x1658;
    /* 0x165C */ int field_0x165c;
    /* 0x1660 */ int field_0x1660;
    /* 0x1664 */ s16 field_0x1664;
    /* 0x1666 */ u8 field_0x1666;
    /* 0x1667 */ u8 field_0x1667;
    /* 0x1668 */ u8 field_0x1668;
    /* 0x1669 */ u8 field_0x1669;
    /* 0x166A */ u8 field_0x166a;
    /* 0x166B */ u8 field_0x166b;
    /* 0x166C */ u8 field_0x166c;
    /* 0x166D */ u8 field_0x166d;
    /* 0x166E */ u8 field_0x166e;
    /* 0x166F */ u8 field_0x166f;
    /* 0x1670 */ int field_0x1670;
};

STATIC_ASSERT(sizeof(daNpc_Moi_c) == 0x1674);

#endif /* D_A_NPC_MOI_H */
