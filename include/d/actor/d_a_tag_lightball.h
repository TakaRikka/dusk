#ifndef D_A_TAG_LIGHTBALL_H
#define D_A_TAG_LIGHTBALL_H

#include "f_op/f_op_actor_mng.h"

class daTagLightBall_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();

    u8 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }
    u32 getType() { return fopAcM_GetParamBit(this, 8, 4); }

    /* 0x568 */ DALKMIST_INFLUENCE mInfluence;
    /* 0x57C */ u8 field_0x57c;
    /* 0x57D */ u8 mInfSet;
};

#endif /* D_A_TAG_LIGHTBALL_H */
