#ifndef D_A_E_AI_H
#define D_A_E_AI_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"


/**
 * @ingroup actors-enemies
 * @class e_ai_class
 * @brief Armos
 *
 * @details
 *
 */
class e_ai_class : public fopEn_enemy_c {
public:
    enum action_e {
        ACTION_WAIT,
        ACTION_MOVE,
        ACTION_ATTACK,
        ACTION_DAMAGE,
        ACTION_RETURN,
    };

    void initCcCylinder();
    void setCcCylinder();
    void anm_init(int, f32, u8, f32);
    int Draw();
    BOOL player_way_check();
    BOOL other_bg_check(fopAc_ac_c*);
    BOOL pl_check(f32, s16);
    void damage_check();
    void e_ai_damage();
    void e_ai_attack();
    void e_ai_move();
    void e_ai_wait();
    void e_ai_return();
    void action();
    void PlayerWaySet();
    void AttentionSet();
    int Execute();
    void setBaseMtx();
    int Delete();
    int CreateHeap();
    int Create();

private:
    /* 0x5AC */ JPABaseEmitter* mpEmitter;
    /* 0x5B0 */ request_of_phase_process_class m_phase;
    /* 0x5B8 */ u8 field_0x5b8;
    /* 0x5B9 */ u8 field_0x5b9;
    /* 0x5BA */ u8 field_0x5ba;
    /* 0x5BC */ cXyz unk_0x5bc;
    /* 0x5C8 */ mDoExt_brkAnm* m_brk;
    /* 0x5CC */ u8 field_0x5cc;
    /* 0x5D0 */ mDoExt_McaMorfSO* m_modelMorf;
    /* 0x5D4 */ Z2CreatureEnemy m_sound;
    /* 0x678 */ u8 field_0x678[0x67C - 0x678];
    /* 0x67C */ s8 field_0x67c;
    /* 0x680 */ u32 m_shadowKey;
    /* 0x684 */ int m_anm;
    /* 0x688 */ u8 field_0x688[0x68C - 0x688];
    /* 0x68C */ s16 m_lifetime;
    /* 0x68E */ s16 m_action;
    /* 0x690 */ s16 m_mode;
    /* 0x692 */ s16 field_0x692;
    /* 0x694 */ s16 field_0x694;
    /* 0x696 */ s16 field_0x696;
    /* 0x698 */ u8 m_swbit;
    /* 0x69C */ cXyz unk_0x69c;
    /* 0x6A8 */ s16 field_0x6a8;
    /* 0x6AA */ s16 m_angleToPlayer;
    /* 0x6AC */ f32 m_playerDist;
    /* 0x6B0 */ s16 m_timers[4];
    /* 0x6B8 */ s16 m_invulnerabilityTimer;
    /* 0x6BA */ s16 field_0x6ba;
    /* 0x6BC */ s16 field_0x6bc;
    /* 0x6C0 */ f32 field_0x6c0;
    /* 0x6C4 */ u8 field_0x6C4[0x6C8 - 0x6C4];
    /* 0x6C8 */ dBgS_AcchCir m_acchcir;
    /* 0x708 */ dBgS_ObjAcch m_acch;
    /* 0x8E0 */ dCcD_Stts m_ccAtStts;
    /* 0x91C */ dCcD_Stts m_ccShieldStts;
    /* 0x958 */ dCcD_Sph m_ccAtSph;
    /* 0xA90 */ dCcD_Sph m_ccShieldSph;
    /* 0xBC8 */ dCcD_Cyl m_ccCyl;
    /* 0xD04 */ dCcU_AtInfo m_atInfo;
    /* 0xD28 */ s8 field_0xd28;
    /* 0xD29 */ s8 m_hitCount;
    /* 0xD2C */ u32 field_0xd2c;
    /* 0xD30 */ u32 field_0xd30;
    /* 0xD34 */ u32 field_0xd34;
    /* 0xD38 */ u32 field_0xd38;
    /* 0xD3C */ u8 field_0xD3C[0xD40 - 0xD3C];
    /* 0xD40 */ u8 m_HIOInit;
};

STATIC_ASSERT(sizeof(e_ai_class) == 0xd44);

#endif /* D_A_E_AI_H */
