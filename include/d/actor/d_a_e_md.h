#ifndef D_A_E_MD_H
#define D_A_E_MD_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class daE_MD_c
 * @brief Suit of Armor
 * 
 * @details 
 * 
 */
class daE_MD_c : public fopEn_enemy_c {
public:
    void initCcCylinder();
    void setCcCylinder(f32);
    void SetAnm(mDoExt_McaMorfSO*, int, int, f32, f32);
    void At_Check();
    void CheckHit();
    void WaitAction();
    void HalfBreakAction();
    bool VibAction();
    void DummyAction();
    void RealAction();
    void Action();
    int Execute();
    int Delete();
    void setBaseMtx();
    int create();

    inline int CreateHeap();
    inline int Draw();

    /* 0x5AC */ int mAction;
    /* 0x5B0 */ int mType;
    /* 0x5B4 */ int mInvulnerabilityTimer;
    /* 0x5B8 */ int mAnm;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ u8 field_0x5C0[0x5C8 - 0x5C0];
    /* 0x5C8 */ s16 field_0x5c8;
    /* 0x5CA */ s16 field_0x5ca;
    /* 0x5CC */ s16 field_0x5cc;
    /* 0x5CE */ s16 field_0x5ce;
    /* 0x5D0 */ u8 mSwbit;
    /* 0x5D4 */ int mBreakTimer;
    /* 0x5D8 */ u8 mHalfBreakMode;
    /* 0x5D9 */ u8 mCanCreateBu;
    /* 0x5DC */ request_of_phase_process_class mPhase;
    /* 0x5E4 */ Z2CreatureEnemy mSound;
    /* 0x688 */ u32 mShadowId;
    /* 0x68C */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x690 */ mDoExt_McaMorfSO* mpYariModelMorf;
    /* 0x694 */ J3DModel* mpTbModel;
    /* 0x698 */ dBgS_AcchCir mAcchCir;
    /* 0x6D8 */ dBgS_ObjAcch mAcch;
    /* 0x8B0 */ dCcD_Stts mCcStts;
    /* 0x8EC */ dCcD_Cyl mCyl;
    /* 0xA28 */ dCcU_AtInfo mAtInfo;
    /* 0xA4C */ u8 field_0xa4c[0xA50 - 0xA4C];
};

STATIC_ASSERT(sizeof(daE_MD_c) == 0xa50);


#endif /* D_A_E_MD_H */
