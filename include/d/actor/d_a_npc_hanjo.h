#ifndef D_A_NPC_HANJO_H
#define D_A_NPC_HANJO_H

#include "d/actor/d_a_npc.h"

class daNpc_HanjoStone_c {
public:
    ~daNpc_HanjoStone_c() {}
    daNpc_HanjoStone_c() {
        initialize();
    }
    cXyz getPos() { return mPos; }
    cXyz getOld() { return mOld; }
    void posMoveF(fopAc_ac_c* param_1) {
        cXyz cStack_48;
        mOld = mPos;
        if (mTmr != 0) {
            cStack_48 = field_0x1c;
            f32 dVar9 = mSpeedF * cM_ssin(mAngle.y);
            f32 dVar10 = cStack_48.y + field_0x16c;
            f32 dVar8 = mSpeedF * cM_scos(mAngle.y);
            if (dVar10 < fopAcM_GetMaxFallSpeed(param_1)) {
                dVar10 = fopAcM_GetMaxFallSpeed(param_1);
            }
            field_0x1c.set(dVar9, dVar10, dVar8);
            mPos += field_0x1c;
            mTmr--;
            if (mTmr == 0) {
                initialize();
            }
        }
    }
 
    void setPos(cXyz pos) {
        mPos = pos;
        mOld = pos;
    } 

    void setAngle(csXyz angle) { mAngle = angle; }

    dCcD_Sph* getSphP() { return &mSphere; }
    f32 getSpeedF() { return mSpeedF; }
    void setModel(J3DModel* model) { mModel = model; }
    J3DModel* getModelP() { return mModel; }

    int chkActive() {
        if (mModel != NULL) {
            return mTmr != 0;
        } 
        return FALSE;
    }

    void initialize() {
        mPos.setall(0.0f);
        mOld = mPos;
        field_0x1c.setall(0.0f);
        mAngle.setall(0);
        mSpeedF = 0;
        field_0x16c = -3.0f;
        mTmr = 0;
    }

    void setMtx(Mtx mtx) {
        if (chkActive()) {
            if (cM3d_IsZero(mSpeedF)) {
                mModel->setBaseTRMtx(mtx);
            } else {
                mDoMtx_stack_c::transS(mPos);
                mDoMtx_stack_c::ZXYrotM(mAngle);
                mModel->setBaseTRMtx(mDoMtx_stack_c::get());
            }
        }
    }

    void setTmr(int tmr) { mTmr = tmr; }
    void setVSpeed(f32 speed) { field_0x1c.y = speed; }
    void setSpeedF(f32 speed) { mSpeedF = speed; }

    /* 0x000 */ J3DModel* mModel;
    /* 0x004 */ cXyz mOld;
    /* 0x010 */ cXyz mPos;
    /* 0x01C */ cXyz field_0x1c;
    /* 0x028 */ csXyz mAngle;
    /* 0x030 */ dCcD_Sph mSphere;
    /* 0x168 */ f32 mSpeedF;
    /* 0x16C */ f32 field_0x16c;
    /* 0x170 */ int mTmr;
};

/**
 * @ingroup actors-npcs
 * @class daNpc_Hanjo_c
 * @brief Hanch
 *
 * @details
 *
 */
class daNpc_Hanjo_c : public daNpcT_c {
public:
    typedef int (daNpc_Hanjo_c::*cutFunc)(int);
    typedef int (daNpc_Hanjo_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
    };

    ~daNpc_Hanjo_c();
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
    void afterMoved();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawOtherMdl();
    int selectAction();
    int chkAction(int (daNpc_Hanjo_c::*)(void*));
    int setAction(int (daNpc_Hanjo_c::*)(void*));
    void initShoot(int);
    int shoot(int);
    void initDive();
    void dive();
    int lookround(s16);
    int commandToHawk();
    int cutConversationAboutSaru(int);
    int cutConversation(int);
    int cutPursuitBee(int);
    int cutAppearHawker(int);
    int cutDive(int);
    int wait(void*);
    int throwStone(void*);
    int takayose(void*);
    int talk(void*);
    daNpc_Hanjo_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                 daNpcT_evtData_c const* param_7, char** param_8) :
                                 daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                  {}
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    int checkChangeJoint(int param_1) { return param_1 == 4; }
    int checkRemoveJoint(int param_1) { return param_1 == 8; }
    s32 getFootLJointNo() { return 22; }
    s32 getFootRJointNo() { return 26; }
    MtxP getArmLMtx() { return mpMorf[0]->getModel()->getAnmMtx(11); }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }

    static dCcD_SrcGObjInf const mStoneCcDObjInfo;
    static char* mCutNameList[6];
    static cutFunc mCutList[6];
    static dCcD_SrcSph mStoneCcDSph;
