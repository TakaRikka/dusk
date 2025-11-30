#ifndef D_A_OBJ_TMOON_H
#define D_A_OBJ_TMOON_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjTMoon_c
 * @brief Howling Moon
 *
 * @details
 *
 */
class daObjTMoon_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    int draw();
    int _delete();

    u32 getEventBit1() { return fopAcM_GetParamBit(this, 0, 10); }
    u32 getEventBit2() { return fopAcM_GetParamBit(this, 10, 10); }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ u16 field_0x574;
    /* 0x576 */ u16 field_0x576;
};

STATIC_ASSERT(sizeof(daObjTMoon_c) == 0x578);

#endif /* D_A_OBJ_TMOON_H */
