#ifndef D_A_TAG_CSW_H
#define D_A_TAG_CSW_H

#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

class dBgW;

struct daTagCsw_c : public fopAc_ac_c, public request_of_phase_process_class {
    ~daTagCsw_c();
    void setMtx();
    int createHeap();
    int chkInsideStatueStart();
    int create();
    int Delete();
    void onLight();
    void offLight();
    int execute();
    int draw();

    int getSw() { return fopAcM_GetParamBit(this, 1, 8); }
    int getSw2() { return fopAcM_GetParamBit(this, 9, 8); }
    int getType() { return fopAcM_GetParamBit(this, 0, 1); }
    int getParam(int param_1, int param_2, int param_3) {
        return (1 << param_3) - 1U & param_1 >> (param_2 & 0x3fU);
    }
    int getArg0() { return getParam(current.angle.x, 0, 8); }
    int getItemBit() { return fopAcM_GetParamBit(this, 17, 8); }

    /* 0x570 */ int field_0x570;
    /* 0x574 */ J3DModel* field_0x574;
    /* 0x578 */ J3DModel* field_0x578;
    /* 0x57C */ mDoExt_brkAnm* field_0x57c;
    /* 0x580 */ mDoExt_brkAnm* field_0x580;
    /* 0x584 */ dBgW* field_0x584;
    /* 0x588 */ dBgW* field_0x588;
    /* 0x58C */ Mtx field_0x58c;
    /* 0x5BC */ Mtx field_0x5bc;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ dBgW* field_0x5f0;
    /* 0x5F4 */ Mtx field_0x5f4;
    /* 0x624 */ f32 field_0x624;
    /* 0x628 */ f32 field_0x628;
    /* 0x62C */ dCcD_Stts field_0x62c;
    /* 0x668 */ dCcD_Cyl field_0x668;
    /* 0x7A4 */ dCcD_Cyl field_0x7a4;
    /* 0x8E0 */ bool mLightOn;
    /* 0x8E1 */ bool field_0x8e1;
    /* 0x8E2 */ s16 field_0x8e2;
    /* 0x8E4 */ int mItemNo;
    /* 0x8E8 */ cXyz field_0x8e8;
    /* 0x8F4 */ u8 field_0x8f4;
};

#endif /* D_A_TAG_CSW_H */
