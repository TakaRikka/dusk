#ifndef D_A_OBJ_LV5YCHNDLR_H
#define D_A_OBJ_LV5YCHNDLR_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjYchndlr_c
 * @brief Snowpeak Ruins Chandelier (Swinging Platform)
 *
 * @details
 *
 */
class daObjYchndlr_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
    int create1st();
    void setMtx();
    void rideActor(fopAc_ac_c*);

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();
    virtual ~daObjYchndlr_c() {}

private:
    /* 0x5A8 */ Mtx field_0x5a8;
    /* 0x5D8 */ Mtx field_0x5d8;
    /* 0x608 */ J3DModel* mpChandlierModel;
    /* 0x60C */ J3DModel* mpShaftModels[4];
    /* 0x61C */ s16 mShaftRotZ;
    /* 0x61E */ s16 field_0x61e;
    /* 0x620 */ u8 field_0x620;
    /* 0x624 */ dCcD_Stts mCcStts;
    /* 0x660 */ dCcD_Cyl mShaftCcCyl[4];
    /* 0xB50 */ dCcD_Cyl mCcCyl;
    /* 0xC8C */ u8 field_0xc8c;
    /* 0xC90 */ f32 field_0xc90;
    /* 0xC94 */ cXyz mChandelierCenter;
};

STATIC_ASSERT(sizeof(daObjYchndlr_c) == 0xca0);


#endif /* D_A_OBJ_LV5YCHNDLR_H */
