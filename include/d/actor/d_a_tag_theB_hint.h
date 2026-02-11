#ifndef D_A_TAG_THEB_HINT_H
#define D_A_TAG_THEB_HINT_H

#include "f_op/f_op_actor_mng.h"

class daTagTheBHint_c : public fopAc_ac_c {
public:

    u32 getSwitchNo() {
        return shape_angle.x & 0xff;
    }

    u32 getMessageNo() {
        return shape_angle.z & 0xffff;
    }

    u16 getLinkID() {
        return fopAcM_GetParam(this) & 0xFFFF;
    }

    s8 getRoomNo() {
        return fopAcM_GetParam(this) >> 24;
    }

    inline int create();
    inline int execute();

    /* 0x568 */ f32 field_0x568;
};

#endif /* D_A_TAG_THEB_HINT_H */
