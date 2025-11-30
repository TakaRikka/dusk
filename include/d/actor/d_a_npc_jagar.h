#ifndef D_A_NPC_JAGAR_H
#define D_A_NPC_JAGAR_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Jagar_c
 * @brief Jaggle
 *
 * @details
 *
 */

struct daNpc_Jagar_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 pumpkin_watch_range;
    /* 0x90 */ f32 pumpkin_watch_Ymax;
    /* 0x94 */ f32 pumpkin_watch_Ymin;
    /* 0x98 */ f32 running_speed;
    /* 0x9C */ f32 hidden_state_release_dist;
};

class daNpc_Jagar_Param_c {
public:
    virtual ~daNpc_Jagar_Param_c() {}

    static const daNpc_Jagar_HIOParam m;
};

#if DEBUG
class daNpc_Jagar_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Jagar_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Jagar_HIOParam m;
};

#define NPC_JAGAR_HIO_CLASS daNpc_Jagar_HIO_c
#else
#define NPC_JAGAR_HIO_CLASS daNpc_Jagar_Param_c
#endif

class daNpc_Jagar_c : public daNpcT_c {
public:
    typedef int (daNpc_Jagar_c::*cutFunc)(int);
    typedef int (daNpc_Jagar_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
    };

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHIN,
        /* 0x06 */ JNT_MAYU_L,
        /* 0x07 */ JNT_MAYU_R,
        /* 0x08 */ JNT_MOUTH,
        /* 0x09 */ JNT_SHOULDERL,
        /* 0x0A */ JNT_ARML1,
        /* 0x0B */ JNT_ARML2_1,
        /* 0x0C */ JNT_HANDL_1,
        /* 0x0D */ JNT_FINGERL,
        /* 0x0E */ JNT_SHOULDERR,
        /* 0x0F */ JNT_ARMR1,
        /* 0x10 */ JNT_ARMR2_1,
        /* 0x11 */ JNT_HANDR_1,
        /* 0x12 */ JNT_FINGERR,
        /* 0x13 */ JNT_WAIST,
        /* 0x14 */ JNT_LEGL1,
        /* 0x15 */ JNT_LEGL2,
        /* 0x16 */ JNT_FOOTL,
        /* 0x17 */ JNT_SKIRTL,
        /* 0x18 */ JNT_LEGR1,
        /* 0x19 */ JNT_LEGR2,
        /* 0x1A */ JNT_FOOTR,
        /* 0x1B */ JNT_SKIRTR,
        /* 0x1C */ JNT_SKIRT1,
        /* 0x1D */ JNT_SKIRT2,
    };
    
    enum Material {
        /* 0x1 */ MAT_JAGA_EYEBALL = 0x1,
        /* 0x2 */ MAT_JAGA_FACE_M,
        /* 0x3 */ MAT_JAGA_M,
    };

    ~daNpc_Jagar_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int isDelete();
    void reset();
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
    void changeBtp(int*, int*);
    int selectAction();
    int chkAction(int (daNpc_Jagar_c::*)(void*));
    int setAction(int (daNpc_Jagar_c::*)(void*));
    int cutClimbUp(int);
    int cutNeedYourHelp(int);
    int cutAnger(int);
    int cutConversationWithBou(int);
    int cutConfidentialConversation(int);
    int cutFindWolf(int);
    int chkSitMotion();
    int chkSitMotion2();
    int chkChuMotion();
    int chkToMotion();
    int wait(void*);
    int talkwithBou(void*);
    int talk(void*);
    daNpc_Jagar_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                 {}
    u16 getEyeballMaterialNo() { return MAT_JAGA_EYEBALL; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int param_1) { return param_1 == JNT_HEAD; }
    BOOL checkRemoveJoint(int param_1) { return param_1 == JNT_MOUTH; }
    s32 getFootLJointNo() { return JNT_FOOTL; }
    s32 getFootRJointNo() { return JNT_FOOTR; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    BOOL chkFindWolf() {
        int uVar3 = (int)daNpcT_getDistTableIdx(field_0xff0, field_0xff4);
        return daNpcT_c::chkFindWolf(mCurAngle.y, uVar3, field_0xfec, mpHIO->m.common.search_distance,
                                     mpHIO->m.common.fov, mpHIO->m.common.search_height,
                                     mpHIO->m.common.search_depth, 1);
    }

    int chkCondition(int i_val) {
        if (mStagger.checkStagger()) {
            return false;
        } else if (i_val != 0) {
            return true;
        } else {
            if (mMotionSeqMngr.getNo() == 7 ||
                mMotionSeqMngr.getNo() == 6 ||
                mMotionSeqMngr.getNo() == 8) {
                if (mMotionSeqMngr.getStepNo() > 0) {
                    return true;
                }
            } else if (mMotionSeqMngr.getNo() == 5) {
                return true;
            }
        }
        return false;
    }

    void setSurpriseMotion() {
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(7, -1.0f, 0, 0);
        field_0xff8 = 0;
    }

    void setListenMotion(f32 arg) {
        mFaceMotionSeqMngr.setNo(8, -1.0f, 0, 0);
        if (0.0f < arg + (cM_rnd() - 0.5f)) {
            mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
        } else {
            mMotionSeqMngr.setNo(5, -1.0f, 0, 0);
        }
    }

    void onListen() { mListen = 1; }
    void offListen() { mListen = 0; }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }

    int crashPumpkin(fopAc_ac_c* pumpkin_p) {
        int reg_r30;
        if (mType == 0 && fopAcM_GetName(pumpkin_p) == PROC_OBJ_PUMPKIN && !daNpcT_chkEvtBit(0xAE)
            && daNpcT_chkEvtBit(0x25B) && !daNpcT_chkEvtBit(0x235)) {
            ++field_0xffc;
            reg_r30 = chkPointInArea(pumpkin_p->current.pos, current.pos, mpHIO->m.pumpkin_watch_range,
                                     mpHIO->m.pumpkin_watch_Ymax, mpHIO->m.pumpkin_watch_Ymin, 0);
            if (reg_r30) {
                if (field_0xffc >= 4) {
                    daNpcT_onTmpBit(0x1B);
                }

                return 1;
            }
        }

        return 0;
    }

    static char* mCutNameList[7];
    static cutFunc mCutList[7];
private:
    /* 0x0E40 */ NPC_JAGAR_HIO_CLASS* mpHIO;
    /* 0x0E44 */ dCcD_Cyl mCyl1;
    /* 0x0F80 */ u8 mType;
    /* 0x0F84 */ daNpcT_ActorMngr_c mActorMngr[5];
    /* 0x0FAC */ daNpcT_Path_c field_0xfac;
    /* 0x0FD4 */ actionFunc field_0xfd4;
    /* 0x0FE0 */ actionFunc field_0xfe0;
    /* 0x0FEC */ int field_0xfec;
    /* 0x0FF0 */ int field_0xff0;
    /* 0x0FF4 */ int field_0xff4;
    /* 0x0FF8 */ int field_0xff8;
    /* 0x0FFC */ int field_0xffc;
    /* 0x1000 */ u8 field_0x1000;
    /* 0x1001 */ u8 field_0x1001;
    /* 0x1002 */ u8 field_0x1002;
    /* 0x1003 */ u8 field_0x1003;
    /* 0x1004 */ u8 field_0x1004;
    /* 0x1005 */ u8 mListen;
    /* 0x1008 */ int field_0x1008;
};

STATIC_ASSERT(sizeof(daNpc_Jagar_c) == 0x100c);

#endif /* D_A_NPC_JAGAR_H */
