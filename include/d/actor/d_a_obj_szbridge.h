#ifndef D_A_OBJ_SZBRIDGE_H
#define D_A_OBJ_SZBRIDGE_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSZbridge_c
 * @brief Stone Statue Bridge
 *
 * @details
 *
 */
class daObjSZbridge_c : public dBgS_MoveBgActor, request_of_phase_process_class {
public:
    void rideActor(fopAc_ac_c*);
    int create1st();
    void setMtx();
    int CreateHeap();
    void chkBg();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    virtual ~daObjSZbridge_c() {}

private:
    /* 0x5A8 */ Mtx mBgMtx;
    /* 0x5D8 */ Mtx field_0x5d8;
    /* 0x608 */ J3DModel* mpModel;
    /* 0x60C */ f32 mWaterY;
    /* 0x610 */ f32 mGroundY;
    /* 0x614 */ u8 field_0x614;
    /* 0x618 */ u32 field_0x618[2];
    /* 0x620 */ int field_0x620;
};

STATIC_ASSERT(sizeof(daObjSZbridge_c) == 0x624);

#endif /* D_A_OBJ_SZBRIDGE_H */
