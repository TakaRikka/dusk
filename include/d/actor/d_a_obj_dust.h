#ifndef D_A_OBJ_DUST_H
#define D_A_OBJ_DUST_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_event_lib.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_resorce.h"
#include "d/d_s_play.h"

#include <cmath.h>

/**
 * @ingroup actors-objects
 * @class daObjDust_c
 * @brief Dust
 *
 * @details
 *
 */
class daObjDust_c : public dBgS_MoveBgActor {
public:
    void Search_Ymb();
    void RideOn_Angle(s16&, f32, s16, f32);
    int Check_RideOn();
    void initBaseMtx();
    void setBaseMtx();

    inline int create();
    inline int CreateHeap();

    inline int Create();
    inline int Execute(Mtx**);
    inline int Draw();
    inline int Delete();

    /* 0x5A0 */ f32 mRideOscillationSpeed;
    /* 0x5A4 */ f32 mRideIntensity; 
    /* 0x5A8 */ f32 mRideBounce; 
    /* 0x5AC */ s16 mRideAngleAmplitude;
    /* 0x5AE */ u8 field_0x5ae[0x5BC - 0x5AE];
    /* 0x5BC */ csXyz field_0x5bc;
    /* 0x5C2 */ u8 field_0x5c2[0x5C4 - 0x5C2];
    /* 0x5C4 */ s16 mRideOscillationPhaseX;
    /* 0x5C6 */ s16 mRideOscillationPhaseZ;
    /* 0x5C8 */ csXyz mRideAngle;
    /* 0x5D0 */ f32 mOffsetY;
    /* 0x5D4 */ cXyz field_0x5d4;
    /* 0x5E0 */ u8 field_0x5e0;
    /* 0x5E1 */ bool mYmbFlag;
    /* 0x5E4 */ dCcD_Stts mStts;
    /* 0x620 */ J3DModel* mpModel;
    /* 0x624 */ u8 field_0x624[0x628 - 0x624];
    /* 0x628 */ request_of_phase_process_class mPhaseReq;
    /* 0x630 */ bool mRideOnFlag;
    /* 0x634 */ dBgS_AcchCir mAcchCir;
    /* 0x674 */ dBgS_ObjAcch mAcch;
};

STATIC_ASSERT(sizeof(daObjDust_c) == 0x84c);


#endif /* D_A_OBJ_DUST_H */
