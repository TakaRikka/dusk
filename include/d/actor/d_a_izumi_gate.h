#ifndef D_A_IZUMI_GATE_H
#define D_A_IZUMI_GATE_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daIzumiGate_c
 * @brief Ordon Spring Gate
 *
 * @details
 *
 */
class daIzumiGate_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    inline int create();

    /* 0x5A0 */ J3DModel* mpModel;
    /* 0x5A4 */ request_of_phase_process_class mPhaseReq;
};     // Size: 0x5AC

#endif /* D_A_IZUMI_GATE_H */
