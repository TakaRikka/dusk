#ifndef D_A_E_WW_H
#define D_A_E_WW_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_WW_c
 * @brief White Wolfos
 * 
 * @details 
 * 
*/

class daE_WW_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    void setHeadAngle();
    int draw();
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setActionMode(int, int);
    void damage_check();
    void setRandamNumber();
    s16 getNearPlayerAngle();
    void setGroundAngle();
    f32 checkCreateBg(cXyz);
    bool checkAttackWall();
    void setBlurEffect();
    void setAppearEffect();
    bool checkSideStep();
    void createWolf(cXyz, u8);
    void executeMaster();
    void executeWait();
    int calcJumpSpeed();
    void executeAttack();
    bool checkAttackStart();
    void executeChase();
    void executeDamage();
    bool checkMoveOut();
    void executeMoveOut();
    bool checkWalkStart();
    void executeWalk();
    void eWW_posMoveF();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    bool calcMoveDir(s16*, s16);
    int _delete();
    int CreateHeap();
    int create();

    u8 isAttack() { return field_0x756; }
    void setAttack() { mActionMode = 10; }
    f32 const getSpeedF() { return speedF; }

private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ Z2CreatureEnemy mSound;
    /* 0x65C */ cXyz field_0x65c; // Under ground position ? see jump speed calc
    /* 0x668 */ cXyz field_0x668;
    /* 0x674 */ csXyz field_0x674; // Probably csXyz
    /* 0x67A */ s16 field_0x67a; // Head angle x ? csXyz ???
    /* 0x67C */ s16 field_0x67c; // Head angle y ?
    /* 0x67E */ s16 field_0x67e; // Head angle z ?
    /* 0x680 */ fpc_ProcID mChildID[10];
    /* 0x6A8 */ f32 field_0x6a8; // Radius draw circle debug ?
    /* 0x6AC */ f32 field_0x6ac; // Off set draw circle debug ??
    /* 0x6B0 */ f32 mDistCheckModifier;
    /* 0x6B4 */ u8 field_0x6b4; // Arg0 ??
    /* 0x6B5 */ u8 field_0x6b5;
    /* 0x6B6 */ u8 field_0x6b6;
    /* 0x6B7 */ u8 field_0x6b7;
    /* 0x6B8 */ s32 mAction;
    /* 0x6BC */ s32 mActionMode; // Which wolf does action ? 1 jump ? 0 init ?, 26 Hide ? check sound action just before, check execute chase for cases in switch
    /* 0x6C0 */ int field_0x6c0; // Side step ? 0-> step left, 1 step right
    /* 0x6C4 */ s32 field_0x6c4; // Active index for the int arrays later ? Change to int ?
    /* 0x6C8 */ s32 field_0x6c8;
    /* 0x6CC */ s16 field_0x6cc; // angleY storage ?
    /* 0x6CE */ s16 field_0x6ce;
    /* 0x6D0 */ int field_0x6d0;
    /* 0x6D4 */ int field_0x6d4[10]; // Existing wolf ?
    /* 0x6FC */ int field_0x6fc[10];
    /* 0x724 */ u16 field_0x724; // Linked with getting damage to link
    /* 0x728 */ int field_0x728;
    /* 0x72C */ int field_0x72c;
    /* 0x730 */ int field_0x730;
    /* 0x734 */ int field_0x734; // Attack interval ?
    /* 0x738 */ int field_0x738;
    /* 0x73C */ int field_0x73c;
    /* 0x740 */ int field_0x740;
    /* 0x744 */ int field_0x744[2];
    /* 0x74C */ int field_0x74c;
    /* 0x750 */ s16 field_0x750[3];
    /* 0x756 */ u8 field_0x756;
    /* 0x757 */ u8 field_0x757;
    /* 0x758 */ u8 field_0x758;
    /* 0x759 */ u8 field_0x759;
    /* 0x75A */ u8 field_0x75a;
    /* 0x75B */ u8 field_0x75b;
    /* 0x75C */ u8 field_0x75c;
    /* 0x75D */ bool field_0x75d;
    /* 0x760 */ dBgS_AcchCir mAcchCir;
    /* 0x7A0 */ dBgS_ObjAcch mObjAcch;
    /* 0x994 */ dCcD_Stts mCcStts;
    /* 0x9B4 */ dCcD_Sph mSph1[2];
    /* 0xC24 */ dCcD_Sph mSph2[2];
    /* 0xE94 */ dCcU_AtInfo mAtInfo;
    /* 0xEB8 */ u8 field_0xeb8[0xec0 - 0xeb8];
    /* 0xEC0 */ u32 mParticleKey;
    /* 0xEC4 */ u8 field_0xec4;
};

STATIC_ASSERT(sizeof(daE_WW_c) == 0xec8);

#endif /* D_A_E_WW_H */
