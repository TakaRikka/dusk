#ifndef D_A_OBJ_HBOMBKOYA_H
#define D_A_OBJ_HBOMBKOYA_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjHBombkoya_c
 * @brief Destructable Bomb House
 *
 * @details
 *
 */
class daObjHBombkoya_c : public dBgS_MoveBgActor,
                         public request_of_phase_process_class,
                         public dEvLib_callback_c {
public:
    struct PSetTbl {
        /* 0x00 */ Vec m_position;
        /* 0x0C */ int m_startTime;  // The time at which this table is able to be used
        /* 0x10 */ f32 m_scale;
    };

    daObjHBombkoya_c() : dEvLib_callback_c(this) {}

    int create1st();
    bool setParticle(u16*, int, PSetTbl*, int, int);
    virtual ~daObjHBombkoya_c() {}

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSw2No() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x5A8 */  // dEvLib_callback_c
    /* 0x5B8 */ Mtx mBgMtx;
    /* 0x5E8 */ Mtx mMtx;
    /* 0x618 */ J3DModel* mpModel;
    /* 0x61C */ int mTimer;
    /* 0x620 */ bool mActive;
    /* 0x624 */ u32 mEmitterKeys[9];
    /* 0x648 */ dCcD_Stts mStts;
    /* 0x684 */ dCcD_Cyl mCyl;
};

STATIC_ASSERT(sizeof(daObjHBombkoya_c) == 0x7c0);


#endif /* D_A_OBJ_HBOMBKOYA_H */
