#ifndef D_A_E_WARPAPPEAR_H
#define D_A_E_WARPAPPEAR_H

#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"

/**
 * @ingroup actors-enemies
 * @class e_warpappear_class
 * @brief Shadow Beast Warp Appear
 * 
 * @details 
 * 
 */
class e_warpappear_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class phase;
    /* 0x570 */ u8 arg0;
    /* 0x574 */ J3DModel* model;
    /* 0x578 */ mDoExt_btkAnm* btk;
    /* 0x57c */ mDoExt_brkAnm* anm_p[2];
    /* 0x584 */ int field_0x584;
    /* 0x588 */ u8 field_0x588[0x58c - 0x588];
    /* 0x58C */ u32 portal_app_b;
    /* 0x590 */ u32 portal_app_c;
    /* 0x594 */ s16 counter;
    /* 0x596 */ s16 action;
    /* 0x598 */ s16 field_0x598;
    /* 0x59A */ s16 timer[2];
    /* 0x59E */ s16 demo_mode;
    /* 0x5A0 */ s16 demo_timer;
    /* 0x5A4 */ cXyz field_0x5a4;
    /* 0x5B0 */ cXyz field_0x5b0;
    /* 0x5BC */ cXyz field_0x5bc;
    /* 0x5C8 */ cXyz field_0x5c8;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ f32 field_0x5dc;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ f32 field_0x5e8;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ dMsgFlow_c MsgFlow;
    /* 0x64C */ int target_info_count;
};

STATIC_ASSERT(sizeof(e_warpappear_class) == 0x650);


#endif /* D_A_E_WARPAPPEAR_H */
