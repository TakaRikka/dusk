#ifndef D_A_OBJ_POTBOX_H
#define D_A_OBJ_POTBOX_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daPoTbox_c
 * @brief Poe Treasure Chest
 *
 * @details
 *
 */
class daPoTbox_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeOpenEnd();
    void modeOpenEnd();
    void init_modeEnd();
    void modeEnd();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    int getStat() { return fopAcM_GetParamBit(this, 8, 4); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpBoxModel;
    /* 0x5AC */ J3DModel* mpAnimModel;
    /* 0x5B0 */ J3DModel* mpLightModel;
    /* 0x5B4 */ dBgW* mpBgw2;
    /* 0x5B8 */ mDoExt_bckAnm mBck;
    /* 0x5D4 */ mDoExt_btkAnm mBtk;
    /* 0x5EC */ mDoExt_brkAnm mBrk;
    /* 0x604 */ u8 mMode;
    /* 0x605 */ u8 field_0x605;
    /* 0x606 */ u8 mStat;
    /* 0x607 */ u8 mIsSwitch;
};

STATIC_ASSERT(sizeof(daPoTbox_c) == 0x608);

#endif /* D_A_OBJ_POTBOX_H */
