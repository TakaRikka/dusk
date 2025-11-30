#ifndef D_A_TAG_LV8GATE_H
#define D_A_TAG_LV8GATE_H

#include "f_op/f_op_actor_mng.h"

class J3DModel;
class mDoExt_bckAnm;

class daTagLv8Gate_c : public fopAc_ac_c {
public:
    inline ~daTagLv8Gate_c();

    int createHeap();
    int execute();

    void initBaseMtx();
    void create_init();
    int create();
    int draw();

    u8 getSceneNo() { return fopAcM_GetParam(this) & 0xff; }

    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_bckAnm* mpBck;
    /* 0x570 */ request_of_phase_process_class mPhaseReq;
    /* 0x578 */ s16 mEventID;
};

STATIC_ASSERT(sizeof(daTagLv8Gate_c) == 0x57C);

#endif /* D_A_TAG_LV8GATE_H */
