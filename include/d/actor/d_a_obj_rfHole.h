#ifndef D_A_OBJ_RFHOLE_H
#define D_A_OBJ_RFHOLE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daRfHole_c
 * @brief (Kakariko?) Roof Hole
 *
 * @details
 *
 */
class daRfHole_c : public dBgS_MoveBgActor {
public:
    enum Mode {
        MODE_WAIT,
        MODE_BREAK,
        MODE_BREAK_EFF,
        MODE_END,
    };

    typedef void (daRfHole_c::*ModeFunc)();

    void setBaseMtx();
    int CreateHeap();
    int create();
    static void rideCallBack(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    int playerAreaCheck();
    int Execute(Mtx**);
    void holeProc();
    void init_modeWait();
    void modeWait();
    void init_modeBreak();
    void modeBreak();
    void init_modeBreakEff();
    void modeBreakEff();
    void init_modeEnd();
    void modeEnd();
    void setBreakEffect();
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 field_0x5ae;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ s8 mRoofSoundOn;
};

STATIC_ASSERT(sizeof(daRfHole_c) == 0x5b4);

class daRfHole_HIO_c : public mDoHIO_entry_c {
public:
    daRfHole_HIO_c();
    virtual ~daRfHole_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ u8 field_0x14;
    /* 0x15 */ u8 field_0x15;
};

#endif /* D_A_OBJ_RFHOLE_H */
