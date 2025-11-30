#ifndef D_A_TAG_QS_H
#define D_A_TAG_QS_H

#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

class daTagQs_c : public fopAc_ac_c {
public:
    ~daTagQs_c();
    int create();
    f32 getPower();
    f32 getSafeArea();
    f32 getPower80();
    f32 getCenterPower();
    f32 calcPower(f32, f32);
    int execute();
    int draw();

    u8 getType() { return fopAcM_GetParamBit(this, 0, 1); }
    int getSwNo() { return fopAcM_GetParamBit(this, 1, 8); }

    /* 0x568 */ f32 field_0x568;
    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ int field_0x570;
    /* 0x574 */ int field_0x574;
    /* 0x578 */ dCcD_Cyl mCyl;
    /* 0x6B4 */ dCcD_Stts mStts;
};

#endif /* D_A_TAG_QS_H */
