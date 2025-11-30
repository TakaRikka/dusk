#ifndef D_A_E_BG_H
#define D_A_E_BG_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_BG_c
 * @brief Bomb Fish
 *
 * @details
 *
 */
class daE_BG_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    void setSparkEffect();
    fopAc_ac_c* search_esa();
    void executeBorn();
    void executeSwim();
    void executeAttack();
    void executeDamage();
    bool setBombCarry(int);
    void executeBomb();
    void executeBirth();
    void executeHook();
    void executeEat();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    void setBgId(u32 i_bgId) { mBgId = i_bgId; }
    bool isBomb() { return mIsBomb; }

private:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorfSO;
    /* 0x5B8 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x5BC */ Z2CreatureEnemy mCreatureSound;
    /* 0x660 */ cXyz field_0x660;
    /* 0x66C */ u32 mBgId;
    /* 0x670 */ s32 mActionMode;
    /* 0x674 */ s32 mMoveMode;
    /* 0x678 */ u32 mShadowKey;
    /* 0x67C */ u8 field_0x67c[0x684 - 0x67C];
    /* 0x684 */ f32 field_0x684;
    /* 0x688 */ f32 field_0x688;
    /* 0x68C */ u8 field_0x68c;
    /* 0x68D */ u8 field_0x68d;
    /* 0x68E */ u8 field_0x68e;
    /* 0x68F */ u8 field_0x68f;
    /* 0x690 */ s32 field_0x690;
    /* 0x694 */ s32 field_0x694;
    /* 0x698 */ s16 field_0x698;
    /* 0x69A */ s16 field_0x69a;
    /* 0x69C */ s16 field_0x69c;
    /* 0x69E */ s16 field_0x69e;
    /* 0x6A0 */ s16 field_0x6a0;
    /* 0x6A2 */ s16 field_0x6a2;
    /* 0x6A4 */ s16 field_0x6a4[3];
    /* 0x6AA */ s16 field_0x6aa;
    /* 0x6AC */ s16 field_0x6ac;
    /* 0x6AE */ u8 field_0x6ae;
    /* 0x6AF */ u8 field_0x6af;
    /* 0x6B0 */ u8 field_0x6b0;
    /* 0x6B1 */ bool mIsBomb;
    /* 0x6B2 */ u8 field_0x6b2[0x6B4 - 0x6B2];
    /* 0x6B4 */ dBgS_AcchCir mAcchCir;
    /* 0x6F4 */ dBgS_ObjAcch mObjAcch;
    /* 0x8CC */ dCcD_Stts mStts;
    /* 0x908 */ dCcD_Sph mSphere;
    /* 0xA40 */ dCcD_Sph mAtSphere;
    /* 0xB78 */ dCcU_AtInfo mAtInfo;
    /* 0xB9C */ u32 mParticle[5];
    /* 0xBB0 */ u32 mParticle2;
    /* 0xBB4 */ u32 mParticle3;
    /* 0xBB8 */ u8 mHIOInit;
};

STATIC_ASSERT(sizeof(daE_BG_c) == 0xbbc);

class daE_BG_HIO_c : public JORReflexible {
public:
    daE_BG_HIO_c();
    virtual ~daE_BG_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mTrackingSpeed;
    /* 0x0C */ f32 mRushSpeed;
    /* 0x10 */ f32 mWaitDistanceBeforeCharging;
    /* 0x14 */ f32 mPlayerSearchDistance;
    /* 0x18 */ f32 mAttackRange;
    /* 0x1C */ f32 mSwimRange;
    /* 0x20 */ f32 mJumpTime;
};

#endif /* D_A_E_BG_H */
