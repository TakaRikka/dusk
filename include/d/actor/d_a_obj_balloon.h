#ifndef D_A_OBJ_BALLOON_H
#define D_A_OBJ_BALLOON_H

#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "d/d_cc_d.h"
#include "SSystem/SComponent/c_phase.h"

/**
 * @ingroup actors-objects
 * @class daObj_Balloon_c
 * @brief Balloon
 *
 * @details
 *
 */
class daObj_Balloon_c : public fopAc_ac_c {
public:
    enum Size_e {
        SIZE_LARGE_e,
        SIZE_MEDIUM_e,
        SIZE_SMALL_e,
    };

    int draw();
    void saveBestScore();
    void cc_set();
    void action();
    void mtx_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mModel;
    /* 0x574 */ u32 mShadowKey;
    /* 0x578 */ u8 mType;
    /* 0x579 */ u8 mRotateMode;
    /* 0x57C */ int mRotateTimer;
    /* 0x580 */ s16 mRotateZ;
    /* 0x582 */ s16 mRotateY;
    /* 0x584 */ s16 mRotZTarget;
    /* 0x586 */ s16 mRotYTarget;
    /* 0x588 */ dBgS_AcchCir mAcchCir;
    /* 0x5C8 */ dBgS_ObjAcch mAcch;
    /* 0x7A0 */ dCcD_Stts mStts;
    /* 0x7D8 */ dCcD_Sph mSph;
    /* 0x914 */ u8 mHIOInit;
    /* 0x914 */ u8 mUnused[8];
};

class daObj_Balloon_HIO_c {
public:
    daObj_Balloon_HIO_c();
    virtual ~daObj_Balloon_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mScale[3];
    /* 0x14 */ f32 mScore[3];
};

#endif /* D_A_OBJ_BALLOON_H */
