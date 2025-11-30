#ifndef D_A_OBJ_WDSTICK_H
#define D_A_OBJ_WDSTICK_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daWdStick_c
 * @brief Wooden Stick
 *
 * @details
 *
 */
class daWdStick_c : public fopAc_ac_c {
public:
    typedef void (daWdStick_c::*modeFunc)();

    enum Mode {
        MODE_WAIT,
        MODE_CARRY,
        MODE_DROP,
        MODE_ROLL,
        MODE_SINK,
    };

    void setBaseMtx();
    virtual int CreateHeap();
    int create();
    static int createHeapCallBack(fopAc_ac_c*);
    void lightInit();
    void setLight();
    void setFire();
    virtual int Execute();
    void bgCheck();
    void setBoundSe();
    void mode_proc_call();
    void mode_init_wait();
    void mode_proc_wait();
    void mode_init_carry();
    void mode_proc_carry();
    void mode_init_drop();
    void mode_proc_drop();
    void mode_init_roll();
    void mode_proc_roll();
    BOOL chkWaterLineIn();
    void mode_init_sink();
    void mode_proc_sink();
    virtual int Draw();
    virtual int Delete();

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcSph mCcDSph;

private:
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mModel;
    /* 0x578 */ u8 mMode;
    /* 0x579 */ bool misCarrying;
    /* 0x57C */ cXyz field_0x57c;
    /* 0x588 */ u8 field_0x588[0x594 - 0x588];
    /* 0x594 */ cXyz field_0x594;
    /* 0x5A0 */ dBgS_ObjAcch mAcch;
    /* 0x778 */ dBgS_AcchCir mAcchCir;
    /* 0x7B8 */ dCcD_Stts mStts;
    /* 0x7F4 */ dCcD_Sph mSph1;
    /* 0x92C */ dCcD_Sph mSph2;
    /* 0xA64 */ cXyz field_0xa64;
    /* 0xA70 */ cXyz field_0xa70;
    /* 0xA7C */ cXyz field_0xa7c;
    /* 0xA88 */ cXyz field_0xa88;
    /* 0xA94 */ cXyz field_0xa94;
    /* 0xAA0 */ cXyz field_0xaa0;
    /* 0xAAC */ u8 field_0xaac;
    /* 0xAAD */ u8 field_0xaad;
    /* 0xAAE */ u16 field_0xaae;
    /* 0xAB0 */ u16 field_0xab0;
    /* 0xAB4 */ cXyz field_0xab4;
    /* 0xAC0 */ cXyz field_0xac0;
    /* 0xACC */ cXyz field_0xacc;
    /* 0xAD8 */ cXyz field_0xad8;
    /* 0xAE4 */ u32 field_0xae4[2];
    /* 0xAEC */ u32 field_0xaec[2];
    /* 0xAF4 */ u8 field_0xaf4;
};

STATIC_ASSERT(sizeof(daWdStick_c) == 0xaf8);

class daWdStick_HIO_c : public mDoHIO_entry_c {
public:
    daWdStick_HIO_c();
    virtual ~daWdStick_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ s16 field_0x06;
    /* 0x08 */ u8 field_0x08;
};


#endif /* D_A_OBJ_WDSTICK_H */
