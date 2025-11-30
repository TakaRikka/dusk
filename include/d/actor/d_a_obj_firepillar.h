#ifndef D_A_OBJ_FIREPILLAR_H
#define D_A_OBJ_FIREPILLAR_H

#include "d/d_bg_s_acch.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjFPillar_c
 * @brief Fire Pillar
 *
 * @details
 *
 */
class daObjFPillar_c : public fopAc_ac_c {
public:
    typedef void (daObjFPillar_c::*actionFunc)();

    enum Action {
        ACTION_OFF,
        ACTION_ON_WAIT,
        ACTION_ON,
    };

    class daObjFPillar_prm {
    public:
        static u8 getArg0(daObjFPillar_c* i_this) { return fopAcM_GetParam(i_this) >> 8; }
        static u8 getArg1(daObjFPillar_c* i_this) { return fopAcM_GetParam(i_this) >> 24; }
        static u8 getArg2(daObjFPillar_c* i_this) { return fopAcM_GetParam(i_this) >> 16; }
        static u8 getSwNo(daObjFPillar_c* i_this) { return fopAcM_GetParam(i_this); }
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    void action();
    void actionOffInit();
    void actionOff();
    void actionOnWaitInit();
    void actionOnWait();
    void actionOnInit();
    void actionOn();
    int draw();
    int _delete();

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ int field_0x570;
    /* 0x574 */ dBgS_ObjAcch mAcch;
    /* 0x74C */ dBgS_AcchCir mAcchCir;
    /* 0x78C */ dCcD_Stts mStts;
    /* 0x7C8 */ dCcD_Cps mCps;
    /* 0x90C */ u8 field_0x90c[0x93c - 0x90c];
    /* 0x93C */ cXyz mCylScale;
    /* 0x948 */ Vec field_0x948;
    /* 0x954 */ cM3dGCpsS field_0x954;
    /* 0x970 */ u8 mAction;
    /* 0x972 */ u16 mActionTimer;
};

STATIC_ASSERT(sizeof(daObjFPillar_c) == 0x974);


#endif /* D_A_OBJ_FIREPILLAR_H */
