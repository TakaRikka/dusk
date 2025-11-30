#ifndef D_A_OBJ_LV6LBLOCK_H
#define D_A_OBJ_LV6LBLOCK_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daLv6Lblock_c
 * @brief Temple of Time L Block
 *
 * @details
 *
 */
class daLv6Lblock_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    u8 isSwitch();
    void moveBlock();
    void init_modeWait();
    void modeWait();
    void init_modeLiftUp();
    void modeLiftUp();
    void init_modeLiftDown();
    void modeLiftDown();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSwState() { return fopAcM_GetParamBit(this, 12, 4); }
    int getUpMax() { return fopAcM_GetParamBit(this, 8, 4); }
    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 mSwState;
    /* 0x5AE */ u8 mSwbit;
    /* 0x5AF */ u8 mIsSwitch;
    /* 0x5B0 */ f32 mMaxHeight;
    /* 0x5B4 */ f32 mBaseHeight;
};

STATIC_ASSERT(sizeof(daLv6Lblock_c) == 0x5b8);

class daLv6Lblock_HIO_c : public mDoHIO_entry_c {
public:
    daLv6Lblock_HIO_c();
    virtual ~daLv6Lblock_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 speed;
    /* 0x8 */ f32 move_amount;
    /* 0xC */ u8 unk_0xc;
};


#endif /* D_A_OBJ_LV6LBLOCK_H */
