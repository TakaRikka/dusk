#ifndef D_A_COACH_FIRE_H
#define D_A_COACH_FIRE_H

#include "d/actor/d_a_npc_coach.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

struct CoachFireAttr {
    f32 radius;
    f32 field_0x04;
    u8 field_0x08;
    // GXColorS10 color;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    f32 pow;
};

/**
 * @ingroup actors-unsorted
 * @class daCoachFire_c
 * @brief Coach Fire
 *
 * @details
 *
 */
class daCoachFire_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void initCcSphere();

    const CoachFireAttr& attr() { return M_attr; }

    static CoachFireAttr const M_attr;

    inline ~daCoachFire_c();

    inline int draw();
    inline void setCcSphere();
    void setNoHitTimer() { noHitTimer = 20; }
    inline int execute();

private:
    /* 0x568 */ LIGHT_INFLUENCE mLightInfluence;
    /* 0x588 */ dCcD_Stts mStts;
    /* 0x5C4 */ dCcD_Sph mSph;
    /* 0x6FC */ Mtx* mpMtx;
    /* 0x700 */ cXyz mPos;
    /* 0x70C */ u32 mParticleKey;
    /* 0x710 */ int noHitTimer;
};

STATIC_ASSERT(sizeof(daCoachFire_c) == 0x714);

#endif /* D_A_COACH_FIRE_H */
