#ifndef D_A_L7OP_DEMO_DR_H
#define D_A_L7OP_DEMO_DR_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-unsorted
 * @class daL7ODR_c
 * @brief
 *
 * @details
 *
 */
class daL7ODR_c : public fopAc_ac_c {
public:
    typedef void (daL7ODR_c::*actionFn)();

    void setAction(actionFn);
    void action();
    void setDrAction(actionFn);
    void dr_action();
    void mtx_set();
    int draw();
    void wait();
    void pl_walk();
    void setZoomOutCamPos(cXyz&, cXyz&, f32);
    void dr_wait();
    void dr_fly();
    int execute();
    int _delete();
    int CreateHeap();
    BOOL check_start();
    int create();

    /* 0x568 */ dBgS_AcchCir mAcchCir;
    /* 0x5A8 */ dBgS_ObjAcch mAcch;
    /* 0x780 */ request_of_phase_process_class mPhase;
    /* 0x788 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x78C */ Z2CreatureEnemy mSound;
    /* 0x830 */ cXyz field_0x830;
    /* 0x83C */ cXyz mDemoCamEye;
    /* 0x848 */ cXyz mDemoCamCenter;
    /* 0x854 */ u8 field_0x854[0x858 - 0x854];
    /* 0x858 */ actionFn mAction;
    /* 0x864 */ actionFn mPrevAction;
    /* 0x870 */ actionFn mDrAction;
    /* 0x87C */ actionFn mPrevDrAction;
    /* 0x888 */ int field_0x888;
    /* 0x88C */ int field_0x88c;
    /* 0x890 */ int field_0x890;
    /* 0x894 */ u32 mShadowKey;
    /* 0x898 */ f32 field_0x898;
    /* 0x89C */ f32 field_0x89c;
    /* 0x8A0 */ f32 field_0x8a0;
    /* 0x8A4 */ f32 field_0x8a4;
    /* 0x8A8 */ u8 field_0x8A8[0x8B4 - 0x8A8];
    /* 0x8B4 */ u8 field_0x8b4;
    /* 0x8B5 */ u8 field_0x8b5;
    /* 0x8B6 */ u8 field_0x8b6;
    /* 0x8B7 */ u8 field_0x8b7;
};

STATIC_ASSERT(sizeof(daL7ODR_c) == 0x8b8);


#endif /* D_A_L7OP_DEMO_DR_H */
