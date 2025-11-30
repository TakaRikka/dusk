#ifndef D_A_OBJ_MYOGAN_H
#define D_A_OBJ_MYOGAN_H

#include "f_op/f_op_actor_mng.h"

class dBgW;

/**
 * @ingroup actors-objects
 * @class obj_myogan_class
 * @brief ???
 *
 * @details
 *
 */
class obj_myogan_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_btkAnm* mpBtk;
    /* 0x578 */ mDoExt_brkAnm* mpBrk;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ s16 mAction;
    /* 0x580 */ u8 field_0x580[0x582 - 0x580];
    /* 0x582 */ s16 field_0x582[2];
    /* 0x586 */ u8 field_0x586[0x58C - 0x586];
    /* 0x58C */ Mtx mBgMtx;
    /* 0x5BC */ dBgW* mpBgW;
    /* 0x5C0 */ u8 mInitialized;
};

class daObj_Myogan_HIO_c {
public:
    daObj_Myogan_HIO_c();
    virtual ~daObj_Myogan_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mSize;
    /* 0x0C */ f32 mNormalHeight;
};

#endif /* D_A_OBJ_MYOGAN_H */
