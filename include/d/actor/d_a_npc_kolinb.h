#ifndef D_A_NPC_KOLINB_H
#define D_A_NPC_KOLINB_H

#include "d/actor/d_a_npc.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Kolinb_c
 * @brief Colin (Bedridden) / Ralis (Bedridden)
 *
 * @details
 *
*/

struct daNpc_Kolinb_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Kolinb_Param_c {
public:
    virtual ~daNpc_Kolinb_Param_c() {}

    static daNpc_Kolinb_HIOParam const m;
};

class daNpc_Kolinb_c : public daNpcT_c {
public:
    enum Joint {
        /* 0x00 */ KOLINB_JNT_CENTER,
        /* 0x01 */ KOLINB_JNT_BED_ROOT,
        /* 0x02 */ KOLINB_JNT_MAT,
        /* 0x03 */ KOLINB_JNT_KOLIN_CENTER,
        /* 0x04 */ KOLINB_JNT_BACKBONE1,
        /* 0x05 */ KOLINB_JNT_BACKBONE2,
        /* 0x06 */ KOLINB_JNT_NECK,
        /* 0x07 */ KOLINB_JNT_HEAD,
        /* 0x08 */ KOLINB_JNT_CHIN,
        /* 0x09 */ KOLINB_JNT_MAYUL,
        /* 0x0A */ KOLINB_JNT_MAYUR,
        /* 0x0B */ KOLINB_JNT_MOUTH,
        /* 0x0C */ KOLINB_JNT_SHOULDERL,
        /* 0x0D */ KOLINB_JNT_ARML1,
        /* 0x0E */ KOLINB_JNT_ARML2,
        /* 0x0F */ KOLINB_JNT_HANDL,
        /* 0x10 */ KOLINB_JNT_FINGERL,
        /* 0x11 */ KOLINB_JNT_SHOULDERR,
        /* 0x12 */ KOLINB_JNT_ARMR1,
        /* 0x13 */ KOLINB_JNT_ARMR2,
        /* 0x14 */ KOLINB_JNT_HANDR,
        /* 0x15 */ KOLINB_JNT_FINGERR,
        /* 0x16 */ KOLINB_JNT_CLOTH_ROOT,
        /* 0x17 */ KOLINB_JNT_CLOTH,
        /* 0x18 */ KOLINB_JNT_WAIST,
        /* 0x19 */ KOLINB_JNT_PILLOW_ROOT,
        /* 0x1A */ KOLINB_JNT_PILLOW,

        /* 0x00 */ ZRCB_JNT_CENTER = 0x0,
        /* 0x01 */ ZRCB_JNT_BED_ROOT,
        /* 0x02 */ ZRCB_JNT_ZRC_CENTER,
        /* 0x03 */ ZRCB_JNT_BACKBONE1,
        /* 0x04 */ ZRCB_JNT_BACKBONE2,
        /* 0x05 */ ZRCB_JNT_NECK,
        /* 0x06 */ ZRCB_JNT_HEAD,
        /* 0x07 */ ZRCB_JNT_H_TAIL1,
        /* 0x08 */ ZRCB_JNT_H_TAIL2,
        /* 0x09 */ ZRCB_JNT_H_TAIL3,
        /* 0x0A */ ZRCB_JNT_CHIN,
        /* 0x0B */ ZRCB_JNT_EARL1,
        /* 0x0C */ ZRCB_JNT_EARL2,
        /* 0x0D */ ZRCB_JNT_EARR1,
        /* 0x0E */ ZRCB_JNT_EARR2,
        /* 0x0F */ ZRCB_JNT_MOUTH,
        /* 0x10 */ ZRCB_JNT_SHOULDERL,
        /* 0x11 */ ZRCB_JNT_ARML1,
        /* 0x12 */ ZRCB_JNT_SHOULDERR,
        /* 0x13 */ ZRCB_JNT_ARMR1,
        /* 0x14 */ ZRCB_JNT_CLOTH_ROOT,
        /* 0x15 */ ZRCB_JNT_CLOTH,
    };

    typedef int (daNpc_Kolinb_c::*cutFunc)(int);
    typedef int (daNpc_Kolinb_c::*actionFunc)(void*);

    ~daNpc_Kolinb_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    BOOL isDelete();
    void reset();
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
    int selectAction();
    BOOL chkAction(actionFunc);
    int setAction(actionFunc);
    int cutConversationInHotel(int);
    int cutConversationAboutDeathMt(int);
    int cutConversationAboutGoron(int);
    int cutNurse(int);
    int cutClothTry(int);
    int cutThankYou(int);
    int wait(void*);
    int talk(void*);
    int test(void*);
    daNpc_Kolinb_c(
            daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
            daNpcT_motionAnmData_c const* i_motionAnmData,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
            int i_faceMotionStepNum,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
            int i_motionStepNum,
            daNpcT_evtData_c const* i_evtData,
            char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
        i_arcNames) {}
    s32 getHeadJointNo() { return mType == 2 ? ZRCB_JNT_HEAD : KOLINB_JNT_HEAD; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == ((mType == 2 ? ZRCB_JNT_H_TAIL1 : KOLINB_JNT_CHIN) - 1); }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == ((mType == 2 ? ZRCB_JNT_MOUTH : KOLINB_JNT_MOUTH) - 1); }
    s32 getBackboneJointNo() { return mType == 2 ? ZRCB_JNT_BACKBONE1 : KOLINB_JNT_BACKBONE1; }
    s32 getNeckJointNo() { return mType == 2 ? ZRCB_JNT_NECK : KOLINB_JNT_NECK; }

    static char* mCutNameList[7];
    static cutFunc mCutList[7];

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

    u32 getModelType() { return fopAcM_GetParam(this) >> 28; }

private:
    /* 0xE40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0xE44 */ dCcD_Cyl field_0xe44;
    /* 0xF80 */ Mtx mMtx;
    /* 0xFB0 */ dBgW* mpBgW;
    /* 0xFB4 */ u8 mType;
    /* 0xFB8 */ actionFunc mNextAction;
    /* 0xFC4 */ actionFunc mAction;
    /* 0xFD0 */ daNpcT_Path_c mPath;
    /* 0xFF8 */ u8 field_0xff8;
};

STATIC_ASSERT(sizeof(daNpc_Kolinb_c) == 0xffc);

#endif /* D_A_NPC_KOLINB_H */
