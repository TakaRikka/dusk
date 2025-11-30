#ifndef D_A_E_PO_H
#define D_A_E_PO_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"


/**
 * @ingroup actors-enemies
 * @class e_po_class
 * @brief Poe
 *
 * @details
 *
 */

enum daE_PO_ANM {
    ANM_ATTACK = 5,
    ANM_AWAKE = 6,
    ANM_DAMAGE = 7,
    ANM_DOWN = 8,
    ANM_DOWN_DAMAGE = 9,
    ANM_DOWN_DEAD = 10,
    ANM_DOWN_WAIT = 11,
    ANM_FIRE_CATCH = 12,
    ANM_HANGED = 13,
    ANM_HANGED_BRUSH = 14,
    ANM_HANGED_DAMAGE = 15,
    ANM_HANGED_WAIT = 16,
    ANM_POSSESS = 17,
    ANM_RUN_AFTER = 18,
    ANM_RUN_AFTER2 = 19,
    ANM_SWAY_BACK = 20,
    ANM_WAIT = 21,
    ANM_WAIT02 = 22,
};

enum Action_e {
    ACT_WAIT = 0,
    ACT_AVOID = 1,
    ACT_OPENING = 2,
    ACT_SEARCH = 3,
    ACT_ATTACK = 4,
    ACT_LIMBERING = 5,
    ACT_ROLL_MOVE = 6,
    ACT_DAMAGE = 10,
    ACT_WOLF_BITE = 11,
    ACT_DEAD = 12,
    ACT_HOLL_DEMO = 20,
};

class e_po_class {
public:
    e_po_class();

    /* 0x000 */ fopEn_enemy_c enemy;
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ int mType;
    /* 0x5B8 */ u32 field_0x5B8;
    /* 0x5BC */ u8 mArg0;
    /* 0x5BD */ u8 BitSW;
    /* 0x5BE */ u8 BitSW2;
    /* 0x5BF */ u8 BitSW3;
    /* 0x5C0 */ bool field_0x5C0;
    /* 0x5C1 */ bool field_0x5C1;
    /* 0x5C2 */ u8 field_0x5C2[0x5D4 - 0x5C2];  // Padding
    /* 0x5D4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5D8 */ mDoExt_McaMorfSO* mpMorf3;
    /* 0x5DC */ mDoExt_invisibleModel mInvModel;
    /* 0x5E4 */ int mAnmID;
    /* 0x5E8 */ J3DModel* mpModel;
    /* 0x5EC */ J3DModel* mpModel2;
    /* 0x5F0 */ mDoExt_McaMorf* mpMorf2;
    /* 0x5F4 */ f32 field_0x5F4;
    /* 0x5F8 */ Z2CreatureEnemy mSound1;
    /* 0x69C */ Z2CreatureEnemy mSound2;
    /* 0x740 */ s16 field_0x740;
    /* 0x742 */ s16 mActionID;
    /* 0x744 */ f32 field_0x744;
    /* 0x748 */ u8 field_0x748[0x74A - 0x748];  // Padding
    /* 0x74A */ s16 field_0x74A[5];
    /* 0x754 */ s16 field_0x754;
    /* 0x756 */ u8 field_0x756;
    /* 0x757 */ s8 field_0x757;
    /* 0x758 */ u8 field_0x758;
    /* 0x759 */ u8 field_0x759;
    /* 0x75A */ u8 field_0x75A;
    /* 0x75B */ u8 field_0x75B;
    /* 0x75C */ u32 field_0x75C;
    /* 0x760 */ u8 field_0x760;
    /* 0x762 */ s16 field_0x762;
    /* 0x764 */ csXyz field_0x764;
    /* 0x76C */ f32 field_0x76C;
    /* 0x770 */ cXyz field_0x770;  // From here on, a lot of joint angles and position vectors...
    /* 0x77C */ cXyz field_0x77C;
    /* 0x788 */ cXyz field_0x788;
    /* 0x794 */ cXyz field_0x794;
    /* 0x7A0 */ cXyz field_0x7A0;
    /* 0x7AC */ cXyz field_0x7AC;
    /* 0x7B8 */ cXyz field_0x7B8;
    /* 0x7C4 */ f32  field_0x7C4;  // Linked to lamp anim after death, flame spawn height above ground ?
    /* 0x7C8 */ u8 field_0x7C8[0x7D0 - 0x7C8];  // Padding
    /* 0x7D0 */ f32 field_0x7D0;
    /* 0x7D4 */ f32 field_0x7D4;
    /* 0x7D8 */ f32 field_0x7D8;  // FovY camera ?
    /* 0x7DC */ bool field_0x7DC;
    /* 0x7DD */ bool field_0x7DD;
    /* 0x7DE */ s16 field_0x7DE;  // Amount of times bitten by wolf
    /* 0x7E0 */ s16 field_0x7E0;
    /* 0x7E2 */ s16 field_0x7E2;
    /* 0x7E4 */ s16 field_0x7E4;
    /* 0x7E6 */ csXyz field_0x7E6[4];
    /* 0x7FE */ u8 field_0x7FE;
    /* 0x800 */ f32 field_0x800;
    /* 0x804 */ f32 field_0x804;
    /* 0x808 */ f32 field_0x808;
    /* 0x80C */ f32 field_0x80C;
    /* 0x810 */ s16 field_0x810;
    /* 0x812 */ s8 field_0x812;
    /* 0x814 */ cXyz field_0x814;
    /* 0x820 */ cXyz field_0x820;
    /* 0x82C */ cXyz field_0x82C;
    /* 0x838 */ cXyz field_0x838;  // Eye pointer, camera ?
    /* 0x844 */ cXyz field_0x844;  // Center pointer, camera ?
    /* 0x850 */ dBgS_AcchCir mAcchCir;
    /* 0x890 */ dBgS_ObjAcch mAcch;
    /* 0xA68 */ s8 field_0xA68;
    /* 0xA6C */ dCcD_Stts mColliderStts;
    /* 0xAA8 */ dCcD_Cyl mCyl;
    /* 0xBE4 */ dCcD_Sph mSph;
    /* 0xD1C */ dCcD_Sph mSph2;
    /* 0xE54 */ dCcU_AtInfo mAtInfo;
    /* 0xE78 */ u32 mParticleKey6;
    /* 0xE7C */ u32 mParticleKey5[2];
    /* 0xE84 */ u32 mParticleKeys[12];
    /* 0xEB4 */ dPa_hermiteEcallBack_c field_0xEB4;
    /* 0xECC */ u8 field_0xECC;
    /* 0xECD */ u8 field_0xECD[0xee0 - 0xECD];  // Padding
};

STATIC_ASSERT(sizeof(e_po_class) == 0xee0);

#endif /* D_A_E_PO_H */
