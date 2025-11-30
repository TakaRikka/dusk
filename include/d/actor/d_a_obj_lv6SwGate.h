#ifndef D_A_OBJ_LV6SWGATE_H
#define D_A_OBJ_LV6SWGATE_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daLv6SwGate_c
 * @brief Temple of Time Switch Gate
 *
 * @details
 *
 */
class daLv6SwGate_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    int create();
    u8 isSwitch();
    int Execute(Mtx**);
    void moveGate();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();
    void init_modeBreak();
    void modeBreak();
    int Draw();
    int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSwState() { return fopAcM_GetParamBit(this, 12, 4); }
    int getSw2() { return fopAcM_GetParamBit(this, 16, 8); }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;
private:
    /* 0x5A0  */ dCcD_Stts mCcStts;
    /* 0x5DC  */ dCcD_Cyl mCcDCyls[12];
    /* 0x14AC */ request_of_phase_process_class mPhase;
    /* 0x14B4 */ J3DModel* mModel1;
    /* 0x14B8 */ J3DModel* mModel2;
    /* 0x14BC */ u8 mMode;
    /* 0x14BD */ u8 mUnderlyingSwitchState;
    /* 0x14BE */ u8 mUnderylingSwitch;
    /* 0x14BE */ u8 mIsSwitch;
    /* 0x14C0 */ f32 mMovementAmount;
    /* 0x14C4 */ dBgW* mpBgW;
    /* 0x14C8 */ Mtx mMtx;
};

STATIC_ASSERT(sizeof(daLv6SwGate_c) == 0x14f8);

class daLv6SwGate_HIO_c : public fOpAcm_HIO_entry_c {
public:
    /* 80C73F0C */
    daLv6SwGate_HIO_c();

    void genMessage(JORMContext*);

    /* 0x8  */ float mSpeed;
    /* 0xC  */ float mMaximumSpeed;
    /* 0x10 */ float mAcceleration;
    /* 0x14 */ float mMovementAmount;
    /* 0x18 */ u8 unk18; // UNUSED
};


#endif /* D_A_OBJ_LV6SWGATE_H */
