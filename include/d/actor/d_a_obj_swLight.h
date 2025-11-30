#ifndef D_A_OBJ_SWLIGHT_H
#define D_A_OBJ_SWLIGHT_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSwLight_c
 * @brief Light Switch
 *
 * @details
 *
 */
class daObjSwLight_c : public dBgS_MoveBgActor, public dEvLib_callback_c {
public:
    typedef void (daObjSwLight_c::*modeFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void action();
    void init_modeSwOnWait();
    void modeSwOnWait();
    void init_modeOnLight();
    void modeOnLight();
    void init_modeSwOffWait();
    void modeSwOffWait();
    void modeDead();
    int checkHit();
    void setOnTimer();

    virtual int Create();
    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();
    virtual ~daObjSwLight_c() {};

    daObjSwLight_c() : dEvLib_callback_c(this) {};

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return fopAcM_GetParamBit(this, 24, 8); }
    u8 getOnTime() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getEvent() { return (u8)field_0xb22; }

private:
    /* 0x5B0 */ request_of_phase_process_class mPhase;
    /* 0x5B8 */ J3DModel* mpModel;
    /* 0x6F4 */ dCcD_Stts mStts;
    /* 0x614 */ dCcD_Sph mAtSph;
    /* 0x730 */ dCcD_Sph mTgSph[2];
    /* 0x9BC */ dCcD_Cyl mCyl;
    /* 0xADC */ f32 field_0xadc;
    /* 0xAE0 */ u8 field_0xae0;
    /* 0xAE1 */ u8 field_0xae1;
    /* 0xAE2 */ u8 field_0xae2;
    /* 0xAE3 */ u8 field_0xae3;
    /* 0xAE0 */ u16 field_0xae4;
    /* 0xAE0 */ u8 field_0xae6;
    /* 0xAE0 */ u8 field_0xae7;
    /* 0xAE8 */ u16 field_0xae8;
    /* 0xAE8 */ u16 field_0xaea;
    /* 0xAEC */ DALKMIST_INFLUENCE mInfluence;
    /* 0xB00 */ Z2SoundObjSimple mSound;
    /* 0xB20 */ u8 field_0xb20;
    /* 0xB22 */ u16 field_0xb22;
    /* 0xB24 */ u16 field_0xb24;
};

STATIC_ASSERT(sizeof(daObjSwLight_c) == 0xb28);

#endif /* D_A_OBJ_SWLIGHT_H */
