#ifndef D_A_B_GO_H
#define D_A_B_GO_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

#define GORON_CHILD_MAX 31
#define RES_IS_MODEL 0x13

enum daB_GO_ANM {
    /* 0x04 */ ANM_ATTACK = 4,
    /* 0x05 */ ANM_DAMAGE_01,
    /* 0x06 */ ANM_DAMAGE_02,
    /* 0x07 */ ANM_DEAD_01,
    /* 0x08 */ ANM_DEAD_02,
    /* 0x09 */ ANM_FALL,
    /* 0x0A */ ANM_START_L,
    /* 0x0B */ ANM_START_R,
    /* 0x0C */ ANM_STEP,
    /* 0x0D */ ANM_TRAP,
    /* 0x0E */ ANM_WAIT_02,
    /* 0x0F */ ANM_WAIT_03,
    /* 0x10 */ ANM_WALK,
};

enum daB_GO_Action {
    /* 0x0 */ ACT_WAIT,
    /* 0x1 */ ACT_WALK,
    /* 0x2 */ ACT_ATTACK,
};

/**
 * @ingroup actors-enemies
 * @class b_go_class
 * @brief Goron Golem
 *
 * @details Unused golem boss made up of several small Gorons.
 *
 */
class b_go_class : public fopEn_enemy_c {
public:
    b_go_class();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5B8 */ Z2CreatureEnemy mSound;
    /* 0x65C */ int mAnmID;
    /* 0x660 */ u8 unk_0x660;
    /* 0x661 */ u8 field_0x661[0x668 - 0x661];
    /* 0x668 */ s16 unk_0x668;
    /* 0x66A */ s16 mActionID;
    /* 0x66C */ s16 mMode;
    /* 0x66E */ u8 field_0x66e[0x67e - 0x66e];
    /* 0x67E */ s16 mAngleToPlayer;
    /* 0x680 */ f32 mDistToPlayer;
    /* 0x684 */ u8 field_0x684[4];
    /* 0x688 */ s16 mTimers[4];
    /* 0x690 */ s16 unk_0x690;
    /* 0x692 */ s16 field_0x692;
    /* 0x694 */ dBgS_AcchCir mAcchCir;
    /* 0x6D4 */ dBgS_ObjAcch mAcch;
    /* 0x8AC */ dCcD_Stts mStts;
    /* 0x8E8 */ dCcD_Sph field_0x8e8;
    /* 0xA20 */ dCcD_Sph field_0xa20;
    /* 0xB58 */ dCcD_Cyl field_0xb58;
    /* 0xC94 */ dCcU_AtInfo mAtInfo;
    /* 0xCB8 */ u32 mGoronChildIDs[GORON_CHILD_MAX];
    /* 0xD34 */ u8 field_0xd34;
};

STATIC_ASSERT(sizeof(b_go_class) == 0xD38);

class daB_GO_HIO_c {
public:
    daB_GO_HIO_c();
    virtual ~daB_GO_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mSmallSize;
    /* 0x0C */ f32 mNormalSpeed;
    /* 0x10 */ f32 mAttackInitRange;
    /* 0x14 */ bool mDisplayModelImage;
};


#endif /* D_A_B_GO_H */
