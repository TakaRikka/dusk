#ifndef D_A_OBJ_GB_H
#define D_A_OBJ_GB_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"

class dBgW;

/**
 * @ingroup actors-objects
 * @class obj_gb_class
 * @brief Ganondorf Barrier
 *
 * @details
 *
 */
class obj_gb_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mModel;
    /* 0x574 */ mDoExt_btkAnm* mBtk;
    /* 0x578 */ mDoExt_brkAnm* mBrk;
    /* 0x57C */ u8 field_0x57c;
    /* 0x57D */ u8 mColorAlpha;
    /* 0x580 */ int mSw1;
    /* 0x584 */ int mSw2;
    /* 0x588 */ s16 mIsFinalBattle;
    /* 0x58A */ s16 field_0x58a[2];
    /* 0x58E */ s16 field_0x58e;
    /* 0x58E */ f32 mBrkFrame;
    /* 0x594 */ cXyz field_0x594[20];
    /* 0x684 */ f32 field_0x684;
    /* 0x688 */ Mtx mBgMtx;
    /* 0x6B8 */ dBgW* mpBgW;
};

STATIC_ASSERT(sizeof(obj_gb_class) == 0x6bc);

#endif /* D_A_OBJ_GB_H */
