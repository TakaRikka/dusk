#ifndef D_A_OBJ_WELL_COVER_H
#define D_A_OBJ_WELL_COVER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjWCover_c
 * @brief Graveyard Well Cover
 *
 * @details
 *
 */
class daObjWCover_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    void action();
    void modeWait();
    void init_modeBreak();
    void modeBreak();
    int Draw();
    int Delete();

    u8 getSwNo() { return fopAcM_GetParam(this); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 field_0x5AC[0x5B0 - 0x5AC];
    /* 0x5B0 */ u8 field_0x5b0;
};

STATIC_ASSERT(sizeof(daObjWCover_c) == 0x5B4);

#endif /* D_A_OBJ_WELL_COVER_H */
