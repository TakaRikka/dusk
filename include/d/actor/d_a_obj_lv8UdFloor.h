#ifndef D_A_OBJ_LV8UDFLOOR_H
#define D_A_OBJ_LV8UDFLOOR_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daUdFloor_c
 * @brief Palace of Twilight Switch Step
 *
 * @details
 *
 */
class daUdFloor_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    void moveLift();
    void init_modeWait();
    void modeWait();
    void init_modeUpMoveInit();
    void modeUpMoveInit();
    u8 colorAnm(int);
    void init_modeUpMove();
    void modeUpMove();
    void init_modeDownMoveInit();
    void modeDownMoveInit();
    void init_modeDownMove();
    void modeDownMove();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getModelType() { return current.angle.x & 0xFF; }
    u8 getTimer() { return (current.angle.x & 0xFF00) >> 8; }

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    int getMoveHeight() { return fopAcM_GetParamBit(this, 16, 8); }
    int getMoveSpeed() { return fopAcM_GetParamBit(this, 24, 8); }
    
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ u8 field_0x5b8;
    /* 0x5B9 */ u8 mTimer;
    /* 0x5BA */ u8 field_0x5ba;
    /* 0x5BB */ u8 mModelType;
    /* 0x5BC */ u8 mIsSwitch;
    /* 0x5BD */ u8 mColorR;
    /* 0x5BE */ u8 mColorG;
    /* 0x5BF */ u8 mColorB;
    /* 0x5C0 */ f32 field_0x5c0;
    /* 0x5C4 */ u8 field_0x5c4;
    /* 0x5C5 */ u8 field_0x5c5;
};

STATIC_ASSERT(sizeof(daUdFloor_c) == 0x5c8);

#endif /* D_A_OBJ_LV8UDFLOOR_H */
