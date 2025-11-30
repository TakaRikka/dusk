#ifndef D_A_OBJ_WEB0_H
#define D_A_OBJ_WEB0_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class obj_web0_class
 * @brief Wall Web
 *
 * @details
 *
 */
class obj_web0_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 mPrm0;
    /* 0x571 */ u8 mScaleXY;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ mDoExt_brkAnm* mpBrk;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ u8 field_0x57e[0x582 - 0x57e];
    /* 0x582 */ s16 mReboundTimer;
    /* 0x584 */ s16 field_0x584[2];
    /* 0x588 */ s16 mHitTimer;
    /* 0x58A */ u8 field_0x58a[0x590 - 0x58a];
    /* 0x590 */ Mtx mMtx;
    /* 0x5C0 */ dBgW* mpBgW;
    /* 0x5C4 */ dCcD_Stts mStts;
    /* 0x600 */ dCcD_Sph mSphCc;
    /* 0x738 */ u8 mDeleteTimer;
    /* 0x739 */ u8 field_0x739;
};

STATIC_ASSERT(sizeof(obj_web0_class) == 0x73C);

class daObj_Web0_HIO_c {
public:
    daObj_Web0_HIO_c();
    virtual ~daObj_Web0_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
};

#endif /* D_A_OBJ_WEB0_H */
