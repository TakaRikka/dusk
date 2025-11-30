#ifndef D_A_OBJ_LV8KEKKAITRAP_H
#define D_A_OBJ_LV8KEKKAITRAP_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daKekaiTrap_c
 * @brief Palace of Twilight Barrier Trap
 *
 * @details
 *
 */
class daKekaiTrap_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    void moveMain();
    void init_modeWait();
    void modeWait();
    void init_modeMoveUp();
    void modeMoveUp();
    void init_modeMoveDown();
    void modeMoveDown();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getType() { return fopAcM_GetParamBit(this, 0, 4); }
    int getSwBit() { return fopAcM_GetParamBit(this, 4, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mIsSwitch;
    /* 0x5AD */ u8 mSwbit;
    /* 0x5AE */ u8 mType;
    /* 0x5AF */ u8 mMode;
    /* 0x5B0 */ u8 mTimer;
};

STATIC_ASSERT(sizeof(daKekaiTrap_c) == 0x5b4);


#endif /* D_A_OBJ_LV8KEKKAITRAP_H */