private:
    /* 0x0E40 */ int field_0x0E40;
    /* 0x0E44 */ J3DModel* mModel1;
    /* 0x0E48 */ J3DModel* mModel2;
    /* 0x0E4C */ dCcD_Cyl mCyl1;
    /* 0x0F88 */ dCcD_Cyl mCyl2;
    /* 0x10C4 */ u8 mType;
    /* 0x10C8 */ daNpcT_ActorMngr_c field_0x10c8[4];
    /* 0x10E8 */ daNpcT_Path_c field_0x10e8;
    /* 0x1110 */ daNpc_HanjoStone_c mStones[4];
    /* 0x16E0 */ actionFunc field_0x16e0;
    /* 0x16EC */ actionFunc field_0x16ec;
    /* 0x16F8 */ cXyz field_0x16f8;
    /* 0x1704 */ int field_0x1704;
    /* 0x1708 */ int field_0x1708;
    /* 0x170C */ int field_0x170c;
    /* 0x1710 */ int field_0x1710;
    /* 0x1714 */ int field_0x1714;
    /* 0x1718 */ f32 field_0x1718;
    /* 0x171C */ s16 field_0x171c;
    /* 0x171E */ u8 field_0x171e;
    /* 0x171F */ u8 field_0x171f;
    /* 0x1720 */ u8 field_0x1720;
    /* 0x1721 */ u8 field_0x1721;
    /* 0x1722 */ u8 field_0x1722;
    /* 0x1723 */ u8 field_0x1723;
    /* 0x1724 */ u8 field_0x1724;
    /* 0x1728 */ int field_0x1728;
};

STATIC_ASSERT(sizeof(daNpc_Hanjo_c) == 0x172c);

class daNpc_Hanjo_Param_c {
public:
    virtual ~daNpc_Hanjo_Param_c() {}

    struct Data {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0c;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1c;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2c;
        /* 0x30 */ f32 field_0x30;
        /* 0x34 */ f32 field_0x34;
        /* 0x38 */ f32 field_0x38;
        /* 0x3C */ f32 field_0x3c;
        /* 0x40 */ f32 field_0x40;
        /* 0x44 */ f32 field_0x44;
        /* 0x48 */ s16 field_0x48;
        /* 0x4A */ s16 field_0x4a;
        /* 0x4C */ s16 field_0x4c;
        /* 0x4E */ s16 field_0x4e;
        /* 0x50 */ f32 field_0x50;
        /* 0x54 */ f32 field_0x54;
        /* 0x58 */ f32 field_0x58;
        /* 0x5C */ f32 field_0x5c;
        /* 0x60 */ s16 field_0x60;
        /* 0x62 */ s16 field_0x62;
        /* 0x64 */ int field_0x64;
        /* 0x68 */ int field_0x68;
        /* 0x6C */ f32 field_0x6c;
        /* 0x70 */ f32 field_0x70;
        /* 0x74 */ f32 field_0x74;
        /* 0x78 */ f32 field_0x78;
        /* 0x7C */ f32 field_0x7c;
        /* 0x80 */ f32 field_0x80;
        /* 0x84 */ f32 field_0x84;
        /* 0x88 */ f32 field_0x88;
        /* 0x8C */ f32 field_0x8c;
        /* 0x90 */ f32 field_0x90;
        /* 0x94 */ f32 field_0x94;
        /* 0x98 */ f32 field_0x98;
        /* 0x9C */ f32 field_0x9c;
        /* 0xA0 */ f32 field_0xa0;
        /* 0xA4 */ f32 field_0xa4;
        /* 0xA8 */ f32 field_0xa8;
        /* 0xAC */ s16 field_0xac;
        /* 0xAE */ s16 field_0xae;
        /* 0xB0 */ s16 field_0xb0;
        /* 0xB2 */ s16 field_0xb2;
    };

    static const Data m;
};

#endif /* D_A_NPC_HANJO_H */
