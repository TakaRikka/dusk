#ifndef D_A_NPC_GRA_H
#define D_A_NPC_GRA_H

#include "d/actor/d_a_npc4.h"
#include "d/d_particle_copoly.h"


class daNpc_grA_HIOParam {
public:
    /* 0x00 */ daNpcF_HIOParam mNpcFParams;
    /* 0x6C */ s16 mBowTimer;
    /* 0x70 */ f32 mRotationalSpeed;
    /* 0x74 */ f32 mWalkingSpeed;
    /* 0x78 */ f32 mWalkingAnimationSpeed;
    /* 0x7C */ s16 mWalkingRotationSpeed;
    /* 0x7A */ s16 mWalkingRotationDivisions;
    /* 0x80 */ f32 mParticleSize;
};

class daNpc_grA_Param_c {
public:
    virtual ~daNpc_grA_Param_c(){};

    static daNpc_grA_HIOParam const m;
};

#if DEBUG
class daNpc_grA_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_grA_HIO_c();
#if DEBUG
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
#endif
    daNpc_grA_HIOParam mHioParams;
};

#define NPC_GRA_HIO_CLASS daNpc_grA_HIO_c

#else

#define NPC_GRA_HIO_CLASS daNpc_grA_Param_c

#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_grA_c
 * @brief Goron (Adult)
 *
 * @details
 *
 */
class daNpc_grA_c : public daNpcF_c {
public:
    class daNpc_GrA_prtclMngr_c {
    public:
        /* 0x00 */ u8 field_0x0;
        /* 0x04 */ cXyz mPos;
        /* 0x10 */ csXyz mAngle;
        /* 0x18 */ cXyz mScale;
        /* 0x24 */ dPaPo_c mManager;
    };

    daNpc_grA_c();
    ~daNpc_grA_c();
    BOOL create();
    BOOL CreateHeap();
    BOOL Delete();
    BOOL Execute();
    BOOL Draw();
    BOOL ctrlJoint(J3DJoint*, J3DModel*);
    static BOOL createHeapCallBack(fopAc_ac_c*);
    static BOOL ctrlJointCallBack(J3DJoint*, int);
    BOOL setTagJump();
    BOOL setTagJump2();
    BOOL setHomeJump();
    bool checkTagGraSub(fopAc_ac_c*);
    bool checkTagGraSub2(fopAc_ac_c*);
    bool checkTagGraSubCrashed(fopAc_ac_c*);
    void setParam();
    BOOL main();
    BOOL ctrlBtk();
    void setAttnPos();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setExpression(int, f32);
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    BOOL drawDbgInfo();
    void drawOtherMdls();
    u8 getMode();
    u8 getMode1();
    u8 getTypeFromParam();
    u8 getSwBit();
    BOOL isDelete();
    void reset();
    void playExpression();
    void playMotion();
    BOOL chkAction(int (daNpc_grA_c::*)(void*));
    BOOL setAction(int (daNpc_grA_c::*)(void*));
    BOOL selectAction();
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    BOOL chkFindPlayer();
    void setExpressionTalkAfter();
    BOOL setPrtcl();
    void setRollPrtcl(cXyz const&, f32);
    void setOtherObjMtx();
    BOOL ECut_talkSpa(int);
    BOOL ECut_grDSRoll(int);
    BOOL ECut_grDSGate(int);
    BOOL ECut_teachElevator(int);
    BOOL ECut_kickOut(int);
    BOOL ECut_noneLook(int);
    BOOL ECut_rescueRock(int);
    BOOL ECut_carrySpaWater(int);
    BOOL ECut_carrySpaWaterFailure(int);
    BOOL ECut_rollRockCrash(int);
    BOOL ECut_talkSpaWater(int);
    BOOL wait(void*);
    BOOL waitTW(void*);
    BOOL waitCheer(void*);
    BOOL waitGate(void*);
    BOOL waitKickOut(void*);
    BOOL moveRock(void*);
    BOOL spaWarm(void*);
    BOOL waitSpaWater(void*);
    BOOL waitSpaBuyer(void*);
    BOOL waitSpaBuyerTW(void*);
    BOOL beforeCrashWait(void*);
    BOOL crashRoll(void*);
    BOOL crashRollWait(void*);
    BOOL crashedWait(void*);
    BOOL waitBuyer(void*);
    BOOL talk(void*);
    BOOL test(void*);
    virtual void adjustShapeAngle() {}

