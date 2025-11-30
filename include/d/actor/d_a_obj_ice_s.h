#ifndef D_A_OBJ_ICE_S_H
#define D_A_OBJ_ICE_S_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjIce_s_c
 * @brief Ice (Small)
 *
 * @details
 *
 */
class daObjIce_s_c : public dBgS_MoveBgActor {
public:
    void RideOn_Angle(s16&, f32, s16, f32);
    bool Check_LinkRideOn(cXyz);
    bool Check_RideOn(cXyz);
    void initBaseMtx();
    void setBaseMtx();
    int create();
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    /* 0x5A0 */ f32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ s16 field_0x5ac;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5AE */ u8 field_0x5b4[0x5c4 - 0x5b4];
    /* 0x5C4 */ u8 field_0x5c4;
    /* 0x5C5 */ u8 field_0x5c5;
    /* 0x5C8 */ cXyz field_0x5c8;
    /* 0x5D8 */ s16 field_0x5d4;
    /* 0x5D8 */ s16 field_0x5d6;
    /* 0x5D8 */ s16 field_0x5d8;
    /* 0x5DA */ s16 field_0x5da;
    /* 0x5DC */ s16 field_0x5dc;
    /* 0x5DE */ s16 field_0x5de;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ s16 field_0x5e4;
    /* 0x5E6 */ s16 field_0x5e6;
    /* 0x5E8 */ u8 field_0x5e8;
    /* 0x5EC */ int field_0x5ec;
    /* 0x5F0 */ int field_0x5f0;
    /* 0x5F4 */ int field_0x5f4;
    /* 0x5EC */ dCcD_Stts mStts;
    /* 0x634 */ J3DModel* mModel;
    /* 0x638 */ request_of_phase_process_class mPhase;
    /* 0x640 */ u8 field_0x640;
};

STATIC_ASSERT(sizeof(daObjIce_s_c) == 0x644);

class daOBJ_ICE_S_HIO_c {
public:
    daOBJ_ICE_S_HIO_c();
    virtual ~daOBJ_ICE_S_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
};


#endif /* D_A_OBJ_ICE_S_H */
