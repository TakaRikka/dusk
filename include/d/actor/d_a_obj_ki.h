#ifndef D_A_OBJ_KI_H
#define D_A_OBJ_KI_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_w.h"

struct sq_cp_s {
    ~sq_cp_s() {}
    sq_cp_s() {}

    /* 0x00 */ cXyz field_0x0[4];
};

STATIC_ASSERT(sizeof(sq_cp_s) == 0x30);

/**
 * @ingroup actors-objects
 * @class obj_ki_class
 * @brief Tree
 *
 * @details
 *
 */
class obj_ki_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ u8 mBmdIdx;
    /* 0x571 */ u8 mScaleXZ;
    /* 0x572 */ u8 mScaleY;
    /* 0x574 */ mDoExt_McaMorf* mpMorf;
    /* 0x578 */ s16 mCounter;
    /* 0x57A */ s16 field_0x57a;
    /* 0x57C */ u8 field_0x57c[8];
    /* 0x584 */ s16 mTimers[4];
    /* 0x58C */ csXyz field_0x58c[3];
    /* 0x59E */ s16 field_0x59e[3];
    /* 0x5A4 */ sq_cp_s field_0x5a4[3];
    /* 0x634 */ u8 field_0x634[4];
    /* 0x638 */ Mtx mMtx;
    /* 0x668 */ dBgW* mpBgW;
    /* 0x66C */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(obj_ki_class) == 0x670);

class daObj_Ki_HIO_c {
public:
    daObj_Ki_HIO_c();
    virtual ~daObj_Ki_HIO_c() {};

    /* 0x4 */ s8 field_0x4;
    /* 0x5 */ u8 field_0x5;
};

STATIC_ASSERT(sizeof(daObj_Ki_HIO_c) == 8);

#endif /* D_A_OBJ_KI_H */
