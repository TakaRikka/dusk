#ifndef D_A_OBJ_WFLAG_H
#define D_A_OBJ_WFLAG_H

#include "f_op/f_op_actor_mng.h"

struct wf_tail_s {
    ~wf_tail_s() {}
    wf_tail_s() {}

    /* 0x000 */ s16 field_0x000;
    /* 0x000 */ u16 field_0x002;
    /* 0x004 */ cXyz field_0x004[20];
    /* 0x0F4 */ csXyz field_0x0f4[20];
    /* 0x16C */ cXyz field_0x16c[20];
    /* 0x25C */ J3DModel* mModels[19];
    /* 0x2A8 */ u32 field_0x2a8;
};

/**
 * @ingroup actors-objects
 * @class obj_wflag_class
 * @brief ???
 *
 * @details
 *
 */
class obj_wflag_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_McaMorf* mMorf;
    /* 0x578 */ mDoExt_btkAnm* field_0x578;
    /* 0x57C */ mDoExt_btpAnm* field_0x57c;
    /* 0x580 */ wf_tail_s mTails[1];
    /* 0x82C */ u8 field_0x82c;
    /* 0x82E */ s16 field_0x82e;
    /* 0x830 */ s16 field_0x830[2];
    /* 0x834 */ s16 field_0x834;
    /* 0x836 */ s16 field_0x836;
    /* 0x838 */ u32 field_0x838;
};

STATIC_ASSERT(sizeof(obj_wflag_class) == 0x83C);

#endif /* D_A_OBJ_WFLAG_H */
