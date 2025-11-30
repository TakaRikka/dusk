#ifndef D_A_OBJ_GROUNDWATER_H
#define D_A_OBJ_GROUNDWATER_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daGrdWater_c
 * @brief Ground Water
 *
 * @details
 *
 */
class daGrdWater_c : public dBgS_MoveBgActor {
public:
    typedef void (daGrdWater_c::*modeFunc)();

    enum Mode {
        WAIT,
        LEVEL_UP_A,
        LEVEL_DOWN_A,
        LEVEL_UP_B,
        LEVEL_DOWN_B,
    };

    void setBaseMtx();
    int CreateHeap();
    int create();
    daGrdWater_c();
    int Execute(Mtx**);
    void init_modeWait();
    void modeWait();
    void init_modeLevelUpA();
    void modeLevelUpA();
    void init_modeLevelDownA();
    void modeLevelDownA();
    void init_modeLevelUpB();
    void modeLevelUpB();
    void init_modeLevelDownB();
    void modeLevelDownB();
    int Draw();
    int Delete();

    u8 getSw() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSw2() { return fopAcM_GetParamBit(this, 16, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel1;
    /* 0x5AC */ J3DModel* mModel2;
    /* 0x5B0 */ mDoExt_btkAnm mBtk1;
    /* 0x5C8 */ mDoExt_btkAnm mBtk2;
    /* 0x5E0 */ mDoExt_brkAnm mBrk1;
    /* 0x5F8 */ mDoExt_brkAnm mBrk2;
    /* 0x610 */ mDoExt_bckAnm mBck1a;
    /* 0x62C */ mDoExt_bckAnm mBck1b;
    /* 0x648 */ mDoExt_bckAnm mBck1c;
    /* 0x664 */ mDoExt_bckAnm mBck1d;
    /* 0x680 */ mDoExt_bckAnm mBck2a;
    /* 0x69C */ mDoExt_bckAnm mBck2b;
    /* 0x6B8 */ mDoExt_bckAnm mBck2c;
    /* 0x6D4 */ mDoExt_bckAnm mBck2d;
    /* 0x6F0 */ u8 field_0x6f0;
    /* 0x6F1 */ u8 field_0x6f1;
    /* 0x6F2 */ u8 mMode;
    /* 0x6F3 */ u8 field_0x6f3;
};

STATIC_ASSERT(sizeof(daGrdWater_c) == 0x6f4);

class daGrdWater_HIO_c : public mDoHIO_entry_c {
public:
    daGrdWater_HIO_c();
    virtual ~daGrdWater_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u8 field_0x06;
    /* 0x07 */ u8 field_0x07;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
};


#endif /* D_A_OBJ_GROUNDWATER_H */
