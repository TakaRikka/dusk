#ifndef D_A_OBJ_COWDOOR_H
#define D_A_OBJ_COWDOOR_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daCowdoor_c
 * @brief Ordon Ranch Stable Door
 *
 * @details
 *
 */
class daCowdoor_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class field_0x5a0;
    /* 0x5A8 */ J3DModel* field_0x5a8;
};

STATIC_ASSERT(sizeof(daCowdoor_c) == 0x5AC);

#endif /* D_A_OBJ_COWDOOR_H */
