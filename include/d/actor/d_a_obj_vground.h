#ifndef D_A_OBJ_VGROUND_H
#define D_A_OBJ_VGROUND_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjVGnd_c
 * @brief Volcano Ground
 *
 * @details
 *
 */
class daObjVGnd_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    int draw();
    int _delete();

    u32 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_btkAnm* mpBtk;
    /* 0x578 */ mDoExt_brkAnm* mpBrk;
};

STATIC_ASSERT(sizeof(daObjVGnd_c) == 0x57C);
#endif /* D_A_OBJ_VGROUND_H */