    int getType() { return mType; }
    s16 getGraNo() { return 0; }
    void addCarryNum() { field_0x1692++; }
    u8 getPathNoFromParam() { return home.angle.z; }
    void setGateWalk() { field_0x14D0 = 1; }
    static char* mEvtCutNameList[12];
    typedef BOOL (daNpc_grA_c::*cut_type)(int);
    static cut_type mEvtCutList[];

private:
    typedef BOOL (daNpc_grA_c::*daNpc_grA_c_Action)(void*);
    
    /* 0x0B48 */ Z2Creature mCreature;
    /* 0x0BD8 */ J3DModel* mpModel;
    /* 0x0BDC */ int field_0xBDC;
    /* 0x0BE0 */ daNpcF_Lookat_c mNpcfLookAt;
    /* 0x0C7C */ daNpcF_ActorMngr_c mNpcfActorManager[3];
    /* 0x0C94 */ NPC_GRA_HIO_CLASS* mpHio;
    /* 0x0C98 */ dCcD_Cyl field_0xC98;
    /* 0x0DD4 */ daNpcF_Path_c field_0xDD4;
    /* 0x1404 */ daNpc_grA_c_Action mAction2;
    /* 0x1410 */ daNpc_grA_c_Action mAction;
    /* 0x141C */ request_of_phase_process_class mPhase[8];
    /* 0x145C */ int field_0x145C;
    /* 0x14B0 */ int field_0x1460;
    /* 0x1464 */ int field_0x1464;
    /* 0x1468 */ int field_0x1468;
    /* 0x146C */ int field_0x146C;
    /* 0x1470 */ s16 mLookMode;
    /* 0x1472 */ u16 field_0x1472;
    /* 0x1474 */ u8 mType;
    /* 0x1478 */ int field_0x1478;
    /* 0x147C */ const char* mName;
    /* 0x1480 */ fpc_ProcID field_0x1480;
    /* 0x1484 */ s16 field_0x1484;
    /* 0x1486 */ u8 field_0x1486;
    /* 0x1487 */ u8 field_0x1487;
    /* 0x1488 */ u8 field_0x1488;
    /* 0x1489 */ u8 field_0x1489;
    /* 0x148C */ cXyz field_0x148C;
    /* 0x1498 */ csXyz field_0x1498;
    /* 0x149E */ u8 field_0x149E;
    /* 0x14A0 */ cXyz field_0x14A0;
    /* 0x14AC */ csXyz field_0x14AC;
    /* 0x14B4 */ f32 field_0x14B4;
    /* 0x14B8 */ f32 field_0x14B8;
    /* 0x14BC */ cXyz field_0x14BC;
    /* 0x14C8 */ csXyz field_0x14C8;
    /* 0x14CE */ u8 mMode;
    /* 0x14CF */ u8 mSwBit;
    /* 0x14D0 */ u8 field_0x14D0;
    /* 0x14D1 */ u8 field_0x14D1;
    /* 0x14D4 */ f32 field_0x14D4;
    /* 0x14D8 */ int field_0x14D8;
    /* 0x14DC */ Mtx field_0x14DC;
    /* 0x150C */ u8 field_0x150C;
    /* 0x1510 */ int mParticleID[3];
    /* 0x151C */ int field_0x151C;
    /* 0x1520 */ daNpc_GrA_prtclMngr_c mParticleManager[4];
    /* 0x1690 */ u8 field_0x1690;
    /* 0x1691 */ u8 field_0x1691;
    /* 0x1692 */ u8 field_0x1692;
    /* 0x1692 */ u8 field_0x1693;
    /* 0x1692 */ s16 field_0x1694;

};

STATIC_ASSERT(sizeof(daNpc_grA_c) == 0x1698);


#endif /* D_A_NPC_GRA_H */
