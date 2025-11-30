#ifndef D_A_NPC_LF_H
#define D_A_NPC_LF_H

#include "d/actor/d_a_npc.h"

struct lf_s {
    ~lf_s();
    lf_s();

    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ cXyz mPos;
    /* 0x10 */ csXyz mAngle;
    /* 0x16 */ s8 field_0x16;
    /* 0x17 */ s8 field_0x17;
    /* 0x18 */ s16 field_0x18;
    /* 0x1A */ s16 mActionMode;
    /* 0x1C */ s16 field_0x1c;
    /* 0x20 */ cXyz field_0x20;
    /* 0x2C */ u8 field_0x2c[0x34-0x2c];
    /* 0x34 */ f32 mDistFromPlayer;
    /* 0x38 */ s16 field_0x38[2];
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ s16 field_0x4c[2];
    /* 0x50 */ s16 field_0x50[2];
    /* 0x54 */ u8 field_0x54[4];
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5c;
    /* 0x60 */ f32 field_0x60;
    /* 0x64 */ u8 field_0x64[0x68-0x64];
};

class daNPC_LF_HIO_c {
public:
    daNPC_LF_HIO_c();
    virtual ~daNPC_LF_HIO_c();

public:
    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
};

/**
 * @ingroup actors-npcs
 * @class npc_lf_class
 * @brief Little Fish
 *
 * @details Group of several small fish spawned in a group. Used in Fishing Pond.
 *
 */
class npc_lf_class : public fopEn_enemy_c {
public:
    enum Action {
        ACT_MOVE
    };

public:
    /* 0x05AC */ request_of_phase_process_class mPhase; ///< @brief Actor phase process class.
    /* 0x05B4 */ u8 mParam1;                            ///< @brief Actor parameters.
    /* 0x05B5 */ u8 mParam2;                            ///< @brief Actor parameters.
    /* 0x05B8 */ int mIter;                             ///< @brief Iterator used to track mFish index.
    /* 0x05BC */ lf_s mFish[64];                        ///< @brief The number of small fish spawned in the group?
    /* 0x1FBC */ u8 field_0x1fbc[0x4];                  ///< @brief ???
    /* 0x1FC0 */ u8 field_0x1fc0;                       ///< @brief ???
};

STATIC_ASSERT(sizeof(npc_lf_class) == 0x1fc4);

#endif /* D_A_NPC_LF_H */
