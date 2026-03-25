#ifndef D_A_OBJ_WFLAG_H
#define D_A_OBJ_WFLAG_H

#include "f_op/f_op_actor_mng.h"

struct wf_tail_s {
    ~wf_tail_s() {}
    wf_tail_s() {}

    /* 0x000 */ s16 field_0x0;
    /* 0x000 */ u16 unk_0x2;
    /* 0x004 */ cXyz pos[20];
    /* 0x0F4 */ csXyz rot[20];
    /* 0x16C */ cXyz unk_0x16c[20];
    /* 0x25C */ J3DModel* model[19];
    /* 0x2A8 */ u32 unk_0x2a8;
};

/**
 * @ingroup actors-objects
 * @class obj_wflag_class
 * @brief ???
 *
 * @details
 *
 */
class obj_wflag_class {
public:
    /* 0x000 */ fopAc_ac_c actor;
    /* 0x568 */ request_of_phase_process_class phase;
    /* 0x570 */ J3DModel* pillar_model;
    /* 0x574 */ mDoExt_McaMorf* anm_p;
    /* 0x578 */ mDoExt_btkAnm* kolin_btk;
    /* 0x57C */ mDoExt_btpAnm* kolin_btp;
    /* 0x580 */ wf_tail_s tail_s[1];
    /* 0x82C */ u8 unk_0x82c;
    /* 0x82E */ s16 field_0x82e;
    /* 0x830 */ s16 timers[2];
    /* 0x834 */ s16 unk_timer;
    /* 0x836 */ s16 counter;
    /* 0x838 */ u32 field_0x838;
};

STATIC_ASSERT(sizeof(obj_wflag_class) == 0x83C);

#endif /* D_A_OBJ_WFLAG_H */
