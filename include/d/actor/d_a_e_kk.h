#ifndef D_A_E_KK_H
#define D_A_E_KK_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"


/**
 * @ingroup actors-enemies
 * @class daE_KK_c
 * @brief Chilfos (Koori no Kenshi)
 *
 * @details Enemy - Ice Knight
 *
 */
class daE_KK_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    void setWeaponBck(int, u8, f32, f32);
    bool mCutTypeCheck(int);
    void setActionMode(int, int);
    void damage_check();
    void nextActionCheck();
    bool way_gake_check();
    void mDeadEffSet(cXyz&);
    void executeWait();
    void executeIcicleWait();
    void executeWalk();
    void executeSpearThrow();
    void executeBackWalk();
    void executeYoroke();
    void executeGuard();
    void executeDamage();
    void executeAttack();
    void executeDead();
    void executeWeaponMove();
    void action();
    void mtx_set();
    void weapon_mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

private:
    /* 0x05AC */ request_of_phase_process_class mPhaseReq;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpMorfSO;
    /* 0x05B8 */ mDoExt_McaMorfSO* mpWeaponMorfSO;
    /* 0x05BC */ Z2CreatureEnemy mCreatureSound;
    /* 0x0660 */ s32 mActionMode;
    /* 0x0664 */ s32 mMoveMode;
    /* 0x0668 */ u32 mModelShadow;
    /* 0x066C */ u32 mWeaponShadow;
    /* 0x0670 */ s16 mTimer;
    /* 0x0672 */ s16 field_0x672;
    /* 0x0674 */ s16 field_0x674;
    /* 0x0676 */ s16 mDistance;
    /* 0x0678 */ u8 mDamageTimer;
    /* 0x0679 */ u8 field_0x679;
    /* 0x067A */ u8 field_0x67a;
    /* 0x067B */ u8 field_0x67b;
    /* 0x067C */ u8 field_0x67c;
    /* 0x067D */ u8 field_0x67d;
    /* 0x067E */ u8 field_0x67e;
    /* 0x0680 */ cXyz field_0x680;
    /* 0x068C */ cXyz field_0x68c;
    /* 0x0698 */ cXyz field_0x698;
    /* 0x06A4 */ cXyz field_0x6a4;
    /* 0x06B0 */ cXyz field_0x6b0[3];
    /* 0x06D4 */ cXyz field_0x6d4[3];
    /* 0x06F8 */ cXyz field_0x6f8[6];
    /* 0x0740 */ cXyz field_0x740;
    /* 0x074C */ cXyz field_0x74c;
    /* 0x0758 */ s16 field_0x758;
    /* 0x075C */ f32 mMovingRange;
    /* 0x0760 */ f32 field_0x760;
    /* 0x0764 */ s32 field_0x764;
    /* 0x0768 */ s32 field_0x768;
    /* 0x076C */ u8 field_0x76c;
    /* 0x0770 */ dBgS_AcchCir mAcchCir;
    /* 0x07B0 */ dBgS_ObjAcch mObjAcch;
    /* 0x0988 */ dCcD_Stts mStts;
    /* 0x09C4 */ dCcD_Cyl mCyl;
    /* 0x0B00 */ dCcD_Sph mSph;
    /* 0x0C38 */ dCcU_AtInfo mAtInfo;
    /* 0x0C5C */ dCcD_Sph mSpheres[6];
    /* 0x13AC */ u8 mHIOInit;
    /* 0x13AD */ u8 field_0x13ad[0x13D4 - 0x13AD];
};

STATIC_ASSERT(sizeof(daE_KK_c) == 0x13d4);

#endif /* D_A_E_KK_H */
