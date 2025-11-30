#ifndef D_A_OBJ_MSIMA_H
#define D_A_OBJ_MSIMA_H

#include "f_op/f_op_actor_mng.h"

struct ms_chain_s {
    /* 0x00 */ J3DModel* model[35];
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ s16 field_0x90;
    /* 0x92 */ s8 field_0x92;
    /* 0x94 */ cXyz mPos;
};

/**
 * @ingroup actors-objects
 * @class obj_msima_class
 * @brief Dangoro Boss Stage (Magnetic Island)
 *
 * @details
 *
 */
class obj_msima_class {
public:
    /* 0x000 */ fopAc_ac_c mActor;
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ mDoExt_brkAnm* mpBrk;
    /* 0x57C */ mDoExt_btkAnm* mpBtk;
    /* 0x580 */ s16 mFloatingOscillator;
    /* 0x582 */ s16 mAction;
    /* 0x584 */ s16 field_0x584;
    /* 0x586 */ s16 field_0x586[2];
    /* 0x58C */ f32 field_0x58c;
    /* 0x590 */ u8 field_0x590[0x594 - 0x590];
    /* 0x594 */ s16 field_0x594;
    /* 0x596 */ s16 field_0x596;
    /* 0x598 */ s16 field_0x598;
    /* 0x59A */ s16 field_0x59a;
    /* 0x59C */ f32 field_0x59c;
    /* 0x5A0 */ s16 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ s16 field_0x5ac;
    /* 0x5AE */ s16 field_0x5ae;
    /* 0x5B0 */ cXyz field_0x5b0;
    /* 0x5BC */ fpc_ProcID field_0x5bc[2];
    /* 0x5C4 */ s8 field_0x5c4[2];
    /* 0x5C6 */ u8 field_0x5C6[0x5CC - 0x5C6];
    /* 0x5CC */ Mtx mBgMtx;
    /* 0x5FC */ dBgW* mpBgW;
    /* 0x600 */ Z2SoundObjSimple mSound;
    /* 0x620 */ ms_chain_s mChains[4];
    /* 0x8A0 */ bool mIsHIOOwner;

    enum MAGNESIMA_RES_FILE_ID {
        /* BMDR */
        /* 0x06 */ BMDR_KUSARI = 6,
        /* 0x07 */ BMDR_S_MAGNE_SIMA,

        /* BRK */
        /* 0x0A */ BRK_S_MAGNE_SIMA = 0xA,

        /* BTK */
        /* 0x0D */ BTK_S_MAGNE_SIMA = 0xD,

        /* DZB */
        /* 0x10 */ DZB_S_MAGNE_SIMA = 0x10,
    };

    enum Action {
        ACTION_START,
        ACTION_DROP,
        ACTION_FLOAT_1,
        ACTION_FLOAT_2,
    };
};

STATIC_ASSERT(sizeof(obj_msima_class) == 0x8a4);

class daObj_Msima_HIO_c : public JORReflexible {
public:
    daObj_Msima_HIO_c();
    virtual ~daObj_Msima_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mSize;
    /* 0x0C */ s16 mMaxTiltAngle;
    /* 0x0E */ s16 mMaxSlopeAngle;
    /* 0x10 */ f32 mBasicPosition;
};


#endif /* D_A_OBJ_MSIMA_H */
