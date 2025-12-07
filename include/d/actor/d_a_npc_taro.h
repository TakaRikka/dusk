#ifndef D_A_NPC_TARO_H
#define D_A_NPC_TARO_H

#include "SSystem/SComponent/c_counter.h"
#include "d/actor/d_a_npc.h"

struct daNpc_Taro_HIOParam {
    /* 0x00 */ f32 mAttentionPosYOffset;
    /* 0x04 */ f32 mGravity;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 mSttsWeight;
    /* 0x14 */ f32 mCylH;
    /* 0x18 */ f32 mWallH;
    /* 0x1C */ f32 mWallR;
    /* 0x20 */ f32 mBodyDownAngle;
    /* 0x24 */ f32 mBodyUpAngle;
    /* 0x28 */ f32 mBodyRightAngle;
    /* 0x2C */ f32 mBodyLeftAngle;
    /* 0x30 */ f32 mHeadDownAngle;
    /* 0x34 */ f32 mHeadUpAngle;
    /* 0x38 */ f32 mHeadRightAngle;
    /* 0x3C */ f32 mHeadLeftAngle;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 mMorfFrames;
    /* 0x48 */ s16 field_0x48;
    /* 0x4A */ s16 field_0x4a;
    /* 0x4C */ s16 field_0x4c;
    /* 0x4E */ s16 field_0x4e;
    /* 0x50 */ f32 mAttnFovY;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5c;
    /* 0x60 */ s16 field_0x60;
    /* 0x62 */ s16 field_0x62;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 field_0x6c;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ f32 field_0x7c;
    /* 0x80 */ f32 field_0x80;
    /* 0x84 */ f32 field_0x84;
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ s16 mChoccaiTimer;
    /* 0x8E */ s16 field_0x8e;
};

class daNpc_Taro_Param_c {
    public:
        virtual ~daNpc_Taro_Param_c() {}
    
        static daNpc_Taro_HIOParam const m;
    };

class daNpc_Taro_HIO_c 
#if DEBUG
: public mDoHIO_entry_c 
#endif
{
public:
    void genMessage(JORMContext*);
};

/**
 * @ingroup actors-npcs
 * @class daNpc_Taro_c
 * @brief Talo
 *
 * @details
 *
 */
class daNpc_Taro_c : public daNpcT_c {
public:
    typedef int (daNpc_Taro_c::*cutFunc)(int);
    typedef int (daNpc_Taro_c::*actionFunc)(void*);

    enum TaroType {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
        TYPE_6,
        TYPE_7,
        TYPE_8,
        TYPE_9,
        TYPE_10,
        TYPE_11,
        TYPE_12,
        TYPE_13,
        TYPE_14,
        TYPE_15,
        TYPE_16,
    };

    ~daNpc_Taro_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchArrow(void*, void*);
    static void* srchNpc(void*, void*);
    fopAc_ac_c* getArrowP();
    u8 getType();
    BOOL isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    BOOL checkChangeEvt();
    BOOL evtEndProc();
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
    daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c);
    BOOL selectAction();
    BOOL chkAction(int (daNpc_Taro_c::*)(void*));
    BOOL setAction(int (daNpc_Taro_c::*)(void*));
    void setSwingVoice();
    int cutConversationAboutPachinko(int);
    int cutConversationAboutWoodSwd(int);
    int cutSwdTutorial(int);
    int cutHail(int);
    int cutFindMonkey(int);
    int cutHelpMe(int);
    int cutAppearanceMoi(int);
    int cutGiveMeWoodSwd(int);
    int cutGetWoodSwd(int);
    int cutConversationWithMaro(int);
    int cutCacaricoConversation(int);
    int cutArrowTutorial(int);
    int cutCaution(int);
    int cutTagPush1(int);
    int cutNotGonnaLet(int);
    int cutTagPush4(int);
    int wait(void*);
    int swdTutorial(void*);
    int talk_withMaro(void*);
    int practice(void*);
    int nurse(void*);
    int talk(void*);
    daNpc_Taro_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        OS_REPORT("|%06d:%x|daNpc_Taro_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int jntNo) { return jntNo == 4; }
    BOOL checkRemoveJoint(int jntNo) { return jntNo == 7; }
    s32 getFootLJointNo() { return 19; }
    s32 getFootRJointNo() { return 22; }

    u32 getFlowNodeNo() { 
        u16 nodeNo = home.angle.x;
        u32 rv;
        if (nodeNo == 0xffff) {
            rv = -1;
        } else {
            rv = nodeNo;
        }
        return rv;
    }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }
    u8 getBitSW2() { return (fopAcM_GetParam(this) & 0xff000000) >> 24; }

    static char* mCutNameList[17];
    static cutFunc mCutList[17];

private:
    /* 0x0E40 */ daNpc_Taro_HIO_c* field_0xe40;
    /* 0x0E44 */ J3DModel* mModels[2];
    /* 0x0E4C */ dCcD_Cyl mCyl1;
    /* 0x0F88 */ dCcD_Cyl mCyl2;
    /* 0x10C4 */ u8 mType;
    /* 0x10C8 */ daNpcT_ActorMngr_c mActors[23];
    /* 0x1180 */ actionFunc mAction;
    /* 0x118C */ actionFunc mAction2;
    /* 0x1198 */ int mChoccaiTimer;
    /* 0x119C */ int field_0x119c;
    /* 0x11A0 */ u8 field_0x11a0;
    /* 0x11A1 */ u8 field_0x11a1;
    /* 0x11A2 */ u8 field_0x11a2;
    /* 0x11A3 */ u8 field_0x11a3;
    /* 0x11A4 */ u8 field_0x11a4;
    /* 0x11A5 */ u8 field_0x11a5;
    /* 0x11A6 */ u8 field_0x11a6;
    /* 0x11A7 */ u8 field_0x11a7;
    /* 0x11A8 */ int field_0x11a8;
};

STATIC_ASSERT(sizeof(daNpc_Taro_c) == 0x11ac);


#endif /* D_A_NPC_TARO_H */
