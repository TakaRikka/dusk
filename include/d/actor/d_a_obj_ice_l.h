#ifndef D_A_OBJ_ICE_L_H
#define D_A_OBJ_ICE_L_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjIce_l_c
 * @brief Ice (Large)
 *
 * @details
 *
 */
class daObjIce_l_c : public dBgS_MoveBgActor {
public:
    void RideOn_Angle(s16&, f32, s16, f32);
    int Check_RideOn(cXyz);
    void initBaseMtx();
    void setBaseMtx();
    int create();
    
    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    /* 0x5A0 */ f32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ u8 field_0x5A8[0x5AC - 0x5A8];
    /* 0x5AC */ s16 field_0x5ac;
    /* 0x5AE */ u8 field_0x5AE[0x5B0 - 0x5AE];
    /* 0x5B0 */ u8 field_0x5b0;
    /* 0x5B1 */ u8 field_0x5b1;
    /* 0x5B4 */ cXyz field_0x5b4;
    /* 0x5C0 */ f32 field_0x5c0;
    /* 0x5C4 */ u8 field_0x5C4[0x5CC - 0x5C4];
    /* 0x5CC */ csXyz field_0x5cc;
    /* 0x5D2 */ u8 field_0x5D2[0x5D4 - 0x5D2];
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D6 */ s16 field_0x5d6;
    /* 0x5D8 */ csXyz field_0x5d8;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ s16 field_0x5e4;
    /* 0x5E6 */ u8 field_0x5e6;
    /* 0x5E8 */ cXyz field_0x5e8;
    /* 0x5F4 */ s16 field_0x5f4;
    /* 0x5F8 */ dCcD_Stts mStts;
    /* 0x634 */ J3DModel* mpModel;
    /* 0x638 */ request_of_phase_process_class mPhase;
};

STATIC_ASSERT(sizeof(daObjIce_l_c) == 0x640);

#endif /* D_A_OBJ_ICE_L_H */
