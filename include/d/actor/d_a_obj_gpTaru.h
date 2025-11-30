#ifndef D_A_OBJ_GPTARU_H
#define D_A_OBJ_GPTARU_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daGpTaru_c
 * @brief Gunpowder Barrel
 *
 * @details
 *
 */
class daGpTaru_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    static int createHeapCallBack(fopAc_ac_c*);
    BOOL chkSinkAll();
    BOOL chkWaterLineIn();
    void bgCheck();
    void mode_proc_call();
    void mode_init_wait();
    void mode_proc_wait();
    void mode_init_carry();
    void mode_proc_carry();
    void mode_init_drop();
    void mode_proc_drop();
    void mode_init_roll();
    void mode_proc_roll();
    void mode_init_exploInit();
    void mode_proc_exploInit();
    void mode_init_explosion();
    void mode_proc_explosion();
    void mode_init_sink();
    void mode_proc_sink();
    void breakEffSet();

    virtual int CreateHeap();
    virtual int Execute();
    virtual int Draw();
    virtual int Delete();

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ u8 mMode;
    /* 0x579 */ u8 mIsPrevCarry;
    /* 0x57C */ cXyz field_0x57c;
    /* 0x588 */ cXyz field_0x588;
    /* 0x594 */ cXyz field_0x594;
    /* 0x5A0 */ u8 mIsPrevInWater;
    /* 0x5A4 */ dBgS_ObjAcch mAcch;
    /* 0x77C */ dBgS_AcchCir mAcchCir;
    /* 0x7BC */ dCcD_Stts mCcStts;
    /* 0x7F8 */ dCcD_Cyl mCcCyl;
    /* 0x934 */ u8 mExploTimer;
    /* 0x938 */ f32 field_0x938;
    /* 0x93C */ csXyz field_0x93c;
    /* 0x944 */ int field_0x944;
    /* 0x948 */ f32 mExplosionRange;
    /* 0x94C */ u32 mSmokeEmtID;
    /* 0x950 */ u32 field_0x950;
    /* 0x954 */ u8 field_0x954;
    /* 0x955 */ u8 field_0x955;
    /* 0x958 */ WIND_INFLUENCE mWind;
    /* 0x984 */ LIGHT_INFLUENCE mLight;
    /* 0x9A4 */ f32 mExploWindStrength;
    /* 0x9A8 */ u8 field_0x9a8;
};

STATIC_ASSERT(sizeof(daGpTaru_c) == 0x9ac);

class daGpTaru_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daGpTaru_HIO_c();
    virtual ~daGpTaru_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 weight;
    /* 0x08 */ f32 shake;
    /* 0x0C */ f32 explosion_range;
    /* 0x10 */ u8 explosion_wait_time;
    /* 0x11 */ u8 vibration;
    /* 0x14 */ cXyz effect_scale;
    /* 0x20 */ cXyz pre_smoke_scale;
    /* 0x2C */ f32 smoke_scale;
    /* 0x30 */ cXyz barrel_scale;
};


#endif /* D_A_OBJ_GPTARU_H */
