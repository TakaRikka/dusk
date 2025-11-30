#ifndef D_A_OBJ_SWBALLC_H
#define D_A_OBJ_SWBALLC_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSwBallC_c
 * @brief Ball Switch C
 *
 * @details
 *
 */
class daObjSwBallC_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int demoProc();
    void setLightOnSwB();
    void setLightOffSwB();
    void deleteLightBall();
    void deleteLightBallA();
    void deleteLightBallB();
    int calcLightBallScale();
    int draw();
    int _delete();

    void setAction(u8 action) { field_0x57f = action; }

    /* 0x568 */ request_of_phase_process_class field_0x568;
    /* 0x570 */ J3DModel* mModel;
    /* 0x574 */ mDoExt_btkAnm* field_0x574;
    /* 0x578 */ u32 field_0x578;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ u8 field_0x57e;
    /* 0x57F */ u8 field_0x57f;
    /* 0x580 */ u8 field_0x580;
    /* 0x581 */ u8 field_0x581;
    /* 0x582 */ u8 field_0x582;
    /* 0x583 */ u8 field_0x583;
    /* 0x584 */ u8 field_0x584;
    /* 0x585 */ s8 field_0x585;
    /* 0x586 */ u8 field_0x586;
    /* 0x587 */ u8 field_0x587;
    /* 0x588 */ u32 field_0x588;
};

STATIC_ASSERT(sizeof(daObjSwBallC_c) == 0x58C);

#endif /* D_A_OBJ_SWBALLC_H */
