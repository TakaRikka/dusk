#ifndef D_A_E_SB_H
#define D_A_E_SB_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_SB_c
 * @brief Shell Blade
 * 
 * @details 
 * 
 */
class daE_SB_c : public fopEn_enemy_c {
public:
    int CreateHeap();
    void initCcCylinder();
    void setCcCylinder();
    int ctrlJoint(J3DJoint*, J3DModel*);
    void Particle_Set(u16);
    void SetAnm(int, int, f32, f32);
    void MemberClear();
    void Shield_Motion();
    void AttackStop();
    void AttackSetSP();
    void AttackAngle();
    void AttackSpeedMotion();
    void Attack_Motion();
    void Bomb_Death_Motion();
    void BombJumpSet();
    void GetHomeSet();
    void HomeJumpMotion();
    void Jump_Normal_Set();
    void Jump_Set();
    void JumpMiddleMotion();
    void Jump_Motion();
    void Death_Motion();
    void Hanekaeri();
    void Hit();
    void ActionCheck();
    void Search();
    void Wait_Motion();
    void Attack();
    void Shield();
    void Anm_Motion();
    void Search_Bomb();
    void ChanceMotion();
    void NoChanceMotion();
    void Obj_Damage();
    void HashiraVib();
    void Yazirushi();
    void setGroundAngle();
    int Execute();
    int Draw();
    int Delete();
    void setBaseMtx();
    cPhs__Step Create();
private:
    /* 0x5AC */ int field_0x5ac;
    /* 0x5B0 */ int field_0x5b0;
    /* 0x5B4 */ int field_0x5b4;
    /* 0x5B8 */ int field_0x5b8;
    /* 0x5BC */ int field_0x5bc;
    /* 0x5C0 */ int field_0x5c0;
    /* 0x5C4 */ int field_0x5c4;
    /* 0x5C8 */ int field_0x5c8;
    /* 0x5CC */ u32 field_0x5cc;
    /* 0x5D0 */ u8 field_0x5d0;
    /* 0x5D2 */ s16 field_0x5d2;
    /* 0x5D1 */ f32 field_0x5d4;
    /* 0x5D8 */ csXyz field_0x5d8;
    /* 0x5DE */ csXyz field_0x5de;
    /* 0x5E4 */ u8 field_0x5e4;
    /* 0x5E6 */ s16 field_0x5e6;
    /* 0x5E8 */ u8 field_0x5e8;
public:
    /* 0x5EC */ cXyz* field_0x5ec;
    /* 0x5F0 */ f32 field_0x5f0;
private:
    /* 0x5F4 */ cXyz field_0x5f4;
    /* 0x600 */ s16 field_0x600;
    /* 0x602 */ u8 field_0x602;
    /* 0x603 */ u8 field_0x603;
    /* 0x604 */ u8 field_0x604;
    /* 0x608 */ int field_0x608;
    /* 0x60C */ int field_0x60c;
    /* 0x610 */ s16 field_0x610;
    /* 0x612 */ s16 field_0x612;
    /* 0x614 */ csXyz field_0x614;
    /* 0x61A */ u8 field_0x61a;
    /* 0x61C */ mDoExt_McaMorfSO* mpMorf;
    /* 0x620 */ request_of_phase_process_class mPhaseReq;
    /* 0x628 */ int mShadowKey;
    /* 0x62C */ Z2CreatureEnemy mSound;
    /* 0x6D0 */ dBgS_AcchCir mAcchCir;
    /* 0x710 */ dBgS_ObjAcch mAcch;
    /* 0x8E8 */ dCcD_Stts mStts;
    /* 0x924 */ dCcD_Sph mSph;
    /* 0xA5C */ dCcD_Cyl mCyl;
    /* 0xB98 */ dCcU_AtInfo mAtInfo;
    /* 0xBBC */ u8 field_0xbbc;
};

STATIC_ASSERT(sizeof(daE_SB_c) == 0xBC0);

class daE_SB_HIO_c : public JORReflexible {
public:
    daE_SB_HIO_c();
    virtual ~daE_SB_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 search_area;
    /* 0x0C */ f32 distance_home;
    /* 0x10 */ f32 jump_xz_axis_speed;
    /* 0x14 */ f32 jump_y_axis_speed;
    /* 0x18 */ f32 atk_jump_xz_axis_speed;
    /* 0x1C */ f32 atk_y_axis_speed;
    /* 0x20 */ f32 magne_on_atk_y_axis_speed;
    /* 0x24 */ f32 grav_reduct_rate;
    /* 0x28 */ f32 other_anm_speed;
    /* 0x2C */ f32 shield_atk_anm_speed;
    /* 0x30 */ f32 death_anm_speed;
    /* 0x34 */ f32 size;
    /* 0x38 */ f32 atk_start_range;
    /* 0x3C */ s16 field_0x3c;
    /* 0x3E */ s16 field_0x3e;
};


#endif /* D_A_E_SB_H */
