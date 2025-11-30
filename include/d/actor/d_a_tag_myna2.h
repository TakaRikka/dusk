#ifndef D_A_TAG_MYNA2_H
#define D_A_TAG_MYNA2_H

#include "d/d_com_inf_game.h"

class daTagMyna2_c : public fopAc_ac_c {
public:
    s32 create();
    s32 execute();

    u8 getSwBit() { return fopAcM_GetParam(this) >> 8; }
    u8 getMode() { return fopAcM_GetParam(this); }
    f32 getExtent() { return scale.x * 100.0f;}

private:
    /* 0x568 */ u32 mSwitchNo;
    /* 0x56C */ u32 mMode;
    /* 0x570 */ s16 mTimer;
};

STATIC_ASSERT(sizeof(daTagMyna2_c) == 0x574);

#endif /* D_A_TAG_MYNA2_H */
