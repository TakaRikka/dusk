#ifndef D_A_OBJ_LV4EDSHUTTER_H
#define D_A_OBJ_LV4EDSHUTTER_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"

/**
 * @ingroup actors-objects
 * @class daLv4EdShutter_c
 * @brief Arbiter's Grounds Death Sword Gate
 *
 * @details
 *
 */
class daLv4EdShutter_c : public dBgS_MoveBgActor, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    daLv4EdShutter_c() : dEvLib_callback_c(this) {}

    void setBaseMtx();
    int create();
    void move();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();
    void init_modeEnd();
    void modeEnd();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    virtual ~daLv4EdShutter_c() {}
    virtual bool eventStart();

    u8 getCloseSw() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getOpenSw() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getWaitTime() { return fopAcM_GetParamBit(this, 16, 8); }
    int getEventID() { return fopAcM_GetParamBit(this, 24, 8); }

    u8 getZenmetuSw() { return shape_angle.z & 0xFF; }
    u8 getStatus() { return mStatus; }

private:
    /* 0x5B8 */ request_of_phase_process_class mPhase;
    /* 0x5C0 */ J3DModel* mpModel;
    /* 0x5C4 */ u8 mMode;
    /* 0x5C6 */ u16 mWaitTimer;
    /* 0x5C8 */ u8 mOpenSw;
    /* 0x5C9 */ u8 mCloseSw;
    /* 0x5CA */ u8 mStatus;
    /* 0x5CB */ u8 field_0x5cb;
    /* 0x5CC */ u8 mZenmetuSw;
    /* 0x5D0 */ f32 mMovePos;
    /* 0x5D4 */ u8 field_0x5D4[0x5D8 - 0x5D4];
    /* 0x5D8 */ f32 mMoveSpeed;
};

STATIC_ASSERT(sizeof(daLv4EdShutter_c) == 0x5dc);

#endif /* D_A_OBJ_LV4EDSHUTTER_H */
