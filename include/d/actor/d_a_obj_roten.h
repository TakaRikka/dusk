#ifndef D_A_OBJ_ROTEN_H
#define D_A_OBJ_ROTEN_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObj_Roten_c
 * @brief Goron Child Stall
 *
 * @details
 *
 */
class daObj_Roten_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    char* getResName();
    virtual ~daObj_Roten_c();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ JPABaseEmitter* mEmitters[2];
    /* 0x5B4 */ f32 mBossLightRefDist;
    /* 0x5B8 */ cXyz mParticlePos;
    /* 0x5C4 */ bool mWithinTime;
};

STATIC_ASSERT(sizeof(daObj_Roten_c) == 0x5c8);


#endif /* D_A_OBJ_ROTEN_H */
