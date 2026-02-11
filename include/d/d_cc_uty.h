#ifndef D_CC_D_CC_UTY_H
#define D_CC_D_CC_UTY_H

#include "SSystem/SComponent/c_sxyz.h"
#include <dolphin/types.h>

class cCcD_Obj;
class fopAc_ac_c;
class Z2Creature;

enum dCcU_AtType {
    /* 0x01 */ HIT_TYPE_LINK_NORMAL_ATTACK = 1,
    /* 0x02 */ HIT_TYPE_BOMB,
    /* 0x09 */ HIT_TYPE_LINK_HEAVY_ATTACK = 9,
    /* 0x0A */ HIT_TYPE_BOOMERANG,
    /* 0x0F */ HIT_TYPE_ARROW = 15,
    /* 0x10 */ HIT_TYPE_STUN,
};

struct dCcU_AtInfo {
    /* 0x00 */ cCcD_Obj* mpCollider;
    /* 0x04 */ fopAc_ac_c* mpActor;
    /* 0x08 */ Z2Creature* mpSound;
    /* 0x0C */ csXyz mHitDirection;
    /* 0x14 */ u32 mHitBit;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ u16 mAttackPower;
    /* 0x1E */ u8 mPowerType;
    /* 0x1F */ s8 mHitStatus;  // maybe inaccurate name
    /* 0x20 */ u8 mHitType;
};

int pl_cut_LRC(int index);
u16 cc_pl_cut_bit_get();
fopAc_ac_c* cc_at_check(fopAc_ac_c*, dCcU_AtInfo*);
fopAc_ac_c* at_power_check(dCcU_AtInfo* i_AtInfo);
void def_se_set(Z2Creature* p_sound, cCcD_Obj* p_collider, u32 map_info, fopAc_ac_c* p_actor);

inline fopAc_ac_c* dCc_GetAc(void* i_actor) {
    return static_cast<fopAc_ac_c*>(i_actor);
}

#endif /* D_CC_D_CC_UTY_H */
