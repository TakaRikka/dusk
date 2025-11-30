#ifndef D_A_OBJ_KWHEEL01_H
#define D_A_OBJ_KWHEEL01_H


#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjKWheel01_c
 * @brief Water Wheel/Gear
 *
 * @details Lakebed temple large water wheels/gears with clawshot targets and/or lifts
 */
class daObjKWheel01_c : public dBgS_MoveBgActor, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    daObjKWheel01_c() : dEvLib_callback_c(this) {}
    ~daObjKWheel01_c() {};

    cPhs__Step create1st();
    void setMtx();
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    bool eventStart();

private:
    /* 0x5B8 */ Mtx mNewBgMtx;
    /* 0x5E8 */ Mtx mTransformMtx;
    /* 0x618 */ J3DModel* mpModel;
    /* 0x61C */ s16 mYAngularVelocity;
    /* 0x620 */ fpc_ProcID m_klift_pid[4];  // JUT_ASSERT debug string shows this was called m_klift_pid
    /* 0x630 */ dBgW* mKLiftCollisions[4];
    /* 0x640 */ Mtx mKLiftBaseMatrices[4];
    /* 0x700 */ u8 mCreatedKLifts;

    // Switch number to check if water is flowing and the gear should turn, and whether the gear should accelerate to the target speed or start off at it
    int getSwNo() {
        return fopAcM_GetParamBit(this, 0, 8);
    }

    // Determines rotational direction of gear; 0 = counter-clockwise, 1 = clockwise
    u32 getArg0() {
        return fopAcM_GetParamBit(this, 8, 1);
    }

    int getEvent() {
        return fopAcM_GetParamBit(this, 9, 8);
    }

    // Determines the number of chain models each lift should use
    u32 getArg2() {
        return fopAcM_GetParamBit(this, 17, 6);
    }

    // Determines whether each of 4 potential lifts should be created and/or updated
    u32 getArg4567() {
        return fopAcM_GetParamBit(this, 23, 4);
    }

    u32 getOut() {
        return fopAcM_GetParamBit(this, 27, 1);
    }
};

STATIC_ASSERT(sizeof(daObjKWheel01_c) == 0x704);

struct daObjKWheel01_HIO_c : public mDoHIO_entry_c {
    daObjKWheel01_HIO_c();
    ~daObjKWheel01_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x4 */ s16 mTargetYAngularSpeed;
    /* 0x6 */ s16 mYAngularAcceleration;
};

#endif /* D_A_OBJ_KWHEEL01_H */
