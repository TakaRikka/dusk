#ifndef D_A_OBJ_HEAVYSW_H
#define D_A_OBJ_HEAVYSW_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daHeavySw_c
 * @brief Heavy Switch
 *
 * @details
 *
 */
class daHeavySw_c : public dBgS_MoveBgActor {
public:
    enum Mode_e { MODE_WAIT, MODE_RIDE, MODE_MOVE_INIT, MODE_MOVE, MODE_MOVE_END };

    void setBaseMtx();
    int CreateHeap();
    int create();
    static void rideCallBack(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    int Execute(Mtx**);
    void moveSwitch();
    void init_modeWait();
    void modeWait();
    void init_modeRide();
    void modeRide();
    void init_modeMoveInit();
    void modeMoveInit();
    void init_modeMove();
    void modeMove();
    void init_modeMoveEnd();
    void modeMoveEnd();
    int Draw();
    int Delete();

    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ f32 field_0x5b8;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ u8 field_0x5C0[0x5C4 - 0x5C0];
    /* 0x5C4 */ u8 field_0x5c4;
    /* 0x5C5 */ u8 field_0x5c5;
    /* 0x5C8 */ s32 field_0x5c8;
    /* 0x5CC */ csXyz field_0x5cc;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ u8 field_0x5d8;
    /* 0x5DC */ u32 field_0x5dc;
    /* 0x5E0 */ dBgS_AcchCir mAcchCir;
    /* 0x620 */ dBgS_ObjAcch mObjAcch;
    /* 0x7F8 */ u32 mShadowKey;
    /* 0x7FC */ u8 field_0x7fc;
};

class daHeavySw_HIO_c : public mDoHIO_entry_c {
public:
    daHeavySw_HIO_c();
    virtual ~daHeavySw_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ u8 field_0x30;
    /* 0x31 */ u8 field_0x31;
    /* 0x32 */ u8 field_0x32;
    /* 0x33 */ u8 field_0x33;
    /* 0x34 */ u8 field_0x34;
};

#endif /* D_A_OBJ_HEAVYSW_H */
