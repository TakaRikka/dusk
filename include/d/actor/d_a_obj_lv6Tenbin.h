#ifndef D_A_OBJ_LV6TENBIN_H
#define D_A_OBJ_LV6TENBIN_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daTenbin_c
 * @brief Temple of Time Scale
 *
 * @details
 *
 */
class daTenbin_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    int create();
    static void rideCallBackRight(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    static void rideCallBackLeft(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    int Execute(Mtx**);
    void procMain();
    void init_modeWait();
    void modeWait();
    void balanceCheck();
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel1;
    /* 0x5AC */ J3DModel* mModel2;
    /* 0x5B0 */ u8 mMode;
    /* 0x5B2 */ u16 field_0x5B2;
    /* 0x5B4 */ u16 field_0x5B4;
    /* 0x5B6 */ u16 field_0x5B6;
    /* 0x5B8 */ u16 field_0x5B8;
    /* 0x5BC */ f32 field_0x5BC;
    /* 0x5C0 */ f32 field_0x5C0;
    /* 0x5C4 */ f32 mUnkFallAmt1;
    /* 0x5C8 */ f32 mUnkFallAmt2;
    /* 0x5CC */ u8 mWaitTime;
    /* 0x5D0 */ int field_0x5D0;
    /* 0x5D4 */ csXyz field_0x5D4;
    /* 0x5DC */ f32 mShakeStrength;
    /* 0x5E0 */ f32 mXAmplitude;
    /* 0x5E4 */ f32 mZAmplitude;
    /* 0x5E8 */ f32 mShakeDamping;
    /* 0x5EC */ f32 mXZMaximumChange;
    /* 0x5F0 */ f32 mXZMinimumChange;
    /* 0x5F4 */ cXyz field_0x5F4;
    /* 0x600 */ f32 field_0x600;
    /* 0x604 */ f32 mYAmplitude;
    /* 0x608 */ f32 mMovementStrength;
    /* 0x60C */ f32 mRotationDamping;
    /* 0x610 */ f32 mYMaximumChange;
    /* 0x614 */ f32 mYMinimumChange;
    /* 0x618 */ bool field_0x618;
    /* 0x61C */ dBgW* mdBgW;
    /* 0x620 */ Mtx field_0x620;
    /* 0x650 */ int field_0x650;
};

STATIC_ASSERT(sizeof(daTenbin_c) == 0x654);

class daTenbin_HIO_c : public mDoHIO_entry_c {
public:
    daTenbin_HIO_c();
    ~daTenbin_HIO_c() {}
#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x08 */ f32 mPlateSpacing;
    /* 0x0C */ f32 mFallAmount;
    /* 0x10 */ f32 mPlateYAngle;
    /* 0x14 */ f32 mShakeStrength;
    /* 0x18 */ f32 mZAmplitude;
    /* 0x1C */ f32 mXAmplitude;
    /* 0x20 */ f32 mShakeDamping;
    /* 0x24 */ f32 mXZMaximumChange;
    /* 0x28 */ f32 mXZMinimumChange;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 mYAmplitude;
    /* 0x34 */ f32 mMovementStrength;
    /* 0x38 */ f32 mRotationDamping;
    /* 0x3C */ f32 mYMaximumChange;
    /* 0x40 */ f32 mYMinimumChange;
    /* 0x44 */ u8 mWaitTime;
};

#endif /* D_A_OBJ_LV6TENBIN_H */
