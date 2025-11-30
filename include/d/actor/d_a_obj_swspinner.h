#ifndef D_A_OBJ_SWSPINNER_H
#define D_A_OBJ_SWSPINNER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/actor/d_a_spinner.h"

/**
 * @ingroup actors-objects
 * @class daObjSwSpinner_c
 * @brief Spinner Switch
 *
 * @details
 *
 */
class daObjSwSpinner_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    int GetRotSpeedY() { return mRotSpeedY; }
    f32 GetR() { return 100.0f; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModelA;
    /* 0x5AC */ J3DModel* mpModelB;
    /* 0x5B0 */ dBgW* mpBgW2;
    /* 0x5B4 */ Mtx mMtx;
    /* 0x5E4 */ f32 mPartBHeight;
    /* 0x5E8 */ bool mSpinnerIn;
    /* 0x5E9 */ bool mPrevSpinnerIn;
    /* 0x5EA */ bool mCanUse;
    /* 0x5EB */ bool mPrevIsSwbit2;
    /* 0x5EC */ s16 mPrevAngle;
    /* 0x5EE */ s16 mRotSpeedY;
    /* 0x5F0 */ s16 mCount;
};

STATIC_ASSERT(sizeof(daObjSwSpinner_c) == 0x5F4);

#endif /* D_A_OBJ_SWSPINNER_H */
