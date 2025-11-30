#ifndef D_A_OBJ_KWHEEL00_H
#define D_A_OBJ_KWHEEL00_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjKWheel00_c
 * @brief Water Wheel/Gear
 *
 * @details Lakebed temple water wheels/gears without clawshot targets.
 * There are two types:
 *  1.) Larger gold-colored ones that are completely solid and lie near the end of the long axle
 *  2.) Smaller dark-colored ones where the largest gear teeth are grated and lie centered on a short axle (seen on bridges to/from central room)
 */

class daObjKWheel00_c : public dBgS_MoveBgActor, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    daObjKWheel00_c() : dEvLib_callback_c(this) {}
    ~daObjKWheel00_c() {}

    int create1st();
    void setMtx();
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    bool eventStart();

    int getSwNo() {
        return fopAcM_GetParamBit(this, 0, 8);
    }

    u32 getType() {
        return fopAcM_GetParamBit(this,17,1);
    }

    enum Type_e { TYPE_LARGE_GOLD, TYPE_SMALL_PLATINUM };

private:
    enum QuadrantalAngle_e { DEG_INVALID = -1, DEG_0, DEG_90, DEG_180, DEG_270, DEG_MAX};

    /* 0x5B8 */ Mtx mNewBgMtx;
    /* 0x5E8 */ Mtx mTransformMtx;
    /* 0x618 */ J3DModel* mpModel;
    /* 0x61C */ s16 mZAngularVelocity;
    /* 0x620 */ Type_e m_type;    // JUT_ASSERT string shows this was called m_type
    /* 0x624 */ dCcD_Stts mStts;
    /* 0x660 */ dCcD_Sph mLargeGearTeethSphereColliders[4];
    /* 0xB40 */ QuadrantalAngle_e mPrevQuadrantalZAngle;

    // Determines rotational direction of gear; 0 = counter-clockwise, 1 = clockwise
    u32 getArg0() {
        return fopAcM_GetParamBit(this, 8, 1);
    }

    s32 getEvent() {
        return fopAcM_GetParamBit(this, 9, 8);
    }

    
};


STATIC_ASSERT(sizeof(daObjKWheel00_c) == 0xb44);

struct daObjKWheel00_HIO_c : public mDoHIO_entry_c {
    daObjKWheel00_HIO_c();
    ~daObjKWheel00_HIO_c();

    void genMessage(JORMContext*);

    /* 0x4 */ s16 mTargetZAngularSpeed;
    /* 0x6 */ s16 mZAngularAcceleration;
};

#endif /* D_A_OBJ_KWHEEL00_H */
