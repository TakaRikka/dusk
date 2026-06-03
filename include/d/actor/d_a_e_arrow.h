#ifndef D_A_E_ARROW_H
#define D_A_E_ARROW_H

#include "d/d_com_inf_game.h"

enum e_arrow_action {
    ACTION_ARROW_SHOT,
    ACTION_ARROW_BG,
    ACTION_ARROW_SPIN,
    ACTION_ARROW_SHIELD,
    ACTION_ARROW_FIRE,
    ACTION_ARROW_BOUND,
};

enum e_arrow_type {
    ARROW_TYPE_NORMAL,
    ARROW_TYPE_FIRE,
    ARROW_TYPE_BOMB,
};

/**
 * @ingroup actors-enemies
 * @class e_arrow_class
 * @brief Enemy Arrow
 * 
 * @details 
 * 
 */
class e_arrow_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ J3DModel* mpModel;
    /* 0x5B8 */ u8 mArrowType;
    /* 0x5B9 */ u8 mFlags;
    /* 0x5BC */ char DUSK_CONST* mResName;
    /* 0x5C0 */ s16 field_0x5c0;
    /* 0x5C2 */ s16 mAction;
    /* 0x5C4 */ s16 mMode;
    /* 0x5C6 */ s16 mTimers[4];
    /* 0x5CE */ s16 field_0x5ce;
    /* 0x5D0 */ f32 field_0x5d0;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ cXyz field_0x5d8;
    /* 0x5E4 */ dCcD_Stts mCcStts;
    /* 0x620 */ dCcD_Sph mCcAtSph;
    /* 0x758 */ dCcD_Sph mCcTgSph;
    /* 0x890 */ dCcD_Sph mCcFireEffSph;
    /* 0x9C8 */ cXyz field_0x9c8;
    /* 0x9D4 */ cXyz field_0x9d4;
    /* 0x9E0 */ u32 mStickSmokeEMKey;
    /* 0x9E4 */ u32 mFireEMKeys[2];
    /* 0x9EC */ u32 field_0x9ec;
    /* 0x9F0 */ u32 field_0x9f0[4];
    /* 0xA00 */ cXyz field_0xa00;
    /* 0xA0C */ s16 field_0xa0c;
    /* 0xA10 */ f32 field_0xa10;
    /* 0xA14 */ u8 field_0xa14;
    /* 0xA18 */ Z2SoundObjArrow mSound;
};

#endif /* D_A_E_ARROW_H */
