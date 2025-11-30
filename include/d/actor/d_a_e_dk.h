#ifndef D_A_E_DK_H
#define D_A_E_DK_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_DK_c
 * @brief Bari
 *
 * @details
 *
 */
class daE_DK_c : public fopEn_enemy_c {
public:
    int draw();
    void setBck(int i_resIndex, u8 i_attr, f32 i_morf, f32 i_rate);
    void setBckCore(int i_resIndex, u8 i_attr, f32 i_morf, f32 i_rate);
    void setActionMode(int i_actionMode, int i_moveMode);
    void damage_check();
    bool checkPlayerSearch();
    int checkPlayerAttack(f32);
    void checkWaterHeight();
    static void SphBgcCallBack(dBgS_SphChk*, cBgD_Vtx_t*, int, int, int, cM3dGPla*, void*);
    void setElectricEffect();
    void setCoreDeadEffect();
    void setBodyDeadEffect();
    void executeWait();
    void executeChase();
    void executeAttack();
    void executeDamage();
    void BodyDeathMove();
    void executeDeath();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();
    daE_DK_c();

private:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorfSO;
    /* 0x5B8 */ mDoExt_McaMorfSO* mpCoreMorfSO;
    /* 0x5BC */ mDoExt_brkAnm* mpBrkAnm[2];
    /* 0x5C4 */ mDoExt_btkAnm* mpBtkAnm[2];
    /* 0x5CC */ Z2CreatureEnemy mCreatureSound;
    /* 0x670 */ cXyz field_0x670;
    /* 0x67C */ s32 mActionMode;
    /* 0x680 */ s32 mMoveMode;
    /* 0x684 */ u32 mShadowKey;
    /* 0x688 */ u32 mShadowKey2;
    /* 0x68C */ s16 field_0x68c;
    /* 0x68E */ s16 field_0x68e;
    /* 0x690 */ u8 field_0x690;
    /* 0x691 */ u8 field_0x691;
    /* 0x692 */ u8 field_0x692;
    /* 0x694 */ u16 field_0x694;
    /* 0x698 */ s32 field_0x698;
    /* 0x69C */ s32 field_0x69c;
    /* 0x6A0 */ u8 field_0x6a0;
    /* 0x6A1 */ u8 field_0x6a1;
    /* 0x6A2 */ u8 field_0x6a2;
    /* 0x6A3 */ u8 field_0x6a3;
    /* 0x6A4 */ u8 field_0x6a4;
    /* 0x6A8 */ f32 field_0x6a8;
    /* 0x6AC */ f32 field_0x6ac;
    /* 0x6B0 */ f32 field_0x6b0;
    /* 0x6B4 */ f32 field_0x6b4;
    /* 0x6B8 */ f32 field_0x6b8;
    /* 0x6BC */ dBgS_AcchCir mAcchCir;
    /* 0x6FC */ dBgS_ObjAcch mObjAcch;
    /* 0x8D4 */ dBgS_SphChk mSphereChk;
    /* 0x928 */ dCcD_Stts mStts;
    /* 0x964 */ dCcD_Sph mSphere;
    /* 0xA9C */ dCcD_Sph mAtSphere;
    /* 0xBD4 */ dCcD_Sph mCoreSphere;
    /* 0xD0C */ dCcU_AtInfo mAtInfo;
    /* 0xD30 */ u32 mElectricEffects[6];
    /* 0xD48 */ u32 mDeadEffects[2];
    /* 0xD50 */ u8 mHIOInit;
};

STATIC_ASSERT(sizeof(daE_DK_c) == 0xd54);

class daE_DK_HIO_c : public JORReflexible {
public:
    daE_DK_HIO_c();
    virtual ~daE_DK_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 player_detection_range;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 first_attack_range;
};

#endif /* D_A_E_DK_H */
